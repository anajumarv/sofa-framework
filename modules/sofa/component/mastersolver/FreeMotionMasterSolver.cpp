/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/component/mastersolver/FreeMotionMasterSolver.h>

#include <sofa/component/constraintset/LCPConstraintSolver.h>

#include <sofa/core/ObjectFactory.h>
#include <sofa/core/VecId.h>

#include <sofa/helper/AdvancedTimer.h>

#include <sofa/simulation/common/BehaviorUpdatePositionVisitor.h>
#include <sofa/simulation/common/MechanicalOperations.h>
#include <sofa/simulation/common/SolveVisitor.h>
#include <sofa/simulation/common/VectorOperations.h>


namespace sofa
{

namespace component
{

namespace mastersolver
{

using namespace core::behavior;

FreeMotionMasterSolver::FreeMotionMasterSolver()
: constraintSolver(NULL)
, defaultSolver(NULL)
{
}

FreeMotionMasterSolver::~FreeMotionMasterSolver()
{
	if (defaultSolver != NULL)
		delete defaultSolver;
}

void FreeMotionMasterSolver::parse ( sofa::core::objectmodel::BaseObjectDescription* arg )
{
    defaultSolver = new constraintset::LCPConstraintSolver;
    defaultSolver->parse(arg);
}


void FreeMotionMasterSolver::init()
{
    getContext()->get(constraintSolver, core::objectmodel::BaseContext::SearchDown);
    if (constraintSolver == NULL && defaultSolver != NULL)
    {
        serr << "No ConstraintSolver found, using default LCPConstraintSolver" << sendl;
        this->getContext()->addObject(defaultSolver);
        constraintSolver = defaultSolver;
        defaultSolver = NULL;
    }
    else
    {
        delete defaultSolver;
        defaultSolver = NULL;
    }
}


void FreeMotionMasterSolver::step(const sofa::core::ExecParams* params /* PARAMS FIRST */, double dt)
{
	using helper::system::thread::CTime;
	using sofa::helper::AdvancedTimer;

	simulation::Node *context = (simulation::Node *)(this->getContext());

	simulation::common::VectorOperations vop(params, this->getContext());	  
	simulation::common::MechanicalOperations mop(params, this->getContext());

	MultiVecCoord pos(&vop, core::VecCoordId::position() );
    MultiVecDeriv vel(&vop, core::VecDerivId::velocity() );
	MultiVecCoord freePos(&vop, core::VecCoordId::freePosition() );
    MultiVecDeriv freeVel(&vop, core::VecDerivId::freeVelocity() );

	double time = 0.0;
	double timeTotal = 0.0;
	double timeScale = 1000.0 / (double)CTime::getTicksPerSec();

	if (displayTime.getValue())
	{
		time = (double) CTime::getTime();
		timeTotal = (double) CTime::getTime();
	}

	// This solver will work in freePosition and freeVelocity vectors.
	// We need to initialize them if it's not already done.
	simulation::MechanicalVInitVisitor<core::V_COORD>(params, core::VecCoordId::freePosition(), core::ConstVecCoordId::position(), true).execute(context);
	simulation::MechanicalVInitVisitor<core::V_DERIV>(params, core::VecDerivId::freeVelocity(), core::ConstVecDerivId::velocity()).execute(context);

	// Update the BehaviorModels
	// Required to allow the RayPickInteractor interaction

	if (f_printLog.getValue())
		serr << "updatePos called" << sendl;

	AdvancedTimer::stepBegin("UpdatePosition");
	simulation::BehaviorUpdatePositionVisitor updatePos(params, dt);
	context->execute(&updatePos);
	AdvancedTimer::stepEnd("UpdatePosition");

	if (f_printLog.getValue())
		serr << "updatePos performed - beginVisitor called" << sendl;

	simulation::MechanicalBeginIntegrationVisitor beginVisitor(params, dt);
	context->execute(&beginVisitor);

	if (f_printLog.getValue())
		serr << "beginVisitor performed - SolveVisitor for freeMotion is called" << sendl;	

	// Free Motion
	AdvancedTimer::stepBegin("FreeMotion");
	simulation::SolveVisitor freeMotion(params, dt, true);
	context->execute(&freeMotion);
	AdvancedTimer::stepEnd("FreeMotion");

	mop.propagateXAndV(freePos, freeVel);

	if (f_printLog.getValue())
		serr << " SolveVisitor for freeMotion performed" << sendl;

	if (displayTime.getValue())
	{
		sout << " >>>>> Begin display FreeMotionMasterSolver time" << sendl;
		sout <<" Free Motion " << ((double)CTime::getTime() - time) * timeScale << " ms" << sendl;

		time = (double)CTime::getTime();
	}	 	 

	// Collision detection and response creation
	AdvancedTimer::stepBegin("Collision");
	computeCollision(params);
	AdvancedTimer::stepEnd  ("Collision");
	
	mop.propagateX(pos);

	if (displayTime.getValue())
	{
		sout << " computeCollision " << ((double) CTime::getTime() - time) * timeScale << " ms" << sendl;
		time = (double)CTime::getTime();
	}

	// Solve constraints
	if (constraintSolver)
	{
		AdvancedTimer::stepBegin("ConstraintSolver");
		
		core::ConstraintParams cparams(*params);
		cparams.setX(freePos);
		cparams.setV(freeVel);
        
		constraintSolver->solveConstraint(&cparams, pos, vel);

		AdvancedTimer::stepEnd("ConstraintSolver");

		mop.propagateV(vel);

		MultiVecDeriv dx(&vop, constraintSolver->getDx());
		mop.propagateDx(dx);

		// "mapped" x = xfree + dx
		simulation::MechanicalVOpVisitor(params, pos, freePos, dx, 1.0 ).setOnlyMapped(true).execute(context);
	}

	if ( displayTime.getValue() )
	{
		sout << " contactCorrections " << ((double)CTime::getTime() - time) * timeScale << " ms" <<sendl;
		sout << "<<<<<< End display FreeMotionMasterSolver time." << sendl;
	}

	simulation::MechanicalEndIntegrationVisitor endVisitor(params /* PARAMS FIRST */, dt);
	context->execute(&endVisitor);
}


SOFA_DECL_CLASS(FreeMotionMasterSolver)

int FreeMotionMasterSolverClass = core::RegisterObject("Constraint solver")
.addAlias("MasterContactSolver")
.add< FreeMotionMasterSolver >()
;

} // namespace odesolver

} // namespace component

} // namespace sofa

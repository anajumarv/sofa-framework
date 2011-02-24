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
#include <sofa/simulation/common/MasterSolverImpl.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/simulation/common/CollisionVisitor.h>

#include <sofa/simulation/common/PropagateEventVisitor.h>
#include <sofa/simulation/common/CollisionBeginEvent.h>
#include <sofa/simulation/common/CollisionEndEvent.h>

#include <stdlib.h>
#include <math.h>


namespace sofa
{

namespace simulation
{


MasterSolverImpl::MasterSolverImpl()
{}

MasterSolverImpl::~MasterSolverImpl()
{}

void MasterSolverImpl::computeCollision(const core::ExecParams* params)
{
    if (this->f_printLog.getValue()) std::cerr<<"MasterSolverImpl::computeCollision()"<<endl;


    {
        CollisionBeginEvent evBegin;
        PropagateEventVisitor eventPropagation( &evBegin, params);
        eventPropagation.execute(getContext());
    }

    CollisionVisitor act(params);
    act.setTags(this->getTags());
    act.execute( getContext() );

    {
        CollisionEndEvent evEnd;
        PropagateEventVisitor eventPropagation( &evEnd, params);
        eventPropagation.execute(getContext());
    }
}

void MasterSolverImpl::integrate(double dt, const core::ExecParams* params)
{
    MechanicalIntegrationVisitor act( dt, params );
    act.setTags(this->getTags());
    act.execute( getContext() );
}

const MasterSolverImpl::Solvers& MasterSolverImpl::getSolverSequence()
{
	simulation::Node* gnode = dynamic_cast<simulation::Node*>( getContext() );
	assert( gnode );
	return gnode->solver;
}

// MasterSolverImpl::Pipeline* MasterSolverImpl::getPipeline()
// {
// 	simulation::Node* gnode = dynamic_cast<simulation::Node*>( getContext() );
// 	assert( gnode );
// 	return gnode->collisionPipeline;
// }


} // namespace simulation

} // namespace sofa

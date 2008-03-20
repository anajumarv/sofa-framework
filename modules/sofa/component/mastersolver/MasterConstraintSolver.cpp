#include <sofa/component/mastersolver/MasterConstraintSolver.h>
#include <sofa/component/odesolver/MasterContactSolver.h>

#include <sofa/simulation/tree/AnimateVisitor.h>
#include <sofa/simulation/tree/MechanicalVisitor.h>

#include <sofa/helper/LCPcalc.h>

#include <sofa/core/ObjectFactory.h>

#include <sofa/helper/system/thread/CTime.h>

#include <math.h>
#include <iostream>
#include <map>

namespace sofa
{

namespace component
{

namespace mastersolver
{

using namespace sofa::component::odesolver;
using namespace sofa::defaulttype;
using namespace helper::system::thread;
using namespace core::componentmodel::behavior;

MasterConstraintSolver::MasterConstraintSolver()
    :_tol( initData(&_tol, 0.001, "tolerance", "Tolerance of the Gauss-Seidel")),
     _mu( initData(&_mu, 0.6, "mu", "Friction coefficient")),
     _maxIt( initData(&_maxIt, 1000, "maxIterations", "Maximum number of iterations of the Gauss-Seidel"))
{
}

MasterConstraintSolver::~MasterConstraintSolver()
{
}

void MasterConstraintSolver::init()
{
    getContext()->get<core::componentmodel::behavior::BaseConstraintCorrection> ( &constraintCorrections, core::objectmodel::BaseContext::SearchDown );
}

void MasterConstraintSolver::step ( double dt )
{
    simulation::tree::GNode *context = dynamic_cast<simulation::tree::GNode *>(this->getContext()); // access to current node

    // Update the BehaviorModels
    // Required to allow the RayPickInteractor interaction
    for (simulation::tree::GNode::ChildIterator it = context->child.begin(); it != context->child.end(); ++it)
    {
        for (unsigned i=0; i<(*it)->behaviorModel.size(); i++)
            (*it)->behaviorModel[i]->updatePosition(dt);
    }

    simulation::tree::MechanicalBeginIntegrationVisitor beginVisitor(dt);
    context->execute(&beginVisitor);

    // Free Motion
    for (simulation::tree::GNode::ChildIterator it = context->child.begin(); it != context->child.end(); ++it)
    {
        for (unsigned i=0; i<(*it)->solver.size(); i++)
            (*it)->solver[i]->solve(dt);
    }

    simulation::tree::MechanicalPropagateFreePositionVisitor().execute(context);

    core::componentmodel::behavior::BaseMechanicalState::VecId dx_id = core::componentmodel::behavior::BaseMechanicalState::VecId::dx();
    simulation::tree::MechanicalVOpVisitor(dx_id).execute(context);
    simulation::tree::MechanicalPropagateDxVisitor(dx_id).execute(context);
    simulation::tree::MechanicalVOpVisitor(dx_id).execute(context);

    computeCollision();

    for (unsigned int i=0; i<constraintCorrections.size(); i++)
    {
        core::componentmodel::behavior::BaseConstraintCorrection* cc = constraintCorrections[i];
        cc->resetContactForce();
    }

    unsigned int numConstraints = 0;

    // mechanical action executed from root node to propagate the constraints
    simulation::tree::MechanicalResetConstraintVisitor().execute(context);
    double unused=0;
    // calling applyConstraint
    simulation::tree::MechanicalAccumulateConstraint(numConstraints, unused).execute(context);

    _dFree.resize(numConstraints);
    _W.resize(numConstraints,numConstraints);
    _constraintsType.resize(numConstraints);
    _result.resize(2*numConstraints+1);

    // calling getConstraintValue
    MechanicalGetConstraintValueVisitor(_dFree.ptr()).execute(context);
    // calling getConstraintType
    MechanicalGetConstraintTypeVisitor(_constraintsType.ptr()).execute(context);

    for (unsigned int i=0; i<constraintCorrections.size(); i++ )
    {
        core::componentmodel::behavior::BaseConstraintCorrection* cc = constraintCorrections[i];
        cc->getCompliance ( &_W ); // getDelassusOperator(_W) = H*C*Ht
    }

    gaussSeidelConstraint ( numConstraints, _dFree.ptr(), _W.lptr(), _result.ptr(), _constraintsType.ptr() );

//	helper::afficheLCP(_dFree.ptr(), _W.lptr(), _result.ptr(),  numConstraints);

    for (unsigned int i=0; i<constraintCorrections.size(); i++)
    {
        core::componentmodel::behavior::BaseConstraintCorrection* cc = constraintCorrections[i];
        cc->applyContactForce(&_result);
    }

    simulation::tree::MechanicalPropagateAndAddDxVisitor().execute(context);
    simulation::tree::MechanicalPropagatePositionAndVelocityVisitor().execute(context);

    for (unsigned int i=0; i<constraintCorrections.size(); i++)
    {
        core::componentmodel::behavior::BaseConstraintCorrection* cc = constraintCorrections[i];
        cc->resetContactForce();
    }

    simulation::tree::MechanicalEndIntegrationVisitor endVisitor(dt);
    context->execute(&endVisitor);
}


void MasterConstraintSolver::gaussSeidelConstraint(int dim, double* dfree, double** w, double* res, bool* type)
{
    int i, j, k;

    double f_1[3], d_1[3];
    double error=0.0;
    double dn, dt, ds;

    double tolerance = _tol.getValue();
    int numItMax = _maxIt.getValue();
    double mu = _mu.getValue();

    std::map<int, helper::LocalBlock33> W33;

    for(i=0; i<numItMax; i++)
    {
        error=0.0;
        for(j=0; j<dim; j++)
        {
            f_1[0] = res[dim+j];
            res[j]=dfree[j];
            for(k=0; k<dim; k++)
                res[j] += w[j][k] * res[dim+k];

            res[j] -= w[j][j] * res[dim+j];

            if(type[j])  // bilateral
            {
                res[dim+j]=-res[j]/w[j][j];
                error += fabs( w[j][j] * (res[dim+j] - f_1[0]) );
            }
            else if (!mu)    // unilateral without friction
            {
                if(res[j]<0)
                    res[dim+j]=-res[j]/w[j][j];
                else
                    res[dim+j]=0.0;

                error += fabs( w[j][j] * (res[dim+j] - f_1[0]) );
            }
            else // unilateral with friction
            {
                // put the previous value of the contact force in a buffer and put the current value to 0
                /* f_1[0] = res[dim+j]; */ f_1[1] = res[dim+j+1]; f_1[2] = res[dim+j+2];
                res[dim+j]=0.0; res[dim+j+1]=0.0; res[dim+j+2]=0.0;

                // computation of actual d due to contribution of other contacts
                dn=dfree[j]; dt=dfree[j+1]; ds=dfree[j+2];
                for (k=0; k<dim; k++)
                {
                    dn+=w[j  ][k]* res[dim+k];
                    dt+=w[j+1][k]* res[dim+k];
                    ds+=w[j+2][k]* res[dim+k];
                }

                d_1[0] = dn + w[j  ][j  ]*f_1[0]+w[j  ][j+1]*f_1[1]+w[j  ][j+2]*f_1[2];
                d_1[1] = dt + w[j+1][j  ]*f_1[0]+w[j+1][j+1]*f_1[1]+w[j+1][j+2]*f_1[2];
                d_1[2] = ds + w[j+2][j  ]*f_1[0]+w[j+2][j+1]*f_1[1]+w[j+2][j+2]*f_1[2];

                if(W33[j].computed==false)
                    W33[j].compute(w[j][j], w[j][j+1], w[j][j+2], w[j+1][j+1], w[j+1][j+2], w[j+2][j+2]);

                W33[j].GS_State(mu,dn,dt,ds,f_1[0],f_1[1],f_1[2]);

                error += helper::absError(dn,dt,ds,d_1[0],d_1[1],d_1[2]);

                res[dim+j]=f_1[0]; res[dim+j+1]=f_1[1]; res[dim+j+2]=f_1[2];

                j+=2;
            }
        }

        if(error < tolerance)
            break;
    }

    for(i=0; i<dim; i++)
        res[i] = res[i+dim];

    if(error >= tolerance)
        std::cerr << "No convergence in gaussSeidelConstraint : error = " << error << std::endl;
}


SOFA_DECL_CLASS ( MasterConstraintSolver )

int MasterConstraintSolverClass = core::RegisterObject ( "Constraint solver" )
        .add< MasterConstraintSolver >()
        ;

} // namespace odesolver

} // namespace component

} // namespace sofa

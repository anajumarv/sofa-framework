/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 3      *
*                (c) 2006-2008 MGH, INRIA, USTL, UJF, CNRS                    *
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
#ifndef SOFA_SIMULATION_TREE_MASTERSOLVERIMPL_H
#define SOFA_SIMULATION_TREE_MASTERSOLVERIMPL_H

#include <sofa/core/componentmodel/behavior/MasterSolver.h>
#include <sofa/simulation/common/Node.h>

namespace sofa
{

namespace simulation
{


/**
 *  \brief Implementation of MasterSolver relying on simulation::Node.
 *
 *  Note that it is in a preliminary stage, hence its fonctionnalities and API will
 *  certainly change soon.
 *
 */

class MasterSolverImpl : public sofa::core::componentmodel::behavior::MasterSolver
{
public:

    MasterSolverImpl();

    virtual ~MasterSolverImpl();

    /// @name Visitors
    /// These methods provides an abstract view of the mechanical system to animate.
    /// They are implemented by executing Visitors in the subtree of the scene-graph below this solver.
    /// @{

    /// Activate collision pipeline
    virtual void computeCollision();

    /// Activate OdeSolvers
    virtual void integrate(double dt);
	
	protected:
		typedef simulation::Node::Sequence<core::componentmodel::behavior::OdeSolver> Solvers;
		typedef core::componentmodel::collision::Pipeline Pipeline;
		const Solvers& getSolverSequence();
// 		Pipeline* getPipeline();

    /// @}
};

} // namespace simulation

} // namespace sofa

#endif

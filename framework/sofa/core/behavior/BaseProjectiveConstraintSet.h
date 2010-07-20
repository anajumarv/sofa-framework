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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_CORE_BEHAVIOR_BASEPROJECTIVECONSTRAINTSET_H
#define SOFA_CORE_BEHAVIOR_BASEPROJECTIVECONSTRAINTSET_H

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/behavior/MultiMatrixAccessor.h>

namespace sofa
{

  namespace core
  {

    namespace behavior
    {

      /**
   *  \brief Component computing projective constraints within a simulated body.
   *
   *  This class define the abstract API common to all projective constraints.
   *  A BaseConstraint computes constraints applied to one or more simulated body
   *  given its current position and velocity.
   *
   *  Constraints can be internal to a given body (attached to one MechanicalState,
   *  see the Constraint class), or link several bodies together (such as contacts,
   *  see the InteractionConstraint class).
   *
   */
      class SOFA_CORE_API BaseProjectiveConstraintSet : public virtual objectmodel::BaseObject
      {
      public:
        SOFA_CLASS(BaseProjectiveConstraintSet, objectmodel::BaseObject);

        BaseProjectiveConstraintSet()
          : group(initData(&group, 0, "group", "ID of the group containing this constraint. This ID is used to specify which constraints are solved by which solver, by specifying in each solver which groups of constraints it should handle."))
        {
        }

        virtual ~BaseProjectiveConstraintSet() {}

        /// Get the ID of the group containing this constraint. This ID is used to specify which constraints are solved by which solver, by specifying in each solver which groups of constraints it should handle.
        int getGroup() const { return group.getValue(); }

        /// Set the ID of the group containing this constraint. This ID is used to specify which constraints are solved by which solver, by specifying in each solver which groups of constraints it should handle.
        void setGroup(int g) { group.setValue(g); }




        /// @name Vector operations
        /// @{

        /// Project dx to constrained space (dx models an acceleration).
        virtual void projectResponse() = 0;

        /// Project the L matrix of the Lagrange Multiplier equation system.
        virtual void projectJacobianMatrix() = 0;

        /// Project v to constrained space (v models a velocity).
        virtual void projectVelocity() = 0;

        /// Project x to constrained space (x models a position).
        virtual void projectPosition() = 0;

        /// Project vFree to constrained space (vFree models a velocity).
        virtual void projectFreeVelocity() = 0;

        /// Project xFree to constrained space (xFree models a position).
        virtual void projectFreePosition() = 0;

        /// @}

        /// @name Matrix operations
        /// @{

        /// Project the compliance Matrix to constrained space.
        virtual void projectResponse(double **){};

        /// Project the global Mechanical Matrix to constrained space using offset parameter
        virtual void applyConstraint(const sofa::core::behavior::MultiMatrixAccessor* /*matrix*/){};

        /// Project the global Mechanical Vector to constrained space using offset parameter
        virtual void applyConstraint(defaulttype::BaseVector* /*vector*/, const sofa::core::behavior::MultiMatrixAccessor* /*matrix*/){};



        /// @}


        /// If the constraint is applied only on a subset of particles.
        /// That way, we can optimize the time spent traversing the mappings
        /// Deactivated by default. The constraints using only a subset of particles should activate the mask,
        /// and during projectResponse(), insert the indices of the particles modified
        virtual bool useMask() const {return false;}


        /// says if the constraint is holonomic or not
        /// holonomic constraints can be processed using different methods such as :
        /// projection - reducing the degrees of freedom - simple lagrange multiplier process
        /// Non-holonomic constraints (like contact, friction...) need more specific treatments
        virtual bool isHolonomic() {return false; }

      protected:
        Data<int> group;
      };

    } // namespace behavior

  } // namespace core

} // namespace sofa

#endif

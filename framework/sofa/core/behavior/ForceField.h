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
#ifndef SOFA_CORE_BEHAVIOR_FORCEFIELD_H
#define SOFA_CORE_BEHAVIOR_FORCEFIELD_H

#include <sofa/core/core.h>
#include <sofa/core/behavior/BaseForceField.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/core/MechanicalParams.h>
#include <sofa/defaulttype/BaseVector.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>

namespace sofa
{

namespace core
{

namespace behavior
{

/**
 *  \brief Component computing forces within a simulated body.
 *
 *  This class define the abstract API common to force fields using a
 *  given type of DOFs.
 *  A force field computes forces applied to one simulated body
 *  given its current position and velocity.
 *
 *  For implicit integration schemes, it must also compute the derivative
 *  ( df, given a displacement dx ).
 */
template<class TDataTypes>
class ForceField : public BaseForceField
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(ForceField, TDataTypes), BaseForceField);

    typedef TDataTypes DataTypes;
    typedef typename DataTypes::Coord            Coord;
    typedef typename DataTypes::Deriv            Deriv;
    typedef typename DataTypes::VecCoord         VecCoord;
    typedef typename DataTypes::VecDeriv         VecDeriv;
    typedef core::objectmodel::Data<VecCoord>    DataVecCoord;
    typedef core::objectmodel::Data<VecDeriv>    DataVecDeriv;
    
    ForceField(MechanicalState<DataTypes> *mm = NULL);

    virtual ~ForceField();

    virtual void init();

    /// Retrieve the associated MechanicalState
    MechanicalState<DataTypes>* getMState() { return mstate; }


    /// @name Vector operations
    /// @{

    /// Given the current position and velocity states, update the current force
    /// vector by computing and adding the forces associated with this
    /// ForceField.
    ///
    /// If the ForceField can be represented as a matrix, this method computes
    /// $ f += B v + K x $
    ///
    /// This method retrieves the force, x and v vector from the MechanicalState
    /// and call the internal addForce(DataVecDeriv&,const DataVecCoord&,const DataVecDeriv&, const MechanicalParams*)
    /// method implemented by the component.
	virtual void addForce(MultiVecDerivId fId , const MechanicalParams* mparams );

    /// Given the current position and velocity states, update the current force
    /// vector by computing and adding the forces associated with this
    /// ForceField.
    ///
    /// If the ForceField can be represented as a matrix, this method computes
    /// $ f += B v + K x $
    ///
    /// This is the method that should be implemented by the component
	virtual void addForce(DataVecDeriv& f , const DataVecCoord& x , const DataVecDeriv& v , const MechanicalParams* /*mparams*/)
#ifdef SOFA_DEPRECATE_OLD_API
	= 0;
#else
	;
    /// @deprecated use instead addForce(DataVecDeriv&,const DataVecCoord&,const DataVecDeriv&, const MechanicalParams*)
    virtual void addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v);
#endif

    /// Compute the force derivative given a small displacement from the
    /// position and velocity used in the previous call to addForce().
    ///
    /// The derivative should be directly derived from the computations
    /// done by addForce. Any forces neglected in addDForce will be integrated
    /// explicitly (i.e. using its value at the beginning of the timestep).
    ///
    /// If the ForceField can be represented as a matrix, this method computes
    /// $ df += kFactor K dx + bFactor B dx $
    ///
    /// This method retrieves the force and dx vector from the MechanicalState
    /// and call the internal addDForce(VecDeriv&,const VecDeriv&,double,double)
    /// method implemented by the component.
    virtual void addDForce(MultiVecDerivId dfId , const MechanicalParams* mparams );

    virtual void addDForce(DataVecDeriv&   df , const DataVecDeriv&   dx , const MechanicalParams* mparams )
#ifdef SOFA_DEPRECATE_OLD_API
    = 0;
#else
    ;
    /// @deprecated
    virtual void addDForce(VecDeriv&       df , const VecDeriv&       dx , double kFactor, double bFactor);
    /// @deprecated
    virtual void addDForce(VecDeriv&       df , const VecDeriv&       dx );
#endif



    /// Get the potential energy associated to this ForceField.
    ///
    /// Used to extimate the total energy of the system by some
    /// post-stabilization techniques.
    ///
    /// This method must be implemented by the component, and is usually called
    /// by the generic ForceField::getPotentialEnergy(const MechanicalParams* mparams) method.

    virtual double getPotentialEnergy(const MechanicalParams* mparams) const  ;

    virtual double getPotentialEnergy(const DataVecCoord&   x, const MechanicalParams* /*mparams*/) const
#ifdef SOFA_DEPRECATE_OLD_API
    = 0;
#else
    ;
    /// @deprecated
    virtual double getPotentialEnergy(const VecCoord& x) const;
#endif

    /// @}

    /// @name Matrix operations
    /// @{

    virtual void addKToMatrix(const sofa::core::behavior::MultiMatrixAccessor* matrix, const MechanicalParams* mparams );

    /// @deprecated
    virtual void addKToMatrix(sofa::defaulttype::BaseMatrix * matrix, double kFact, unsigned int &offset)               ;



    virtual void addBToMatrix(const sofa::core::behavior::MultiMatrixAccessor* matrix, const MechanicalParams* mparams);

    /// @deprecated
    virtual void addBToMatrix(sofa::defaulttype::BaseMatrix * matrix, double bFact, unsigned int &offset)              ;

    /// @}





    /// Pre-construction check method called by ObjectFactory.
    /// Check that DataTypes matches the MechanicalState.
    template<class T>
    static bool canCreate(T*& obj, objectmodel::BaseContext* context, objectmodel::BaseObjectDescription* arg)
    {
        if (dynamic_cast<MechanicalState<DataTypes>*>(context->getMechanicalState()) == NULL)
            return false;
        return BaseObject::canCreate(obj, context, arg);
    }

    virtual std::string getTemplateName() const{
        return templateName(this);
    }

    static std::string templateName(const ForceField<DataTypes>* = NULL){
        return DataTypes::Name();
    }

protected:
    MechanicalState<DataTypes> *mstate;
};

#if defined(WIN32) && !defined(SOFA_BUILD_CORE)
extern template class SOFA_CORE_API ForceField<defaulttype::Vec3dTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Vec2dTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Vec1dTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Vec6dTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Rigid3dTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Rigid2dTypes>;

extern template class SOFA_CORE_API ForceField<defaulttype::Vec3fTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Vec2fTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Vec1fTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Vec6fTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Rigid3fTypes>;
extern template class SOFA_CORE_API ForceField<defaulttype::Rigid2fTypes>;
#endif

} // namespace behavior

} // namespace core

} // namespace sofa

#endif

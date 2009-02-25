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
#ifndef SOFA_COMPONENT_CONSTRAINT_BOXROI_H
#define SOFA_COMPONENT_CONSTRAINT_BOXROI_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/defaulttype/Vec.h>
#include <sofa/core/objectmodel/DDGNode.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/componentmodel/behavior/BaseConstraint.h>
#include <sofa/component/constraint/FixedConstraint.h>
#include <sofa/core/objectmodel/XDataPtr.h>

namespace sofa
{

namespace component
{

namespace constraint
{

using namespace core::componentmodel::behavior;

/** Keep fixed all the particles located inside a given box.
*/
template <class DataTypes>
class BoxROI : public virtual core::objectmodel::DDGNode, public virtual core::objectmodel::BaseObject
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Real Real;
    typedef defaulttype::Vec<6,Real> Vec6;
    typedef topology::PointSubset SetIndex;

public:

    BoxROI();

    ~BoxROI();

    void init();

    void update();

    void draw();

    bool addBBox(double* minBBox, double* maxBBox);

    /// this constraint is holonomic
    bool isHolonomic() {return true;}

    /// Pre-construction check method called by ObjectFactory.
    /// Check that DataTypes matches the MechanicalState.
    template<class T>
    static bool canCreate(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
    {
        if (dynamic_cast<MechanicalState<DataTypes>*>(context->getMechanicalState()) == NULL)
            return false;
        return BaseObject::canCreate(obj, context, arg);
    }

    /// Construction method called by ObjectFactory.
    template<class T>
    static void create(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
    {
        core::objectmodel::BaseObject::create(obj, context, arg);
        if (context)
        {
            obj->mstate = dynamic_cast<MechanicalState<DataTypes>*>(context->getMechanicalState());
        }
    }


    VecCoord* x0;
    Data< helper::vector<Vec6> > boxes;
    XDataPtr<DataTypes>* const f_X0;
    Data<SetIndex> f_indices;
    Data<double> _drawSize;
    MechanicalState<DataTypes>* mstate;
};

#if defined(WIN32) && !defined(SOFA_COMPONENT_CONSTRAINT_BOXROI_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
template class SOFA_COMPONENT_CONSTRAINT_API BoxROI<Vec3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_COMPONENT_CONSTRAINT_API BoxROI<Vec3fTypes>;
#endif
#endif

} // namespace constraint

} // namespace component

} // namespace sofa

#endif

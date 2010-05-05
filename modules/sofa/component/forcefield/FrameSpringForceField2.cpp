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
// Author: François Faure, INRIA-UJF, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
#define SOFA_COMPONENT_FORCEFIELD_FRAMESPRINGFORCEFIELD2_CPP
#include <sofa/component/forcefield/FrameSpringForceField2.inl>
#include <sofa/core/behavior/PairInteractionForceField.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/ObjectFactory.h>
//#include <typeinfo>


namespace sofa
{

namespace component
{

namespace forcefield
{

SOFA_DECL_CLASS(FrameSpringForceField2)

using namespace sofa::defaulttype;


// Register in the Factory
int FrameSpringForceField2Class = core::RegisterObject("Springs between frames.")
#ifndef SOFA_FLOAT
        .add< FrameSpringForceField2<Rigid3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
//.add< FrameSpringForceField2<Rigid3fTypes> >()
#endif
        ;
#ifndef SOFA_FLOAT
template class SOFA_COMPONENT_FORCEFIELD_API FrameSpringForceField2<Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
//template class SOFA_COMPONENT_FORCEFIELD_API FrameSpringForceField2<Rigid3fTypes>;
#endif


} // namespace forcefield

} // namespace component

} // namespace sofa


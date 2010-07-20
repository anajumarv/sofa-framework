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
#include "MechanicalMapping.inl"
#include "MechanicalState.h"
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/defaulttype/RigidTypes.h>

namespace sofa
{

namespace core
{

namespace behavior
{

using namespace sofa::defaulttype;
    
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec2dTypes>, MechanicalState<Vec2dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Vec1dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec6dTypes>, MechanicalState<Vec6dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid2dTypes>, MechanicalState<Vec2dTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Rigid2dTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid3dTypes>, MechanicalState<Vec3dTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Rigid3dTypes> > ;

template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Vec1fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec2fTypes>, MechanicalState<Vec2fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec6fTypes>, MechanicalState<Vec6fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid3fTypes>, MechanicalState<Vec3fTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid2fTypes>, MechanicalState<Vec2fTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Rigid2fTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Rigid3fTypes> > ;



template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Vec1fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Vec1dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec2dTypes>, MechanicalState<Vec2fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec2fTypes>, MechanicalState<Vec2dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec6dTypes>, MechanicalState<Vec6fTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec6fTypes>, MechanicalState<Vec6dTypes> >;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid2dTypes>, MechanicalState<Vec2fTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid2fTypes>, MechanicalState<Vec2dTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Rigid2fTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Rigid2dTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid3dTypes>, MechanicalState<Vec3fTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Rigid3fTypes>, MechanicalState<Vec3dTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Rigid3fTypes> > ;
template class SOFA_CORE_API MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Rigid3dTypes> > ;


} // namespace behavior

} // namespace core

} // namespace sofa

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

#include <sofa/component/topology/TetrahedronSetTopologyAlgorithms.h>
#include <sofa/component/topology/TetrahedronSetTopologyAlgorithms.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace topology
{

using namespace sofa::defaulttype;
SOFA_DECL_CLASS(TetrahedronSetTopologyAlgorithms)
int TetrahedronSetTopologyAlgorithmsClass = core::RegisterObject("Tetrahedron set topology algorithms")
#ifndef SOFA_FLOAT
        .add< TetrahedronSetTopologyAlgorithms<Vec3dTypes> >()
        .add< TetrahedronSetTopologyAlgorithms<Vec2dTypes> >()
        .add< TetrahedronSetTopologyAlgorithms<Vec1dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< TetrahedronSetTopologyAlgorithms<Vec3fTypes> >()
        .add< TetrahedronSetTopologyAlgorithms<Vec2fTypes> >()
        .add< TetrahedronSetTopologyAlgorithms<Vec1fTypes> >()
#endif
        ;

#ifndef SOFA_FLOAT
template class TetrahedronSetTopologyAlgorithms<Vec3dTypes>;
template class TetrahedronSetTopologyAlgorithms<Vec2dTypes>;
template class TetrahedronSetTopologyAlgorithms<Vec1dTypes>;
#endif

#ifndef SOFA_DOUBLE
template class TetrahedronSetTopologyAlgorithms<Vec3fTypes>;
template class TetrahedronSetTopologyAlgorithms<Vec2fTypes>;
template class TetrahedronSetTopologyAlgorithms<Vec1fTypes>;
#endif

} // namespace topology

} // namespace component

} // namespace sofa


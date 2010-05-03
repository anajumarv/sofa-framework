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
#define SOFA_COMPONENT_MAPPING_BARYCENTRICMAPPING_CPP
#include <sofa/component/mapping/BarycentricMapping.inl>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/componentmodel/behavior/MappedModel.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/Mapping.inl>
#include <sofa/core/componentmodel/behavior/MechanicalMapping.inl>

namespace sofa
{

namespace component
{

namespace mapping
{

using namespace sofa::defaulttype;
using namespace core;
using namespace core::componentmodel::behavior;

SOFA_DECL_CLASS(BarycentricMapping)

// Register in the Factory
int BarycentricMappingClass = core::RegisterObject("Mapping using barycentric coordinates of the child with respect to cells of its parent")
#ifndef SOFA_FLOAT
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3dTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<ExtVec3fTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<Vec3dTypes> > > >()
// .add< BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<ExtVec3dTypes> > > >()
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3dTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<Rigid3dTypes> > > >()
#endif
#ifndef SOFA_DOUBLE
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3fTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<ExtVec3fTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<Vec3fTypes> > > >()
// .add< BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<ExtVec3dTypes> > > >()
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3fTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<Rigid3fTypes> > > >()
#endif
#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3dTypes> > > >()
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<Vec3dTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<Vec3fTypes> > > >()
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3dTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<Rigid3dTypes> > > >()
.add< BarycentricMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3fTypes> > > >()
.add< BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<Rigid3fTypes> > > >()
#endif
#endif
;

#ifndef SOFA_FLOAT
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3dTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<Vec3dTypes> > >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<ExtVec3dTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<ExtVec3fTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3dTypes, ExtVec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3dTypes, Vec3dTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3dTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3dTypes, ExtVec3fTypes >;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3fTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<Vec3fTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<ExtVec3fTypes> > >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<ExtVec3dTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3fTypes, ExtVec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3fTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3fTypes, ExtVec3fTypes >;
// template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3fTypes, ExtVec3dTypes >;
#endif
#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3dTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3dTypes>, MappedModel<Vec3fTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapping< Mapping< State<Vec3fTypes>, MappedModel<Vec3dTypes> > >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapper<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API TopologyBarycentricMapper<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperRegularGridTopology<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperSparseGridTopology<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperMeshTopology<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperEdgeSetTopology<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTriangleSetTopology<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperQuadSetTopology<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperTetrahedronSetTopology<Vec3fTypes, Vec3dTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3dTypes, Vec3fTypes >;
template class SOFA_COMPONENT_MAPPING_API BarycentricMapperHexahedronSetTopology<Vec3fTypes, Vec3dTypes >;
#endif
#endif


} // namespace mapping

} // namespace component

} // namespace sofa


/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2010 MGH, INRIA, USTL, UJF, CNRS                    *
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



// File automatically generated by "generateTypedef"


#ifndef SOFA_TYPEDEF_ForceField_double_H
#define SOFA_TYPEDEF_ForceField_double_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>


#ifdef SOFA_GPU_CUDA
#include <sofa/gpu/cuda/CudaTypesBase.h>
#include <sofa/gpu/cuda/CudaTypes.h>
#endif
#ifdef SOFA_GPU_OPENCL
#include <sofa/gpu/opencl/OpenCLTypes.h>
#endif


#include <sofa/component/forcefield/AspirationForceField.h>
#include <sofa/component/forcefield/BeamFEMForceField.h>
#include <sofa/component/interactionforcefield/BoxStiffSpringForceField.h>
#include <sofa/component/forcefield/BuoyantForceField.h>
#include <sofa/component/forcefield/ConicalForceField.h>
#include <sofa/component/forcefield/ConstantForceField.h>
#include <sofa/component/forcefield/DistanceGridForceField.h>
#include <sofa/component/forcefield/EdgePressureForceField.h>
#include <sofa/component/forcefield/EllipsoidForceField.h>
#include <sofa/component/forcefield/FastTetrahedralCorotationalForceField.h>
#include <sofa/component/interactionforcefield/FrameSpringForceField.h>
#include <sofa/component/forcefield/HexahedralFEMForceField.h>
#include <sofa/component/forcefield/HexahedronFEMForceField.h>
#include <sofa/component/interactionforcefield/InteractionEllipsoidForceField.h>
#include <sofa/component/interactionforcefield/JointSpringForceField.h>
#include <sofa/component/forcefield/LennardJonesForceField.h>
#include <sofa/component/forcefield/LinearForceField.h>
#include <sofa/component/forcefield/MJEDTetrahedralForceField.h>
#include <sofa/component/interactionforcefield/MeshSpringForceField.h>
#include <sofa/component/forcefield/OscillatingTorsionPressureForceField.h>
#include <sofa/component/forcefield/ParticlesRepulsionForceField.h>
#include <sofa/component/interactionforcefield/PenalityContactForceField.h>
#include <sofa/component/forcefield/PlaneForceField.h>
#include <sofa/component/interactionforcefield/QuadBendingSprings.h>
#include <sofa/component/forcefield/QuadularBendingSprings.h>
#include <sofa/component/interactionforcefield/RegularGridSpringForceField.h>
#include <sofa/component/interactionforcefield/RepulsiveSpringForceField.h>
#include <sofa/component/forcefield/RestShapeSpringsForceField.h>
#include <sofa/component/forcefield/SPHFluidForceField.h>
#include <sofa/component/forcefield/ShapeMatchingForceField.h>
#include <sofa/component/forcefield/SphereForceField.h>
#include <sofa/component/interactionforcefield/SpringForceField.h>
#include <sofa/component/forcefield/StandardTetrahedralFEMForceField.h>
#include <sofa/component/interactionforcefield/StiffSpringForceField.h>
#include <sofa/component/forcefield/SurfacePressureForceField.h>
#include <sofa/component/forcefield/TetrahedralBiquadraticSpringsForceField.h>
#include <sofa/component/forcefield/TetrahedralCorotationalFEMForceField.h>
#include <sofa/component/forcefield/TetrahedralQuadraticSpringsForceField.h>
#include <sofa/component/forcefield/TetrahedralTensorMassForceField.h>
#include <sofa/component/forcefield/TetrahedralTotalLagrangianForceField.h>
#include <sofa/component/forcefield/TetrahedronFEMForceField.h>
#include <sofa/component/fem/forcefield/Triangle2DFEMForceField.h>
#include <sofa/component/fem/forcefield/TriangleBendingFEMForceField.h>
#include <sofa/component/interactionforcefield/TriangleBendingSprings.h>
#include <sofa/component/forcefield/TriangleFEMForceField.h>
#include <sofa/component/forcefield/TrianglePressureForceField.h>
#include <sofa/component/forcefield/TriangularAnisotropicFEMForceField.h>
#include <sofa/component/forcefield/TriangularBendingSprings.h>
#include <sofa/component/forcefield/TriangularBiquadraticSpringsForceField.h>
#include <sofa/component/forcefield/TriangularFEMForceField.h>
#include <sofa/component/forcefield/TriangularQuadraticSpringsForceField.h>
#include <sofa/component/forcefield/TriangularTensorMassForceField.h>
#include <sofa/component/forcefield/VaccumSphereForceField.h>
#include <sofa/component/interactionforcefield/VectorSpringForceField.h>



//---------------------------------------------------------------------------------------------
//Typedef for AspirationForceField
typedef  sofa::component::forcefield::AspirationForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > AspirationForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for BeamFEMForceField
typedef  sofa::component::forcefield::BeamFEMForceField< sofa::defaulttype::StdRigidTypes<3,double> > BeamFEMForceFieldRigid3d;



//---------------------------------------------------------------------------------------------
//Typedef for BoxStiffSpringForceField
typedef  sofa::component::interactionforcefield::BoxStiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > BoxStiffSpringForceField1d;
typedef  sofa::component::interactionforcefield::BoxStiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > BoxStiffSpringForceField2d;
typedef  sofa::component::interactionforcefield::BoxStiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > BoxStiffSpringForceField3d;
typedef  sofa::component::interactionforcefield::BoxStiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,double>, sofa::defaulttype::Vec<6,double>,double> > BoxStiffSpringForceField6d;



//---------------------------------------------------------------------------------------------
//Typedef for BuoyantForceField
typedef  sofa::component::forcefield::BuoyantForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > BuoyantForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for ConicalForceField
typedef  sofa::component::forcefield::ConicalForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > ConicalForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for ConstantForceField
typedef  sofa::component::forcefield::ConstantForceField< sofa::defaulttype::StdRigidTypes<3,double> > ConstantForceFieldRigid3d;
typedef  sofa::component::forcefield::ConstantForceField< sofa::defaulttype::StdRigidTypes<2,double> > ConstantForceFieldRigid2d;
typedef  sofa::component::forcefield::ConstantForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > ConstantForceField1d;
typedef  sofa::component::forcefield::ConstantForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > ConstantForceField2d;
typedef  sofa::component::forcefield::ConstantForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > ConstantForceField3d;
typedef  sofa::component::forcefield::ConstantForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,double>, sofa::defaulttype::Vec<6,double>,double> > ConstantForceField6d;



//---------------------------------------------------------------------------------------------
//Typedef for DistanceGridForceField
typedef  sofa::component::forcefield::DistanceGridForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > DistanceGridForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for EdgePressureForceField
typedef  sofa::component::forcefield::EdgePressureForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > EdgePressureForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for EllipsoidForceField
typedef  sofa::component::forcefield::EllipsoidForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > EllipsoidForceField1d;
typedef  sofa::component::forcefield::EllipsoidForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > EllipsoidForceField2d;
typedef  sofa::component::forcefield::EllipsoidForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > EllipsoidForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for FastTetrahedralCorotationalForceField
typedef  sofa::component::forcefield::FastTetrahedralCorotationalForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > FastTetrahedralCorotationalForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for FrameSpringForceField
typedef  sofa::component::interactionforcefield::FrameSpringForceField< sofa::defaulttype::StdRigidTypes<3,double> > FrameSpringForceFieldRigid3d;



//---------------------------------------------------------------------------------------------
//Typedef for HexahedralFEMForceField
typedef  sofa::component::forcefield::HexahedralFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > HexahedralFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for HexahedronFEMForceField
typedef  sofa::component::forcefield::HexahedronFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > HexahedronFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for InteractionEllipsoidForceField
typedef  sofa::component::interactionforcefield::InteractionEllipsoidForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double>, sofa::defaulttype::StdRigidTypes<3,double> > InteractionEllipsoidForceField3d_Rigid3d;



//---------------------------------------------------------------------------------------------
//Typedef for JointSpringForceField
typedef  sofa::component::interactionforcefield::JointSpringForceField< sofa::defaulttype::StdRigidTypes<3,double> > JointSpringForceFieldRigid3d;



//---------------------------------------------------------------------------------------------
//Typedef for LennardJonesForceField
typedef  sofa::component::forcefield::LennardJonesForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > LennardJonesForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for LinearForceField
typedef  sofa::component::forcefield::LinearForceField< sofa::defaulttype::StdRigidTypes<3,double> > LinearForceFieldRigid3d;
typedef  sofa::component::forcefield::LinearForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > LinearForceField1d;
typedef  sofa::component::forcefield::LinearForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > LinearForceField2d;
typedef  sofa::component::forcefield::LinearForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > LinearForceField3d;
typedef  sofa::component::forcefield::LinearForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,double>, sofa::defaulttype::Vec<6,double>,double> > LinearForceField6d;



//---------------------------------------------------------------------------------------------
//Typedef for MJEDTetrahedralForceField
typedef  sofa::component::forcefield::MJEDTetrahedralForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > MJEDTetrahedralForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for MeshSpringForceField
typedef  sofa::component::interactionforcefield::MeshSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > MeshSpringForceField1d;
typedef  sofa::component::interactionforcefield::MeshSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > MeshSpringForceField2d;
typedef  sofa::component::interactionforcefield::MeshSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > MeshSpringForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for OscillatingTorsionPressureForceField
typedef  sofa::component::forcefield::OscillatingTorsionPressureForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > OscillatingTorsionPressureForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for ParticlesRepulsionForceField
typedef  sofa::component::forcefield::ParticlesRepulsionForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > ParticlesRepulsionForceField2d;
typedef  sofa::component::forcefield::ParticlesRepulsionForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > ParticlesRepulsionForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for PenalityContactForceField
typedef  sofa::component::interactionforcefield::PenalityContactForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > PenalityContactForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for PlaneForceField
typedef  sofa::component::forcefield::PlaneForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > PlaneForceField1d;
typedef  sofa::component::forcefield::PlaneForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > PlaneForceField2d;
typedef  sofa::component::forcefield::PlaneForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > PlaneForceField3d;
typedef  sofa::component::forcefield::PlaneForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,double>, sofa::defaulttype::Vec<6,double>,double> > PlaneForceField6d;



//---------------------------------------------------------------------------------------------
//Typedef for QuadBendingSprings
typedef  sofa::component::interactionforcefield::QuadBendingSprings< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > QuadBendingSprings2d;
typedef  sofa::component::interactionforcefield::QuadBendingSprings< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > QuadBendingSprings3d;



//---------------------------------------------------------------------------------------------
//Typedef for QuadularBendingSprings
typedef  sofa::component::forcefield::QuadularBendingSprings< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > QuadularBendingSprings3d;



//---------------------------------------------------------------------------------------------
//Typedef for RegularGridSpringForceField
typedef  sofa::component::interactionforcefield::RegularGridSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > RegularGridSpringForceField1d;
typedef  sofa::component::interactionforcefield::RegularGridSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > RegularGridSpringForceField2d;
typedef  sofa::component::interactionforcefield::RegularGridSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > RegularGridSpringForceField3d;
typedef  sofa::component::interactionforcefield::RegularGridSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,double>, sofa::defaulttype::Vec<6,double>,double> > RegularGridSpringForceField6d;



//---------------------------------------------------------------------------------------------
//Typedef for RepulsiveSpringForceField
typedef  sofa::component::interactionforcefield::RepulsiveSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > RepulsiveSpringForceField1d;
typedef  sofa::component::interactionforcefield::RepulsiveSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > RepulsiveSpringForceField2d;
typedef  sofa::component::interactionforcefield::RepulsiveSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > RepulsiveSpringForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for RestShapeSpringsForceField
typedef  sofa::component::forcefield::RestShapeSpringsForceField< sofa::defaulttype::StdRigidTypes<3,double> > RestShapeSpringsForceFieldRigid3d;
typedef  sofa::component::forcefield::RestShapeSpringsForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > RestShapeSpringsForceField1d;
typedef  sofa::component::forcefield::RestShapeSpringsForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > RestShapeSpringsForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for SPHFluidForceField
typedef  sofa::component::forcefield::SPHFluidForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > SPHFluidForceField2d;
typedef  sofa::component::forcefield::SPHFluidForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > SPHFluidForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for ShapeMatchingForceField
typedef  sofa::component::forcefield::ShapeMatchingForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > ShapeMatchingForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for SphereForceField
typedef  sofa::component::forcefield::SphereForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > SphereForceField1d;
typedef  sofa::component::forcefield::SphereForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > SphereForceField2d;
typedef  sofa::component::forcefield::SphereForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > SphereForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for SpringForceField
typedef  sofa::component::interactionforcefield::SpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > SpringForceField1d;
typedef  sofa::component::interactionforcefield::SpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > SpringForceField2d;
typedef  sofa::component::interactionforcefield::SpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > SpringForceField3d;
typedef  sofa::component::interactionforcefield::SpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,double>, sofa::defaulttype::Vec<6,double>,double> > SpringForceField6d;



//---------------------------------------------------------------------------------------------
//Typedef for StandardTetrahedralFEMForceField
typedef  sofa::component::forcefield::StandardTetrahedralFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > StandardTetrahedralFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for StiffSpringForceField
typedef  sofa::component::interactionforcefield::StiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > StiffSpringForceField1d;
typedef  sofa::component::interactionforcefield::StiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > StiffSpringForceField2d;
typedef  sofa::component::interactionforcefield::StiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > StiffSpringForceField3d;
typedef  sofa::component::interactionforcefield::StiffSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,double>, sofa::defaulttype::Vec<6,double>,double> > StiffSpringForceField6d;



//---------------------------------------------------------------------------------------------
//Typedef for SurfacePressureForceField
typedef  sofa::component::forcefield::SurfacePressureForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > SurfacePressureForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedralBiquadraticSpringsForceField
typedef  sofa::component::forcefield::TetrahedralBiquadraticSpringsForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TetrahedralBiquadraticSpringsForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedralCorotationalFEMForceField
typedef  sofa::component::forcefield::TetrahedralCorotationalFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TetrahedralCorotationalFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedralQuadraticSpringsForceField
typedef  sofa::component::forcefield::TetrahedralQuadraticSpringsForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TetrahedralQuadraticSpringsForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedralTensorMassForceField
typedef  sofa::component::forcefield::TetrahedralTensorMassForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TetrahedralTensorMassForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedralTotalLagrangianForceField
typedef  sofa::component::forcefield::TetrahedralTotalLagrangianForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TetrahedralTotalLagrangianForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedronFEMForceField
typedef  sofa::component::forcefield::TetrahedronFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TetrahedronFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for Triangle2DFEMForceField
typedef  sofa::component::fem::forcefield::Triangle2DFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > Triangle2DFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangleBendingFEMForceField
typedef  sofa::component::fem::forcefield::TriangleBendingFEMForceField< sofa::defaulttype::StdRigidTypes<3,double> > TriangleBendingFEMForceFieldRigid3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangleBendingSprings
typedef  sofa::component::interactionforcefield::TriangleBendingSprings< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > TriangleBendingSprings2d;
typedef  sofa::component::interactionforcefield::TriangleBendingSprings< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangleBendingSprings3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangleFEMForceField
typedef  sofa::component::forcefield::TriangleFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangleFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TrianglePressureForceField
typedef  sofa::component::forcefield::TrianglePressureForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TrianglePressureForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularAnisotropicFEMForceField
typedef  sofa::component::forcefield::TriangularAnisotropicFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangularAnisotropicFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularBendingSprings
typedef  sofa::component::forcefield::TriangularBendingSprings< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangularBendingSprings3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularBiquadraticSpringsForceField
typedef  sofa::component::forcefield::TriangularBiquadraticSpringsForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangularBiquadraticSpringsForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularFEMForceField
typedef  sofa::component::forcefield::TriangularFEMForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangularFEMForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularQuadraticSpringsForceField
typedef  sofa::component::forcefield::TriangularQuadraticSpringsForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangularQuadraticSpringsForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularTensorMassForceField
typedef  sofa::component::forcefield::TriangularTensorMassForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > TriangularTensorMassForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for VaccumSphereForceField
typedef  sofa::component::forcefield::VaccumSphereForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,double>, sofa::defaulttype::Vec<1,double>,double> > VaccumSphereForceField1d;
typedef  sofa::component::forcefield::VaccumSphereForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,double>, sofa::defaulttype::Vec<2,double>,double> > VaccumSphereForceField2d;
typedef  sofa::component::forcefield::VaccumSphereForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > VaccumSphereForceField3d;



//---------------------------------------------------------------------------------------------
//Typedef for VectorSpringForceField
typedef  sofa::component::interactionforcefield::VectorSpringForceField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > VectorSpringForceField3d;





#ifndef SOFA_FLOAT
typedef AspirationForceField3d AspirationForceField3;
typedef BeamFEMForceFieldRigid3d BeamFEMForceFieldRigid3;
typedef BoxStiffSpringForceField1d BoxStiffSpringForceField1;
typedef BoxStiffSpringForceField2d BoxStiffSpringForceField2;
typedef BoxStiffSpringForceField3d BoxStiffSpringForceField3;
typedef BoxStiffSpringForceField6d BoxStiffSpringForceField6;
typedef BuoyantForceField3d BuoyantForceField3;
typedef ConicalForceField3d ConicalForceField3;
typedef ConstantForceFieldRigid3d ConstantForceFieldRigid3;
typedef ConstantForceFieldRigid2d ConstantForceFieldRigid2;
typedef ConstantForceField1d ConstantForceField1;
typedef ConstantForceField2d ConstantForceField2;
typedef ConstantForceField3d ConstantForceField3;
typedef ConstantForceField6d ConstantForceField6;
typedef DistanceGridForceField3d DistanceGridForceField3;
typedef EdgePressureForceField3d EdgePressureForceField3;
typedef EllipsoidForceField1d EllipsoidForceField1;
typedef EllipsoidForceField2d EllipsoidForceField2;
typedef EllipsoidForceField3d EllipsoidForceField3;
typedef FastTetrahedralCorotationalForceField3d FastTetrahedralCorotationalForceField3;
typedef FrameSpringForceFieldRigid3d FrameSpringForceFieldRigid3;
typedef HexahedralFEMForceField3d HexahedralFEMForceField3;
typedef HexahedronFEMForceField3d HexahedronFEMForceField3;
typedef InteractionEllipsoidForceField3d_Rigid3d InteractionEllipsoidForceField3_Rigid3;
typedef JointSpringForceFieldRigid3d JointSpringForceFieldRigid3;
typedef LennardJonesForceField3d LennardJonesForceField3;
typedef LinearForceFieldRigid3d LinearForceFieldRigid3;
typedef LinearForceField1d LinearForceField1;
typedef LinearForceField2d LinearForceField2;
typedef LinearForceField3d LinearForceField3;
typedef LinearForceField6d LinearForceField6;
typedef MJEDTetrahedralForceField3d MJEDTetrahedralForceField3;
typedef MeshSpringForceField1d MeshSpringForceField1;
typedef MeshSpringForceField2d MeshSpringForceField2;
typedef MeshSpringForceField3d MeshSpringForceField3;
typedef OscillatingTorsionPressureForceField3d OscillatingTorsionPressureForceField3;
typedef ParticlesRepulsionForceField2d ParticlesRepulsionForceField2;
typedef ParticlesRepulsionForceField3d ParticlesRepulsionForceField3;
typedef PenalityContactForceField3d PenalityContactForceField3;
typedef PlaneForceField1d PlaneForceField1;
typedef PlaneForceField2d PlaneForceField2;
typedef PlaneForceField3d PlaneForceField3;
typedef PlaneForceField6d PlaneForceField6;
typedef QuadBendingSprings2d QuadBendingSprings2;
typedef QuadBendingSprings3d QuadBendingSprings3;
typedef QuadularBendingSprings3d QuadularBendingSprings3;
typedef RegularGridSpringForceField1d RegularGridSpringForceField1;
typedef RegularGridSpringForceField2d RegularGridSpringForceField2;
typedef RegularGridSpringForceField3d RegularGridSpringForceField3;
typedef RegularGridSpringForceField6d RegularGridSpringForceField6;
typedef RepulsiveSpringForceField1d RepulsiveSpringForceField1;
typedef RepulsiveSpringForceField2d RepulsiveSpringForceField2;
typedef RepulsiveSpringForceField3d RepulsiveSpringForceField3;
typedef RestShapeSpringsForceFieldRigid3d RestShapeSpringsForceFieldRigid3;
typedef RestShapeSpringsForceField1d RestShapeSpringsForceField1;
typedef RestShapeSpringsForceField3d RestShapeSpringsForceField3;
typedef SPHFluidForceField2d SPHFluidForceField2;
typedef SPHFluidForceField3d SPHFluidForceField3;
typedef ShapeMatchingForceField3d ShapeMatchingForceField3;
typedef SphereForceField1d SphereForceField1;
typedef SphereForceField2d SphereForceField2;
typedef SphereForceField3d SphereForceField3;
typedef SpringForceField1d SpringForceField1;
typedef SpringForceField2d SpringForceField2;
typedef SpringForceField3d SpringForceField3;
typedef SpringForceField6d SpringForceField6;
typedef StandardTetrahedralFEMForceField3d StandardTetrahedralFEMForceField3;
typedef StiffSpringForceField1d StiffSpringForceField1;
typedef StiffSpringForceField2d StiffSpringForceField2;
typedef StiffSpringForceField3d StiffSpringForceField3;
typedef StiffSpringForceField6d StiffSpringForceField6;
typedef SurfacePressureForceField3d SurfacePressureForceField3;
typedef TetrahedralBiquadraticSpringsForceField3d TetrahedralBiquadraticSpringsForceField3;
typedef TetrahedralCorotationalFEMForceField3d TetrahedralCorotationalFEMForceField3;
typedef TetrahedralQuadraticSpringsForceField3d TetrahedralQuadraticSpringsForceField3;
typedef TetrahedralTensorMassForceField3d TetrahedralTensorMassForceField3;
typedef TetrahedralTotalLagrangianForceField3d TetrahedralTotalLagrangianForceField3;
typedef TetrahedronFEMForceField3d TetrahedronFEMForceField3;
typedef Triangle2DFEMForceField3d Triangle2DFEMForceField3;
typedef TriangleBendingFEMForceFieldRigid3d TriangleBendingFEMForceFieldRigid3;
typedef TriangleBendingSprings2d TriangleBendingSprings2;
typedef TriangleBendingSprings3d TriangleBendingSprings3;
typedef TriangleFEMForceField3d TriangleFEMForceField3;
typedef TrianglePressureForceField3d TrianglePressureForceField3;
typedef TriangularAnisotropicFEMForceField3d TriangularAnisotropicFEMForceField3;
typedef TriangularBendingSprings3d TriangularBendingSprings3;
typedef TriangularBiquadraticSpringsForceField3d TriangularBiquadraticSpringsForceField3;
typedef TriangularFEMForceField3d TriangularFEMForceField3;
typedef TriangularQuadraticSpringsForceField3d TriangularQuadraticSpringsForceField3;
typedef TriangularTensorMassForceField3d TriangularTensorMassForceField3;
typedef VaccumSphereForceField1d VaccumSphereForceField1;
typedef VaccumSphereForceField2d VaccumSphereForceField2;
typedef VaccumSphereForceField3d VaccumSphereForceField3;
typedef VectorSpringForceField3d VectorSpringForceField3;
#endif

#endif

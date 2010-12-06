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


#ifndef SOFA_TYPEDEF_ProjectiveConstraintSet_float_H
#define SOFA_TYPEDEF_ProjectiveConstraintSet_float_H

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


#include <sofa/component/projectiveconstraintset/AttachConstraint.h>
#include <sofa/component/projectiveconstraintset/DisplacementConstraint.h>
#include <sofa/component/projectiveconstraintset/FixedConstraint.h>
#include <sofa/component/projectiveconstraintset/FixedPlaneConstraint.h>
#include <sofa/component/projectiveconstraintset/FixedRotationConstraint.h>
#include <sofa/component/projectiveconstraintset/FixedTranslationConstraint.h>
#include <sofa/component/projectiveconstraintset/HermiteSplineConstraint.h>
#include <sofa/component/projectiveconstraintset/LinearMovementConstraint.h>
#include <sofa/component/projectiveconstraintset/LinearVelocityConstraint.h>
#include <sofa/component/projectiveconstraintset/OscillatorConstraint.h>
#include <sofa/component/projectiveconstraintset/ParabolicConstraint.h>
#include <sofa/component/projectiveconstraintset/PartialFixedConstraint.h>
#include <sofa/component/misc/ParticleSink.h>
#include <sofa/component/misc/ParticleSource.h>
#include <sofa/component/projectiveconstraintset/ProjectionLineConstraint.h>
#include <sofa/component/projectiveconstraintset/ProjectionPlaneConstraint.h>



//---------------------------------------------------------------------------------------------
//Typedef for AttachConstraint
typedef  sofa::component::projectiveconstraintset::AttachConstraint< sofa::defaulttype::StdRigidTypes<2,float> > AttachConstraintRigid2f;
typedef  sofa::component::projectiveconstraintset::AttachConstraint< sofa::defaulttype::StdRigidTypes<3,float> > AttachConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::AttachConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,float>, sofa::defaulttype::Vec<1,float>,float> > AttachConstraint1f;
typedef  sofa::component::projectiveconstraintset::AttachConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > AttachConstraint2f;
typedef  sofa::component::projectiveconstraintset::AttachConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > AttachConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for DisplacementConstraint
typedef  sofa::component::projectiveconstraintset::DisplacementConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,float>, sofa::defaulttype::Vec<1,float>,float> > DisplacementConstraint1f;
typedef  sofa::component::projectiveconstraintset::DisplacementConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > DisplacementConstraint2f;
typedef  sofa::component::projectiveconstraintset::DisplacementConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > DisplacementConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for FixedConstraint
typedef  sofa::component::projectiveconstraintset::FixedConstraint< sofa::defaulttype::StdRigidTypes<2,float> > FixedConstraintRigid2f;
typedef  sofa::component::projectiveconstraintset::FixedConstraint< sofa::defaulttype::StdRigidTypes<3,float> > FixedConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::FixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,float>, sofa::defaulttype::Vec<1,float>,float> > FixedConstraint1f;
typedef  sofa::component::projectiveconstraintset::FixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > FixedConstraint2f;
typedef  sofa::component::projectiveconstraintset::FixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > FixedConstraint3f;
typedef  sofa::component::projectiveconstraintset::FixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,float>, sofa::defaulttype::Vec<6,float>,float> > FixedConstraint6f;



//---------------------------------------------------------------------------------------------
//Typedef for FixedPlaneConstraint
typedef  sofa::component::projectiveconstraintset::FixedPlaneConstraint< sofa::defaulttype::StdRigidTypes<3,float> > FixedPlaneConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::FixedPlaneConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > FixedPlaneConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for FixedRotationConstraint
typedef  sofa::component::projectiveconstraintset::FixedRotationConstraint< sofa::defaulttype::StdRigidTypes<3,float> > FixedRotationConstraintRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for FixedTranslationConstraint
typedef  sofa::component::projectiveconstraintset::FixedTranslationConstraint< sofa::defaulttype::StdRigidTypes<2,float> > FixedTranslationConstraintRigid2f;
typedef  sofa::component::projectiveconstraintset::FixedTranslationConstraint< sofa::defaulttype::StdRigidTypes<3,float> > FixedTranslationConstraintRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for HermiteSplineConstraint
typedef  sofa::component::projectiveconstraintset::HermiteSplineConstraint< sofa::defaulttype::StdRigidTypes<3,float> > HermiteSplineConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::HermiteSplineConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > HermiteSplineConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for LinearMovementConstraint
typedef  sofa::component::projectiveconstraintset::LinearMovementConstraint< sofa::defaulttype::StdRigidTypes<3,float> > LinearMovementConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::LinearMovementConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,float>, sofa::defaulttype::Vec<1,float>,float> > LinearMovementConstraint1f;
typedef  sofa::component::projectiveconstraintset::LinearMovementConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > LinearMovementConstraint2f;
typedef  sofa::component::projectiveconstraintset::LinearMovementConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > LinearMovementConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for LinearVelocityConstraint
typedef  sofa::component::projectiveconstraintset::LinearVelocityConstraint< sofa::defaulttype::StdRigidTypes<3,float> > LinearVelocityConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::LinearVelocityConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,float>, sofa::defaulttype::Vec<1,float>,float> > LinearVelocityConstraint1f;
typedef  sofa::component::projectiveconstraintset::LinearVelocityConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > LinearVelocityConstraint2f;
typedef  sofa::component::projectiveconstraintset::LinearVelocityConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > LinearVelocityConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for OscillatorConstraint
typedef  sofa::component::projectiveconstraintset::OscillatorConstraint< sofa::defaulttype::StdRigidTypes<3,float> > OscillatorConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::OscillatorConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > OscillatorConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for ParabolicConstraint
typedef  sofa::component::projectiveconstraintset::ParabolicConstraint< sofa::defaulttype::StdRigidTypes<3,float> > ParabolicConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::ParabolicConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > ParabolicConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for PartialFixedConstraint
typedef  sofa::component::projectiveconstraintset::PartialFixedConstraint< sofa::defaulttype::StdRigidTypes<2,float> > PartialFixedConstraintRigid2f;
typedef  sofa::component::projectiveconstraintset::PartialFixedConstraint< sofa::defaulttype::StdRigidTypes<3,float> > PartialFixedConstraintRigid3f;
typedef  sofa::component::projectiveconstraintset::PartialFixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,float>, sofa::defaulttype::Vec<1,float>,float> > PartialFixedConstraint1f;
typedef  sofa::component::projectiveconstraintset::PartialFixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > PartialFixedConstraint2f;
typedef  sofa::component::projectiveconstraintset::PartialFixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > PartialFixedConstraint3f;
typedef  sofa::component::projectiveconstraintset::PartialFixedConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<6,float>, sofa::defaulttype::Vec<6,float>,float> > PartialFixedConstraint6f;



//---------------------------------------------------------------------------------------------
//Typedef for ParticleSink
typedef  sofa::component::misc::ParticleSink< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > ParticleSink2f;
typedef  sofa::component::misc::ParticleSink< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > ParticleSink3f;



//---------------------------------------------------------------------------------------------
//Typedef for ParticleSource
typedef  sofa::component::misc::ParticleSource< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > ParticleSource2f;
typedef  sofa::component::misc::ParticleSource< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > ParticleSource3f;



//---------------------------------------------------------------------------------------------
//Typedef for ProjectionLineConstraint
typedef  sofa::component::projectiveconstraintset::ProjectionLineConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<1,float>, sofa::defaulttype::Vec<1,float>,float> > ProjectionLineConstraint1f;
typedef  sofa::component::projectiveconstraintset::ProjectionLineConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<2,float>, sofa::defaulttype::Vec<2,float>,float> > ProjectionLineConstraint2f;
typedef  sofa::component::projectiveconstraintset::ProjectionLineConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > ProjectionLineConstraint3f;



//---------------------------------------------------------------------------------------------
//Typedef for ProjectionPlaneConstraint
typedef  sofa::component::projectiveconstraintset::ProjectionPlaneConstraint< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,float>, sofa::defaulttype::Vec<3,float>,float> > ProjectionPlaneConstraint3f;





#ifdef SOFA_FLOAT
typedef AttachConstraintRigid2f AttachConstraintRigid2;
typedef AttachConstraintRigid3f AttachConstraintRigid3;
typedef AttachConstraint1f AttachConstraint1;
typedef AttachConstraint2f AttachConstraint2;
typedef AttachConstraint3f AttachConstraint3;
typedef DisplacementConstraint1f DisplacementConstraint1;
typedef DisplacementConstraint2f DisplacementConstraint2;
typedef DisplacementConstraint3f DisplacementConstraint3;
typedef FixedConstraintRigid2f FixedConstraintRigid2;
typedef FixedConstraintRigid3f FixedConstraintRigid3;
typedef FixedConstraint1f FixedConstraint1;
typedef FixedConstraint2f FixedConstraint2;
typedef FixedConstraint3f FixedConstraint3;
typedef FixedConstraint6f FixedConstraint6;
typedef FixedPlaneConstraintRigid3f FixedPlaneConstraintRigid3;
typedef FixedPlaneConstraint3f FixedPlaneConstraint3;
typedef FixedRotationConstraintRigid3f FixedRotationConstraintRigid3;
typedef FixedTranslationConstraintRigid2f FixedTranslationConstraintRigid2;
typedef FixedTranslationConstraintRigid3f FixedTranslationConstraintRigid3;
typedef HermiteSplineConstraintRigid3f HermiteSplineConstraintRigid3;
typedef HermiteSplineConstraint3f HermiteSplineConstraint3;
typedef LinearMovementConstraintRigid3f LinearMovementConstraintRigid3;
typedef LinearMovementConstraint1f LinearMovementConstraint1;
typedef LinearMovementConstraint2f LinearMovementConstraint2;
typedef LinearMovementConstraint3f LinearMovementConstraint3;
typedef LinearVelocityConstraintRigid3f LinearVelocityConstraintRigid3;
typedef LinearVelocityConstraint1f LinearVelocityConstraint1;
typedef LinearVelocityConstraint2f LinearVelocityConstraint2;
typedef LinearVelocityConstraint3f LinearVelocityConstraint3;
typedef OscillatorConstraintRigid3f OscillatorConstraintRigid3;
typedef OscillatorConstraint3f OscillatorConstraint3;
typedef ParabolicConstraintRigid3f ParabolicConstraintRigid3;
typedef ParabolicConstraint3f ParabolicConstraint3;
typedef PartialFixedConstraintRigid2f PartialFixedConstraintRigid2;
typedef PartialFixedConstraintRigid3f PartialFixedConstraintRigid3;
typedef PartialFixedConstraint1f PartialFixedConstraint1;
typedef PartialFixedConstraint2f PartialFixedConstraint2;
typedef PartialFixedConstraint3f PartialFixedConstraint3;
typedef PartialFixedConstraint6f PartialFixedConstraint6;
typedef ParticleSink2f ParticleSink2;
typedef ParticleSink3f ParticleSink3;
typedef ParticleSource2f ParticleSource2;
typedef ParticleSource3f ParticleSource3;
typedef ProjectionLineConstraint1f ProjectionLineConstraint1;
typedef ProjectionLineConstraint2f ProjectionLineConstraint2;
typedef ProjectionLineConstraint3f ProjectionLineConstraint3;
typedef ProjectionPlaneConstraint3f ProjectionPlaneConstraint3;
#endif

#endif

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


#ifndef SOFA_TYPEDEF_LinearSolver_double_H
#define SOFA_TYPEDEF_LinearSolver_double_H

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


#include <sofa/component/linearsolver/PrecomputedWarpPreconditioner.h>
#include <sofa/component/linearsolver/WarpPreconditioner.h>



//---------------------------------------------------------------------------------------------
//Typedef for PrecomputedWarpPreconditioner
typedef  sofa::component::linearsolver::PrecomputedWarpPreconditioner< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double>, sofa::component::linearsolver::SparseMatrix<double>, sofa::component::linearsolver::FullVector<double> > PrecomputedWarpPreconditioner3d;



//---------------------------------------------------------------------------------------------
//Typedef for WarpPreconditioner
typedef  sofa::component::linearsolver::WarpPreconditioner< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > WarpPreconditioner3d;





#ifndef SOFA_FLOAT
typedef PrecomputedWarpPreconditioner3d PrecomputedWarpPreconditioner3;
typedef WarpPreconditioner3d WarpPreconditioner3;
#endif

#endif

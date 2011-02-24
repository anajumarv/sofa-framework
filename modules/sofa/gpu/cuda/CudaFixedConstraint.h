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
#ifndef SOFA_GPU_CUDA_CUDAFIXEDCONSTRAINT_H
#define SOFA_GPU_CUDA_CUDAFIXEDCONSTRAINT_H

#include "CudaTypes.h"
#include <sofa/component/projectiveconstraintset/FixedConstraint.h>

namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{

template<class TCoord, class TDeriv, class TReal>
class FixedConstraintInternalData< gpu::cuda::CudaVectorTypes<TCoord,TDeriv,TReal> >
{
public:
    typedef FixedConstraintInternalData< gpu::cuda::CudaVectorTypes<TCoord,TDeriv,TReal> > Data;
    typedef gpu::cuda::CudaVectorTypes<TCoord,TDeriv,TReal> DataTypes;
    typedef FixedConstraint<DataTypes> Main;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::Real Real;
    typedef typename Main::SetIndex SetIndex;

    // min/max fixed indices for contiguous constraints
    int minIndex;
    int maxIndex;
    // vector of indices for general case
    gpu::cuda::CudaVector<int> cudaIndices;

    static void init(Main* m);

    static void addConstraint(Main* m, unsigned int index);

    static void removeConstraint(Main* m, unsigned int index);

    static void projectResponse(Main* m, VecDeriv& dx);
};


// I know using macros is bad design but this is the only way not to repeat the code for all CUDA types
#define CudaFixedConstraint_DeclMethods(T) \
    template<> void FixedConstraint< T >::init(); \
    template<> void FixedConstraint< T >::addConstraint(unsigned int index); \
    template<> void FixedConstraint< T >::removeConstraint(unsigned int index); \
    template<> void FixedConstraint< T >::projectResponse(DataVecDeriv& resData, const core::MechanicalParams* mparams);

CudaFixedConstraint_DeclMethods(gpu::cuda::CudaVec3fTypes);
CudaFixedConstraint_DeclMethods(gpu::cuda::CudaVec3f1Types);

#ifdef SOFA_GPU_CUDA_DOUBLE

CudaFixedConstraint_DeclMethods(gpu::cuda::CudaVec3dTypes);
CudaFixedConstraint_DeclMethods(gpu::cuda::CudaVec3d1Types);

#endif // SOFA_GPU_CUDA_DOUBLE

#undef CudaFixedConstraint_DeclMethods

} // namespace projectiveconstraintset

} // namespace component

} // namespace sofa

#endif

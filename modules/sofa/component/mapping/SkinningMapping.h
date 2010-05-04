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
#ifndef SOFA_COMPONENT_MAPPING_SKINNINGMAPPING_H
#define SOFA_COMPONENT_MAPPING_SKINNINGMAPPING_H

#include <sofa/core/componentmodel/behavior/MechanicalMapping.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/componentmodel/behavior/MappedModel.h>

#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/VecTypes.h>

#include <vector>

#include <sofa/component/component.h>


namespace sofa
{

namespace component
{

namespace mapping
{


using sofa::helper::vector;
using sofa::helper::Quater;
using sofa::component::topology::Coefs;

#define DISTANCE_EUCLIDIAN 0
#define DISTANCE_GEODESIC 1
#define DISTANCE_HARMONIC 2

#define WEIGHT_LINEAR 0
#define WEIGHT_INVDIST_SQUARE 1
#define WEIGHT_HERMITE 2

#define INTERPOLATION_LINEAR 0
#define INTERPOLATION_DUAL_QUATERNION 1
      /*
      typedef enum
      {
        DISTANCE_EUCLIDIAN, DISTANCE_GEODESIC, DISTANCE_HARMONIC
      } DistanceType;

      typedef enum
      {
        WEIGHT_LINEAR, WEIGHT_INVDIST_SQUARE, WEIGHT_HERMITE
      } WeightingType;

      typedef enum
      {
        INTERPOLATION_LINEAR, INTERPOLATION_DUAL_QUATERNION
      } InterpolationType;*/



      template <class BasicMapping>
      class SkinningMapping : public BasicMapping
        {
        public:
          SOFA_CLASS ( SOFA_TEMPLATE ( SkinningMapping,BasicMapping ), BasicMapping );
          typedef BasicMapping Inherit;
          typedef typename Inherit::In In;
          typedef typename Inherit::Out Out;
          typedef typename Out::VecCoord VecCoord;
          typedef typename Out::VecDeriv VecDeriv;
          typedef typename Out::Coord Coord;
          typedef typename Out::Deriv Deriv;
          typedef typename defaulttype::SparseConstraint<Deriv> OutSparseConstraint;
          typedef typename OutSparseConstraint::const_data_iterator OutConstraintIterator;

          typedef typename In::Coord InCoord;
          typedef typename In::Deriv InDeriv;
          typedef typename In::VecCoord VecInCoord;
          typedef typename InCoord::value_type InReal;
          typedef typename Coord::value_type Real;
          enum { N=Coord::static_size };
          typedef defaulttype::Mat<N,N,Real> Mat;
          //typedef defaulttype::Mat<3,1,Real> Mat31;
          typedef defaulttype::Mat<3,3,Real> Mat33;
          typedef defaulttype::Mat<3,6,Real> Mat36;
          typedef vector<Mat36> VMat36;
          typedef vector<VMat36> VVMat36;
          typedef defaulttype::Mat<3,8,Real> Mat38;
          typedef defaulttype::Mat<4,4,Real> Mat44;
          //typedef defaulttype::Mat<6,1,Real> Mat61;
          typedef defaulttype::Mat<6,3,Real> Mat63;
          typedef defaulttype::Mat<6,6,Real> Mat66;
          typedef vector<Mat66> VMat66;
          typedef vector<VMat66> VVMat66;
          //typedef defaulttype::Mat<8,1,Real> Mat81;
          typedef defaulttype::Mat<8,3,Real> Mat83;
          typedef defaulttype::Mat<8,6,Real> Mat86;
          typedef vector<Mat86> VMat86;
          typedef defaulttype::Mat<8,8,Real> Mat88;
          typedef vector<Mat88> VMat88;
          typedef defaulttype::Vec<3,Real> Vec3;
          typedef vector<Vec3> VVec3;
          typedef vector<VVec3> VVVec3;
          typedef defaulttype::Vec<4,Real> Vec4;
          typedef defaulttype::Vec<6,Real> Vec6;
          typedef vector<Vec6> VVec6;
          typedef vector<VVec6> VVVec6;
          typedef defaulttype::Vec<8,Real> Vec8;
					typedef Quater<InReal> Quat;
					typedef sofa::helper::vector< VecCoord > VecVecCoord;
					typedef vector<double> VD;
          typedef Coefs<double> VVD;

        protected:
          vector<Coord> initPos; // pos: point coord in the world reference frame
          vector<Coord> rotatedPoints;

          core::componentmodel::behavior::BaseMechanicalState::ParticleMask* maskFrom;
          core::componentmodel::behavior::BaseMechanicalState::ParticleMask* maskTo;

          Data<vector<int> > repartition;
          Data<Coefs<double> > coefs;
          Data<unsigned int> nbRefs;
					public:
          Data<bool> showBlendedFrame;
          Data<bool> computeJ;
          Data<bool> computeAllMatrices;
          Data<bool> showDefTensors;
					Data<unsigned int> displayedFromIndex;
					Data<double> showTextScaleFactor;

					protected:
          Data<int /* = WeightingType*/> wheightingType;
          Data<int /* = InterpolationType*/> interpolationType;
          Data<int /* = DistanceType*/> distanceType;
          bool computeWeights;
          Coefs<double> distances;
          vector<vector<Coord> > distGradients;

          inline void computeInitPos();
          inline void computeDistances();
          inline void sortReferences();

        public:
          SkinningMapping ( In* from, Out* to );
          virtual ~SkinningMapping();

          void init();

          void apply ( typename Out::VecCoord& out, const typename In::VecCoord& in );
          void applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in );
          void applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in );
          void applyJT ( typename In::VecConst& out, const typename Out::VecConst& in );

          void draw();
          void clear();

          // Weights
          void setWeightsToHermite();
          void setWieghtsToInvDist();
          void setWeightsToLinear();
          inline void updateWeights();

          // Interpolations
          void setInterpolationToLinear();
          void setInterpolationToDualQuaternion();

          // Accessors
          void setNbRefs ( unsigned int nb )
          {
            nbRefs.setValue ( nb );
          }
          void setWeightCoefs ( vector<vector<double> > &weights );
          void setRepartition ( vector<int> &rep );
          void setComputeWeights ( bool val )
          {
            computeWeights=val;
          }
          unsigned int getNbRefs()
          {
            return nbRefs.getValue();
          }
          const vector<vector<double> >& getWeightCoefs()
          {
            return coefs.getValue();
          }
          const vector<int>& getRepartition()
          {
            return repartition.getValue();
          }
          bool getComputeWeights()
          {
            return computeWeights;
          }

        };

      using core::Mapping;
      using core::componentmodel::behavior::MechanicalMapping;
      using core::componentmodel::behavior::MappedModel;
      using core::componentmodel::behavior::State;
      using core::componentmodel::behavior::MechanicalState;

      using sofa::defaulttype::Vec2dTypes;
      using sofa::defaulttype::Vec3dTypes;
      using sofa::defaulttype::Vec2fTypes;
      using sofa::defaulttype::Vec3fTypes;
      using sofa::defaulttype::ExtVec2fTypes;
      using sofa::defaulttype::ExtVec3fTypes;
      using sofa::defaulttype::Rigid2dTypes;
      using sofa::defaulttype::Rigid3dTypes;
      using sofa::defaulttype::Rigid2fTypes;
      using sofa::defaulttype::Rigid3fTypes;

#if defined(WIN32) && !defined(SOFA_COMPONENT_MAPPING_SKINNINGMAPPING_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< MechanicalMapping< MechanicalState<Rigid3dTypes>, MechanicalState<Vec3dTypes> > >;
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3dTypes>, MappedModel<Vec3dTypes> > >;
// template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3dTypes>, MappedModel<ExtVec3dTypes> > >;
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3dTypes>, MappedModel<ExtVec3fTypes> > >;
#endif
#ifndef SOFA_DOUBLE
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< MechanicalMapping< MechanicalState<Rigid3fTypes>, MechanicalState<Vec3fTypes> > >;
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3fTypes>, MappedModel<Vec3fTypes> > >;
// template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3fTypes>, MappedModel<ExtVec3dTypes> > >;
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3fTypes>, MappedModel<ExtVec3fTypes> > >;
#endif
#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< MechanicalMapping< MechanicalState<Rigid3dTypes>, MechanicalState<Vec3fTypes> > >;
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< MechanicalMapping< MechanicalState<Rigid3fTypes>, MechanicalState<Vec3dTypes> > >;
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3dTypes>, MappedModel<Vec3fTypes> > >;
      extern template class SOFA_COMPONENT_MAPPING_API SkinningMapping< Mapping< State<Rigid3fTypes>, MappedModel<Vec3dTypes> > >;
#endif
#endif
#endif




    } // namespace mapping

  } // namespace component

} // namespace sofa

#endif

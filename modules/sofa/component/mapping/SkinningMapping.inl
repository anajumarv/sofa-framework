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
#ifndef SOFA_COMPONENT_MAPPING_SKINNINGMAPPING_INL
#define SOFA_COMPONENT_MAPPING_SKINNINGMAPPING_INL

#include <sofa/component/mapping/SkinningMapping.h>
#include <sofa/helper/io/Mesh.h>
#include <sofa/helper/gl/template.h>
#include <sofa/helper/gl/Axis.h>
#include <sofa/core/componentmodel/behavior/MechanicalMapping.inl>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <string>
#include <iostream>
#include <sofa/component/topology/TriangleSetTopologyContainer.h>
#include <sofa/helper/gl/glText.inl>



namespace sofa
  {

  namespace component
    {

    namespace mapping
      {

      using namespace sofa::defaulttype;
			using sofa::component::topology::TriangleSetTopologyContainer;


      template <class BasicMapping>
      SkinningMapping<BasicMapping>::SkinningMapping ( In* from, Out* to )
          : Inherit ( from, to )
          , repartition ( initData ( &repartition,"repartition","repartition between input DOFs and skinned vertices" ) )
          , coefs ( initData ( &coefs,"coefs","weights list for the influences of the references Dofs" ) )
          , nbRefs ( initData ( &nbRefs, ( unsigned ) 3,"nbRefs","nb references for skinning" ) )
          , showBlendedFrame ( initData ( &showBlendedFrame, false, "showBlendedFrame","weights list for the influences of the references Dofs" ) )
          , computeJ ( initData ( &computeJ, false, "computeJ", "compute matrix J in addition to apply for the dual quat interpolation method." ) )
          , computeAllMatrices ( initData ( &computeAllMatrices, false, "computeAllMatrices","compute all the matrices in addition to apply for the dual quat interpolation method." ) )
          , showDefTensors ( initData ( &showDefTensors, false, "showDefTensors","show computed deformation tensors." ) )
          , displayedFromIndex ( initData ( &displayedFromIndex, ( unsigned ) 0, "displayedFromIndex","Displayed From Index." ) )
          , showTextScaleFactor ( initData ( &showTextScaleFactor, 0.00005, "showTextScaleFactor","Text Scale Factor." ) )
          , wheightingType ( initData ( &wheightingType, WEIGHT_INVDIST_SQUARE, "wheightingType","Weighting computation method." ) )
          , interpolationType ( initData ( &interpolationType, INTERPOLATION_LINEAR, "interpolationType","Interpolation method." ) )
          , distanceType ( initData ( &distanceType, DISTANCE_HARMONIC, "distanceType","Distance computation method." ) )
          , computeWeights ( true )
      {
        maskFrom = NULL;
        if ( core::componentmodel::behavior::BaseMechanicalState *stateFrom = dynamic_cast< core::componentmodel::behavior::BaseMechanicalState *> ( from ) )
          maskFrom = &stateFrom->forceMask;
        maskTo = NULL;
        if ( core::componentmodel::behavior::BaseMechanicalState *stateTo = dynamic_cast< core::componentmodel::behavior::BaseMechanicalState *> ( to ) )
          maskTo = &stateTo->forceMask;
      }

      template <class BasicMapping>
      SkinningMapping<BasicMapping>::~SkinningMapping ()
      {
      }


      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::computeInitPos ( )
      {
        const VecCoord& xto = ( this->toModel->getX0() == NULL)?*this->toModel->getX():*this->toModel->getX0();
        const VecInCoord& xfrom = *this->fromModel->getX0();

        const vector<int>& m_reps = repartition.getValue();

        switch ( interpolationType.getValue() )
          {
          case INTERPOLATION_LINEAR:
          {
            initPos.resize ( xto.size() * nbRefs.getValue() );
            for ( unsigned int i = 0; i < xto.size(); i++ )
              for ( unsigned int m = 0; m < nbRefs.getValue(); m++ )
                {
                  initPos[nbRefs.getValue() *i+m] = xfrom[m_reps[nbRefs.getValue() *i+m]].getOrientation().inverseRotate ( xto[i] - xfrom[m_reps[nbRefs.getValue() *i+m]].getCenter() );
                }
            break;
          }
          default:
          {}
          }
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::computeDistances ()
      {
        Coord posTo;
        VecCoord& xto = ( this->toModel->getX0() == NULL)?*this->toModel->getX():*this->toModel->getX0();
        VecInCoord& xfrom = *this->fromModel->getX0();

        distances.clear();
        distGradients.clear();

        switch ( distanceType.getValue() )
          {
          case DISTANCE_EUCLIDIAN:
          {
            distances.resize ( xfrom.size() );
            distGradients.resize ( xfrom.size() );
            for ( unsigned int i=0;i<xfrom.size();i++ )
              {
                distances[i].resize ( xto.size() );
                distGradients[i].resize ( xto.size() );
                for ( unsigned int j=0;j<xto.size();j++ )
                  {
                    distGradients[i][j] = xto[j] - xfrom[i].getCenter();
                    distances[i][j] = distGradients[i][j].norm();
                    distGradients[i][j].normalize();
                  }
              }
            break;
          }
          case DISTANCE_GEODESIC:
          case DISTANCE_HARMONIC:
          {
            break;
          }
          default:
          {}
          }
      }


      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::sortReferences()
      {
        VecCoord& xto = ( this->toModel->getX0() == NULL)?*this->toModel->getX():*this->toModel->getX0();
        VecInCoord& xfrom = *this->fromModel->getX0();
        const unsigned int& nbRef = nbRefs.getValue();

        vector<int>& m_reps = * ( repartition.beginEdit() );
        m_reps.clear();
        m_reps.resize ( nbRefs.getValue() *xto.size() );
        for ( unsigned int i=0;i<nbRefs.getValue() *xto.size();i++ )
          m_reps[i] = -1;

				if( interpolationType.getValue() == INTERPOLATION_LINEAR)
				{
					for ( unsigned int i=0;i<xfrom.size();i++ )
						for ( unsigned int j=0;j<xto.size();j++ )
							for ( unsigned int k=0; k<nbRef; k++ )
								{
									const int idxReps=m_reps[nbRef*j+k];
									if ( ( idxReps == -1 ) || ( distances[i][j] < distances[idxReps][j] ) )
										{
											for ( unsigned int m=nbRef-1 ; m>k ; m-- )
												m_reps[nbRef *j+m] = m_reps[nbRef *j+m-1];
											m_reps[nbRef *j+k] = i;
											break;
										}
								}
				}
				else if( INTERPOLATION_DUAL_QUATERNION)
				{
					for ( unsigned int i=0;i<xfrom.size();i++ )
						for ( unsigned int j=0;j<xto.size();j++ )
							m_reps[nbRef *j+i] = i;
				}
      }


      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::init()
      {
        distanceType.setValue( DISTANCE_EUCLIDIAN);
        VecInCoord& xfrom = *this->fromModel->getX0();
        if ( this->initPos.empty() && this->toModel!=NULL && computeWeights==true && coefs.getValue().size() ==0 )
          {
            if ( wheightingType.getValue() == WEIGHT_LINEAR || wheightingType.getValue() == WEIGHT_HERMITE )
                nbRefs.setValue ( 2 );

            if( xfrom.size() < nbRefs.getValue() || interpolationType.getValue() == INTERPOLATION_DUAL_QUATERNION)
                nbRefs.setValue ( xfrom.size() );

            computeDistances();
            sortReferences ();
            updateWeights ();
            computeInitPos ();

          }
        else if ( computeWeights == false || coefs.getValue().size() !=0 )
          {
            computeInitPos();
          }

        this->BasicMapping::init();
			}

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::clear()
      {
        this->initPos.clear();
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::setWeightsToHermite()
      {
        wheightingType.setValue( WEIGHT_HERMITE);
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::setWeightsToLinear()
      {
        wheightingType.setValue( WEIGHT_LINEAR);
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::setWieghtsToInvDist()
      {
        wheightingType.setValue( WEIGHT_INVDIST_SQUARE);
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::setInterpolationToLinear()
      {
        interpolationType.setValue( INTERPOLATION_LINEAR);
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::setInterpolationToDualQuaternion()
      {
        interpolationType.setValue( INTERPOLATION_DUAL_QUATERNION);
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::updateWeights ()
      {
        VecCoord& xto = ( this->toModel->getX0() == NULL)?*this->toModel->getX():*this->toModel->getX0();
        VecInCoord& xfrom = *this->fromModel->getX0();

        vector<vector<double> >& m_coefs = * ( coefs.beginEdit() );
        const vector<int>& m_reps = repartition.getValue();

        m_coefs.resize ( xfrom.size() );
        for ( unsigned int i=0;i<xfrom.size();i++ )
          m_coefs[i].resize ( xto.size() );

        switch ( wheightingType.getValue() )
          {
          case WEIGHT_LINEAR:
          {
            for ( unsigned int i=0;i<xto.size();i++ )
              {
                Vec3d r1r2, r1p;
                r1r2 = xfrom[m_reps[nbRefs.getValue() *i+1]].getCenter() - xfrom[m_reps[nbRefs.getValue() *i+0]].getCenter();
                r1p  = xto[i] - xfrom[m_reps[nbRefs.getValue() *i+0]].getCenter();
                double r1r2NormSquare = r1r2.norm()*r1r2.norm(); 
                double wi = ( r1r2*r1p ) / ( r1r2NormSquare);

                // Abscisse curviligne
                m_coefs[m_reps[nbRefs.getValue() *i+0]][i] = ( 1 - wi );
                m_coefs[m_reps[nbRefs.getValue() *i+1]][i] = wi;
                distGradients[m_reps[nbRefs.getValue() *i+0]][i] = -r1r2 / r1r2NormSquare;
                distGradients[m_reps[nbRefs.getValue() *i+1]][i] = r1r2 / r1r2NormSquare;
              }
            break;
          }
          case WEIGHT_INVDIST_SQUARE:
          {
            for ( unsigned int j=0;j<xto.size();j++ )
            {
              for ( unsigned int i=0;i<nbRefs.getValue();i++ )
              {
                int indexFrom = m_reps[nbRefs.getValue() *j + i];
                m_coefs[indexFrom][j] = 1 / (distances[indexFrom][j]*distances[indexFrom][j]);
                if( distanceType.getValue() == DISTANCE_EUCLIDIAN)
                  distGradients[indexFrom][j] = - distGradients[indexFrom][j] / (double)(distances[indexFrom][j]*distances[indexFrom][j]*distances[indexFrom][j]) * 2.0;
              }

              /* TODO normalize later. Store the weights to be able to recompute the coeffs as the normalized weights (for each frame) even when we insert a new frame
              //normalize the coefs vector such as the sum is equal to 1
              double norm=0.0;
              for ( unsigned int i=0;i<xfrom.size();i++ )
                norm += m_coefs[i][j];
              norm = helper::rsqrt ( norm );

              for ( unsigned int i=0;i<xfrom.size();i++ )
                m_coefs[i][j] /= norm;
              //*/
            }

            break;
          }
          case WEIGHT_HERMITE:
          {
            for ( unsigned int i=0;i<xto.size();i++ )
              {
                Vec3d r1r2, r1p;
                double wi;
                r1r2 = xfrom[m_reps[nbRefs.getValue() *i+1]].getCenter() - xfrom[m_reps[nbRefs.getValue() *i+0]].getCenter();
                r1p  = xto[i] - xfrom[m_reps[nbRefs.getValue() *i+0]].getCenter();
                wi = ( r1r2*r1p ) / ( r1r2.norm() *r1r2.norm() );

                // Fonctions d'Hermite
                m_coefs[m_reps[nbRefs.getValue() *i+0]][i] = 1-3*wi*wi+2*wi*wi*wi;
                m_coefs[m_reps[nbRefs.getValue() *i+1]][i] = 3*wi*wi-2*wi*wi*wi;

                r1r2.normalize();
                distGradients[m_reps[nbRefs.getValue() *i+0]][i] = -r1r2;
                distGradients[m_reps[nbRefs.getValue() *i+1]][i] = r1r2;
              }
            break;
          }
          default:
          {}
          }
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::setWeightCoefs ( vector<vector<double> > &weights )
      {
        vector<vector<double> > * m_coefs = coefs.beginEdit();
        m_coefs->clear();
        m_coefs->insert ( m_coefs->begin(), weights.begin(), weights.end() );
        coefs.endEdit();
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::setRepartition ( vector<int> &rep )
      {
        vector<int> * m_reps = repartition.beginEdit();
        m_reps->clear();
        m_reps->insert ( m_reps->begin(), rep.begin(), rep.end() );;
        repartition.endEdit();
      }


      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
      {
        const vector<int>& m_reps = repartition.getValue();
        const vector<vector<double> >& m_coefs = coefs.getValue();

        switch ( interpolationType.getValue() )
          {
          case INTERPOLATION_LINEAR:
          {
            rotatedPoints.resize ( initPos.size() );
            out.resize ( initPos.size() / nbRefs.getValue() );
            for ( unsigned int i=0 ; i<out.size(); i++ )
              {
                out[i] = Coord();
                for ( unsigned int m=0 ; m<nbRefs.getValue(); m++ )
                  {

                    const int& idx=nbRefs.getValue() *i+m;
                    const int& idxReps=m_reps[idx];

                    // Save rotated points for applyJ/JT
                    rotatedPoints[idx] = in[idxReps].getOrientation().rotate ( initPos[idx] );

                    // And add each reference frames contributions to the new position out[i]
                    out[i] += ( in[idxReps ].getCenter() + rotatedPoints[idx] ) * m_coefs[idxReps][i];
                  }
              }
            break;
          }
          default:
          {}
          }
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
      {
        const vector<int>& m_reps = repartition.getValue();
        const vector<vector<double> >& m_coefs = coefs.getValue();
        VecCoord& xto = *this->toModel->getX();
        out.resize ( xto.size() );
        Deriv v,omega;

        if ( ! ( maskTo->isInUse() ) )
          {
            switch ( interpolationType.getValue() )
              {
              case INTERPOLATION_LINEAR:
              {
                for ( unsigned int i=0;i<out.size();i++ )
                  {
                    out[i] = Deriv();
                    for ( unsigned int m=0 ; m<nbRefs.getValue(); m++ )
                      {
                        const int idx=nbRefs.getValue() *i+m;
                        const int idxReps=m_reps[idx];

                        v = in[idxReps].getVCenter();
                        omega = in[idxReps].getVOrientation();
                        out[i] += ( v - cross ( rotatedPoints[idx],omega ) ) * m_coefs[idxReps][i];
                      }
                  }
                break;
              }
              default:
              {}
              }
          }
        else
          {
            typedef core::componentmodel::behavior::BaseMechanicalState::ParticleMask ParticleMask;
            const ParticleMask::InternalStorage &indices=maskTo->getEntries();

            ParticleMask::InternalStorage::const_iterator it;
            switch ( interpolationType.getValue() )
              {
              case INTERPOLATION_LINEAR:
              {
                for ( it=indices.begin();it!=indices.end();it++ )
                  {
                    const int i= ( int ) ( *it );
                    out[i] = Deriv();
                    for ( unsigned int m=0 ; m<nbRefs.getValue(); m++ )
                      {
                        const int idx=nbRefs.getValue() *i+m;
                        const int idxReps=m_reps[idx];

                        v = in[idxReps].getVCenter();
                        omega = in[idxReps].getVOrientation();
                        out[i] += ( v - cross ( rotatedPoints[idx],omega ) ) * m_coefs[idxReps][i];
                      }
                  }
                break;
              }
              default:
              {}
              }
          }
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
      {
        const vector<int>& m_reps = repartition.getValue();
        const vector<vector<double> >& m_coefs = coefs.getValue();

        Deriv v,omega;
        if ( ! ( maskTo->isInUse() ) )
          {
            switch ( interpolationType.getValue() )
              {
              case INTERPOLATION_LINEAR:
              {
                maskFrom->setInUse ( false );
                for ( unsigned int i=0;i<in.size();i++ )
                  {
                    for ( unsigned int m=0 ; m<nbRefs.getValue(); m++ )
                      {
                        Deriv f = in[i];
                        v = f;
                        const int idx=nbRefs.getValue() *i+m;
                        const int idxReps=m_reps[idx];
                        omega = cross ( rotatedPoints[idx],f );
                        out[idxReps].getVCenter() += v * m_coefs[idxReps][i];
                        out[idxReps].getVOrientation() += omega * m_coefs[idxReps][i];
                      }
                  }
                break;
              }
              default:
              {}
              }
          }
        else
          {
            typedef core::componentmodel::behavior::BaseMechanicalState::ParticleMask ParticleMask;
            const ParticleMask::InternalStorage &indices=maskTo->getEntries();

            ParticleMask::InternalStorage::const_iterator it;
            switch ( interpolationType.getValue() )
              {
              case INTERPOLATION_LINEAR:
              {
                for ( it=indices.begin();it!=indices.end();it++ )
                  {
                    const int i= ( int ) ( *it );
                    for ( unsigned int m=0 ; m<nbRefs.getValue(); m++ )
                      {
                        Deriv f = in[i];
                        v = f;
                        const int idx=nbRefs.getValue() *i+m;
                        const int idxReps=m_reps[idx];
                        omega = cross ( rotatedPoints[idx],f );
                        out[idxReps].getVCenter() += v * m_coefs[idxReps][i];
                        out[idxReps].getVOrientation() += omega * m_coefs[idxReps][i];

                        maskFrom->insertEntry ( idxReps );
                      }
                  }
                break;
              }
              default:
              {}
              }
          }

      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::applyJT ( typename In::VecConst& out, const typename Out::VecConst& in )
      {
        const vector<int>& m_reps = repartition.getValue();
        const vector<vector<double> >& m_coefs = coefs.getValue();
        const unsigned int nbr = nbRefs.getValue();
        const unsigned int nbi = this->fromModel->getX()->size();
        Deriv omega;
        typename In::VecDeriv v;
        vector<bool> flags;
        int outSize = out.size();
        out.resize ( in.size() + outSize ); // we can accumulate in "out" constraints from several mappings
        switch ( interpolationType.getValue() )
          {
          case INTERPOLATION_LINEAR:
          {
            for ( unsigned int i=0;i<in.size();i++ )
              {
                v.clear();
                v.resize ( nbi );
                flags.clear();
                flags.resize ( nbi );
                OutConstraintIterator itOut;
                std::pair< OutConstraintIterator, OutConstraintIterator > iter=in[i].data();

                for ( itOut=iter.first;itOut!=iter.second;itOut++ )
                  {
                    unsigned int indexIn = itOut->first;
                    Deriv data = ( Deriv ) itOut->second;
                    Deriv f = data;
                    for ( unsigned int m=0 ; m<nbr; m++ )
                      {
                        omega = cross ( rotatedPoints[nbr*indexIn+m],f );
                        flags[m_reps[nbr*indexIn+m] ] = true;
                        v[m_reps[nbr*indexIn+m] ].getVCenter() += f * m_coefs[m_reps[nbr*indexIn+m]][i];
                        v[m_reps[nbr*indexIn+m] ].getVOrientation() += omega * m_coefs[m_reps[nbr*indexIn+m]][i];
                      }
                  }
                for ( unsigned int j=0 ; j<nbi; j++ )
                  {
                    //if (!(v[i] == typename In::Deriv()))
                    if ( flags[j] )
                      out[outSize+i].add ( j,v[j] );
                  }
              }
            break;
          }
          default:
          {}
          }
      }

      template <class BasicMapping>
      void SkinningMapping<BasicMapping>::draw()
      {
        const typename Out::VecCoord& xto = *this->toModel->getX();
        const typename In::VecCoord& xfrom = *this->fromModel->getX();
        const vector<int>& m_reps = repartition.getValue();
        const vector<vector<double> >& m_coefs = coefs.getValue();
				const unsigned int nbRef = nbRefs.getValue();

        if ( this->getShow() )
          {

            /*/ Display  m_reps for each points
            for ( unsigned int i=0;i<xto.size();i++ )
                    sofa::helper::gl::GlText::draw ( m_reps[nbRefs.getValue() *i+0], xto[i], showTextScaleFactor.getValue() );
            //*/

            // Display coefs for each points
            glDisable ( GL_LIGHTING );
            glColor3f( 1.0, 1.0, 1.0);
            for ( unsigned int i=0;i<xto.size();i++ )
                    sofa::helper::gl::GlText::draw ( (int)(m_coefs[displayedFromIndex.getValue()%xfrom.size()][i]*10000), xto[i], showTextScaleFactor.getValue() );
            //*/

            // Display mapping links between in and out elements
            if ( interpolationType.getValue() != INTERPOLATION_DUAL_QUATERNION )
              {
                glDisable ( GL_LIGHTING );
                glPointSize ( 1 );
                glColor4f ( 1,1,0,1 );
                glBegin ( GL_LINES );

                for ( unsigned int i=0; i<xto.size(); i++ )
                  {
                    for ( unsigned int m=0 ; m<nbRef; m++ )
                      {
                        const int idxReps=m_reps[nbRef *i+m];
                        double coef = m_coefs[idxReps][i];
                        if ( coef > 0.0 )
                          {
                            glColor4d ( coef,coef,0,1 );
                            helper::gl::glVertexT ( xfrom[m_reps[nbRef *i+m] ].getCenter() );
                            helper::gl::glVertexT ( xto[i] );
                          }
                      }
                  }
                glEnd();
              }
          }


      }


    } // namespace mapping

  } // namespace component

} // namespace sofa

#endif

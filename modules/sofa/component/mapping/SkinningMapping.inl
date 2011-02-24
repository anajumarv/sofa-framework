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

#include <sofa/component/topology/TriangleSetTopologyContainer.h>
#include <sofa/core/Mapping.inl>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/helper/gl/Axis.h>
#include <sofa/helper/gl/Color.h>
#include <sofa/helper/gl/glText.inl>
#include <sofa/helper/gl/template.h>
#include <sofa/helper/io/Mesh.h>


#include <string>
#include <iostream>

namespace sofa
{

namespace component
{

namespace mapping
{

using namespace sofa::defaulttype;
using sofa::component::topology::TriangleSetTopologyContainer;


template <class TIn, class TOut>
SkinningMapping<TIn, TOut>::SkinningMapping (core::State<In>* from, core::State<Out>* to )
        : Inherit ( from, to )
        , repartition ( initData ( &repartition,"repartition","repartition between input DOFs and skinned vertices" ) )
        , weights ( initData ( &weights,"weights","weights list for the influences of the references Dofs" ) )
        , weightGradients ( initData ( &weightGradients,"weightGradients","weight gradients list for the influences of the references Dofs" ) )
        , nbRefs ( initData ( &nbRefs, ( unsigned ) 3,"nbRefs","nb references for skinning" ) )
        , showBlendedFrame ( initData ( &showBlendedFrame, false, "showBlendedFrame","weights list for the influences of the references Dofs" ) )
        , showDefTensors ( initData ( &showDefTensors, false, "showDefTensors","show computed deformation tensors." ) )
        , showDefTensorsValues ( initData ( &showDefTensorsValues, false, "showDefTensorsValues","Show Deformation Tensors Values." ) )
        , showDefTensorScale ( initData ( &showDefTensorScale, 1.0, "showDefTensorScale","deformation tensor scale." ) )
        , showFromIndex ( initData ( &showFromIndex, ( unsigned ) 0, "showFromIndex","Displayed From Index." ) )
        , showDistancesValues ( initData ( &showDistancesValues, false, "showDistancesValues","Show dstances values." ) )
        , showWeights ( initData ( &showWeights, false, "showWeights","Show coeficients." ) )
        , showGammaCorrection ( initData ( &showGammaCorrection, 1.0, "showGammaCorrection","Correction of the Gamma by a power" ) )
        , showWeightsValues ( initData ( &showWeightsValues, false, "showWeightsValues","Show coeficients values." ) )
        , showReps ( initData ( &showReps, false, "showReps","Show repartition." ) )
        , showValuesNbDecimals ( initData ( &showValuesNbDecimals, 0, "showValuesNbDecimals","Multiply floating point by 10^n." ) )
        , showTextScaleFactor ( initData ( &showTextScaleFactor, 0.00005, "showTextScaleFactor","Text Scale Factor." ) )
        , showGradients ( initData ( &showGradients, false, "showGradients","Show gradients." ) )
        , showGradientsValues ( initData ( &showGradientsValues, false, "showGradientsValues","Show Gradients Values." ) )
        , showGradientsScaleFactor ( initData ( &showGradientsScaleFactor, 0.0001, "showGradientsScaleFactor","Gradients Scale Factor." ) )
        , wheightingType ( initData ( &wheightingType, "wheightingType","Weighting computation method.\n0 - none (distance is used).\n1 - inverse distance square.\n2 - linear.\n3 - hermite (on groups of four dofs).\n4 - spline (on groups of four dofs)." ) )
        , distanceType ( initData ( &distanceType, "distanceType","Distance computation method.\n0 - euclidian distance.\n1 - geodesic distance.\n2 - harmonic diffusion." ) )
        , computeWeights ( true )
{


    maskFrom = NULL;
    if ( core::behavior::BaseMechanicalState *stateFrom = dynamic_cast< core::behavior::BaseMechanicalState *> ( from ) )
        maskFrom = &stateFrom->forceMask;
    maskTo = NULL;
    if ( core::behavior::BaseMechanicalState *stateTo = dynamic_cast< core::behavior::BaseMechanicalState *> ( to ) )
        maskTo = &stateTo->forceMask;


    sofa::helper::OptionsGroup wheightingTypeOptions(5,"None","InvDistSquare","Linear", "Hermite", "Spline");
    wheightingTypeOptions.setSelectedItem(WEIGHT_INVDIST_SQUARE);
    wheightingType.setValue(wheightingTypeOptions);

    sofa::helper::OptionsGroup* distanceTypeOptions = distanceType.beginEdit();
    distanceTypeOptions->setNames(5,"Euclidian","Geodesic", "Harmonic", "StiffnessDiffusion", "HarmonicWithStiffness");
    distanceTypeOptions->setSelectedItem(0);
    distanceType.endEdit();
}

template <class TIn, class TOut>
SkinningMapping<TIn, TOut>::~SkinningMapping ()
{
}


template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::computeInitPos ( )
{
    const VecCoord& xto = ( this->toModel->getX0()->size() == 0)?*this->toModel->getX():*this->toModel->getX0();
    const VecInCoord& xfrom = *this->fromModel->getX0();

    const vector<int>& m_reps = repartition.getValue();

        initPos.resize ( xto.size() * nbRefs.getValue() );
        for ( unsigned int i = 0; i < xto.size(); i++ )
            for ( unsigned int m = 0; m < nbRefs.getValue(); m++ )
            {
                const int& idx=nbRefs.getValue() *i+m;
                const int& idxReps=m_reps[idx];
                getLocalCoord( initPos[idx], xfrom[idxReps], xto[i]);
            }
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::computeDistances ()
{

    this->getDistances( 0);
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::getDistances( int xfromBegin)
{
    const VecCoord& xto0 = ( this->toModel->getX0()->size() == 0)?*this->toModel->getX():*this->toModel->getX0();
    const VecInCoord& xfrom0 = *this->fromModel->getX0();

    switch ( distanceType.getValue().getSelectedId() )
    {
    case DISTANCE_EUCLIDIAN:
    {
        distances.resize( xfrom0.size());
        distGradients.resize( xfrom0.size());
        for ( unsigned int i = xfromBegin; i < xfrom0.size(); ++i ) // for each new frame
        {
            distances[i].resize ( xto0.size() );
            distGradients[i].resize ( xto0.size() );
            for ( unsigned int j=0;j<xto0.size();++j )
            {
                distGradients[i][j] = xto0[j] - xfrom0[i].getCenter();
                distances[i][j] = distGradients[i][j].norm();
                distGradients[i][j].normalize();
            }
        }
        break;
    }
    default: {}
    }
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::sortReferences( vector<int>& references)
{
    const VecCoord& xto = ( this->toModel->getX0()->size() == 0)?*this->toModel->getX():*this->toModel->getX0();
    const VecInCoord& xfrom = *this->fromModel->getX0();
    const unsigned int& nbRef = nbRefs.getValue();

    references.clear();
    references.resize ( nbRefs.getValue() *xto.size() );
    for ( unsigned int i=0;i<nbRefs.getValue() *xto.size();i++ )
        references[i] = -1;

    for ( unsigned int i=0;i<xfrom.size();i++ )
        for ( unsigned int j=0;j<xto.size();j++ )
            for ( unsigned int k=0; k<nbRef; k++ )
            {
                const int idxReps=references[nbRef*j+k];
                if ( ( idxReps == -1 ) || ( distances[i][j] < distances[idxReps][j] ) )
                {
                    for ( unsigned int m=nbRef-1 ; m>k ; m-- )
                        references[nbRef *j+m] = references[nbRef *j+m-1];
                    references[nbRef *j+k] = i;
                    break;
                }
            }
}


template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::normalizeWeights()
{
    const unsigned int xtoSize = this->toModel->getX()->size();
    const unsigned int& nbRef = nbRefs.getValue();
    VVD& m_weights = * ( weights.beginEdit() );
    SVector<SVector<GeoCoord> >& m_dweight = * ( weightGradients.beginEdit());

    // Normalise weights & dweights
    for (unsigned int j = 0; j < xtoSize; ++j)
    {
      double sumWeights = 0;
      Vec3 sumGrad;

      // Compute norm
      for (unsigned int i = 0; i < nbRef; ++i)
      {
        sumWeights += m_weights[i][j];
        sumGrad += m_dweight[i][j];
      }
      
      // Normalise
      for (unsigned int i = 0; i < nbRef; ++i)
      {
        m_weights[i][j] /= sumWeights;
        m_dweight[i][j] = (m_dweight[i][j] - sumGrad * m_weights[i][j]) / sumWeights;
      }
    }
}


template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::init()
{
    distanceType.beginEdit()->setSelectedItem(DISTANCE_EUCLIDIAN);
    distanceType.endEdit();
    const VecInCoord& xfrom = *this->fromModel->getX0();
    if ( this->initPos.empty() && this->toModel!=NULL && computeWeights==true && weights.getValue().size() ==0 )
    {
      /* Temporary remove optimistaion. TODO: reactivate this when the different types will be instanciated
        if ( wheightingType.getValue().getSelectedId() == WEIGHT_LINEAR || wheightingType.getValue().getSelectedId() == WEIGHT_HERMITE )
            nbRefs.setValue ( 2 );

        if ( wheightingType.getValue().getSelectedId() == WEIGHT_SPLINE)
            nbRefs.setValue ( 4 );

        if ( xfrom.size() < nbRefs.getValue())
            nbRefs.setValue ( xfrom.size() );
*/
        nbRefs.setValue ( xfrom.size() );

        computeDistances();
        vector<int>& m_reps = * ( repartition.beginEdit() );
        sortReferences ( m_reps);
        repartition.endEdit();
        updateWeights ();
        computeInitPos ();
    }
    else if ( computeWeights == false || weights.getValue().size() !=0 )
    {
        computeInitPos();
    }


    Inherit::init();
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::clear()
{
    this->initPos.clear();
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::setWeightsToHermite()
{
  wheightingType.beginEdit()->setSelectedItem(WEIGHT_HERMITE);
  wheightingType.endEdit();
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::setWeightsToLinear()
{
  wheightingType.beginEdit()->setSelectedItem(WEIGHT_LINEAR);
  wheightingType.endEdit();
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::setWeightsToInvDist()
{
  wheightingType.beginEdit()->setSelectedItem(WEIGHT_INVDIST_SQUARE);
  wheightingType.endEdit();
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::updateWeights ()
{
    const VecCoord& xto = ( this->toModel->getX0()->size() == 0)?*this->toModel->getX():*this->toModel->getX0();
    const VecInCoord& xfrom = *this->fromModel->getX0();

    VVD& m_weights = * ( weights.beginEdit() );
    SVector<SVector<GeoCoord> >& m_dweight = * ( weightGradients.beginEdit());
    const vector<int>& m_reps = repartition.getValue();

    m_weights.resize ( xfrom.size() );
    for ( unsigned int i=0;i<xfrom.size();i++ )
        m_weights[i].resize ( xto.size() );
    m_dweight.resize ( xfrom.size() );
    for ( unsigned int i=0;i<xfrom.size();i++ )
        m_dweight[i].resize ( xto.size() );

    switch ( wheightingType.getValue().getSelectedId() )
    {
    case WEIGHT_NONE:
    {
        for ( unsigned int j=0;j<xto.size();j++ )
            for ( unsigned int i=0;i<nbRefs.getValue();i++ )
            {
                int indexFrom = m_reps[nbRefs.getValue() *j + i];
                    m_weights[indexFrom][j] = distances[indexFrom][j];
                    m_dweight[indexFrom][j] = distGradients[indexFrom][j];
            }
        break;
    }
    case WEIGHT_LINEAR:
    {
        vector<int> tmpReps;
        sortReferences( tmpReps);
        for ( unsigned int i=0;i<xto.size();i++ )
        {
            for ( unsigned int j=0;j<xfrom.size();j++ )
            {
                m_weights[j][i] = 0.0;
                m_dweight[j][i] = Coord();
            }
            Vec3d r1r2, r1p;
            r1r2 = xfrom[tmpReps[nbRefs.getValue() *i+1]].getCenter() - xfrom[tmpReps[nbRefs.getValue() *i+0]].getCenter();
            r1p  = xto[i] - xfrom[tmpReps[nbRefs.getValue() *i+0]].getCenter();
            double r1r2NormSquare = r1r2.norm()*r1r2.norm();
            double wi = ( r1r2*r1p ) / ( r1r2NormSquare);

            // Abscisse curviligne
            m_weights[tmpReps[nbRefs.getValue() *i+0]][i] = ( 1 - wi );
            m_weights[tmpReps[nbRefs.getValue() *i+1]][i] = wi;
            m_dweight[tmpReps[nbRefs.getValue() *i+0]][i] = -r1r2 / r1r2NormSquare;
            m_dweight[tmpReps[nbRefs.getValue() *i+1]][i] = r1r2 / r1r2NormSquare;
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
                if ( distances[indexFrom][j])
                    m_weights[indexFrom][j] = 1 / (distances[indexFrom][j]*distances[indexFrom][j]);
                else
                    m_weights[indexFrom][j] = 0xFFF;
                if ( distances[indexFrom][j])
                    m_dweight[indexFrom][j] = - distGradients[indexFrom][j] / (double)(distances[indexFrom][j]*distances[indexFrom][j]*distances[indexFrom][j]) * 2.0;
                else
                    m_dweight[indexFrom][j] = Coord();
            }
        }

        break;
    }
    case WEIGHT_HERMITE:
    {
        vector<int> tmpReps;
        sortReferences( tmpReps);
        for ( unsigned int i=0;i<xto.size();i++ )
        {
            for ( unsigned int j=0;j<xfrom.size();j++ )
            {
                m_weights[j][i] = 0.0;
                m_dweight[j][i] = Coord();
            }
            Vec3d r1r2, r1p;
            double wi;
            r1r2 = xfrom[tmpReps[nbRefs.getValue() *i+1]].getCenter() - xfrom[tmpReps[nbRefs.getValue() *i+0]].getCenter();
            r1p  = xto[i] - xfrom[tmpReps[nbRefs.getValue() *i+0]].getCenter();
            double r1r2NormSquare = r1r2.norm()*r1r2.norm();
            wi = ( r1r2*r1p ) / r1r2NormSquare;

            // Fonctions d'Hermite
            m_weights[tmpReps[nbRefs.getValue() *i+0]][i] = 1-3*wi*wi+2*wi*wi*wi;
            m_weights[tmpReps[nbRefs.getValue() *i+1]][i] = 3*wi*wi-2*wi*wi*wi;

            r1r2.normalize();
            m_dweight[tmpReps[nbRefs.getValue() *i+0]][i] = -r1r2 * (6*wi-6*wi*wi) / (r1r2NormSquare);
            m_dweight[tmpReps[nbRefs.getValue() *i+1]][i] = r1r2 * (6*wi-6*wi*wi) / (r1r2NormSquare);
        }
        break;
    }
    case WEIGHT_SPLINE:
    {
        if( xfrom.size() < 4 || nbRefs.getValue() < 4)
        {
          serr << "Error ! To use WEIGHT_SPLINE, you must use at least 4 DOFs and set nbRefs to 4.\n WEIGHT_SPLINE requires also the DOFs are ordered along z-axis." << sendl;
          return;
        }
        vector<int> tmpReps;
        sortReferences( tmpReps);
        for ( unsigned int i=0;i<xto.size();i++ )
        {
            // Clear all weights and dweights.
            for ( unsigned int j=0;j<xfrom.size();j++ )
            {
                m_weights[j][i] = 0.0;
                m_dweight[j][i] = Coord();
            }
            // Get the 4 nearest DOFs.
            vector<unsigned int> sortedFrames;
            for( unsigned int j = 0; j < 4; ++j)
              sortedFrames.push_back( tmpReps[nbRefs.getValue() *i+j]);
            std::sort( sortedFrames.begin(), sortedFrames.end());
            
            if( xto[i][2] < xfrom[sortedFrames[1]].getCenter()[2])
            {
              sortedFrames.clear();
              sortedFrames.push_back( 0);
              sortedFrames.push_back( 0);
              sortedFrames.push_back( 1);
              sortedFrames.push_back( 2);
            }
            else if( xto[i][2] > xfrom[sortedFrames[2]].getCenter()[2])
            {
              sortedFrames.clear();
              sortedFrames.push_back( sortedFrames[1]);
              sortedFrames.push_back( sortedFrames[2]);
              sortedFrames.push_back( sortedFrames[3]);
              sortedFrames.push_back( sortedFrames[3]);
            }
              
            // Compute u
            Vec3d r1r2 = xfrom[sortedFrames[2]].getCenter() - xfrom[sortedFrames[1]].getCenter();
            Vec3d r1p  = xto[i] - xfrom[sortedFrames[1]].getCenter();
            double r1r2NormSquare = r1r2.norm()*r1r2.norm();
            double u = ( r1r2*r1p ) / r1r2NormSquare;

            // Set weights and dweights.
            m_weights[sortedFrames[0]][i] = 1-3*u*u+2*u*u*u;
            m_weights[sortedFrames[1]][i] = u*u*u - 2*u*u + u;
            m_weights[sortedFrames[2]][i] = 3*u*u-2*u*u*u;
            m_weights[sortedFrames[3]][i] = u*u*u - u*u;

            r1r2.normalize();
            m_dweight[sortedFrames[0]][i] = -r1r2 * (6*u - 6*u*u) / (r1r2NormSquare);
            m_dweight[sortedFrames[1]][i] = r1r2 * (3*u*u - 4*u + 1) / (r1r2NormSquare);
            m_dweight[sortedFrames[2]][i] = r1r2 * (6*u - 6*u*u) / (r1r2NormSquare);
            m_dweight[sortedFrames[3]][i] = r1r2 * (3*u*u - 2*u) / (r1r2NormSquare);
        }
        break;
    }
    default:
    {}
    }

    normalizeWeights();
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::setWeightCoefs ( VVD &weights )
{
    VVD * m_weights = this->weights.beginEdit();
    m_weights->clear();
    m_weights->insert ( m_weights->begin(), weights.begin(), weights.end() );
    this->weights.endEdit();
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::setRepartition ( vector<int> &rep )
{
    vector<int> * m_reps = repartition.beginEdit();
    m_reps->clear();
    m_reps->insert ( m_reps->begin(), rep.begin(), rep.end() );;
    repartition.endEdit();
}


template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
{
	_apply< InCoord >(out, in);
}


template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
{
	_applyJ< InDeriv >(out, in);
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
{
	_applyJT< InDeriv >(out, in);
}


template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
{
	_applyJT_Matrix< InMatrixDeriv >( out, in);
}

template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::draw()
{
    const typename Out::VecCoord& xto = *this->toModel->getX();
    const typename In::VecCoord& xfrom = *this->fromModel->getX();
    const vector<int>& m_reps = repartition.getValue();
    const VVD& m_weights = weights.getValue();
    const SVector<SVector<GeoCoord> >& dw = weightGradients.getValue();
    const unsigned int nbRef = nbRefs.getValue();
    const int valueScale = showValuesNbDecimals.getValue();
    int scale = 1;
    for (int i = 0; i < valueScale; ++i) scale *= 10;
    const double textScale = showTextScaleFactor.getValue();

    glDisable ( GL_LIGHTING );

    if ( this->getShow() )
    {
        // Display mapping links between in and out elements
            glDisable ( GL_LIGHTING );
            glPointSize ( 1 );
            glColor4f ( 1,1,0,1 );
            glBegin ( GL_LINES );

            for ( unsigned int i=0; i<xto.size(); i++ )
            {
                for ( unsigned int m=0 ; m<nbRef; m++ )
                {
                    const int idxReps=m_reps[nbRef *i+m];
                    double coef = m_weights[idxReps][i];
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

    // Display  m_reps for each points
    if ( showReps.getValue())
    {
        for ( unsigned int i=0;i<xto.size();i++ )
            sofa::helper::gl::GlText::draw ( m_reps[nbRef*i+0]*scale, xto[i], textScale );
    }

    // Display distances for each points
    if ( showDistancesValues.getValue())
    {
        glColor3f( 1.0, 1.0, 1.0);
        for ( unsigned int i=0;i<xto.size();i++ )
            sofa::helper::gl::GlText::draw ( (int)(distances[m_reps[i*nbRef+showFromIndex.getValue()%distances.size()]][i]*scale), xto[i], textScale );
    }

    // Display distance gradients values for each points
    if ( showGradientsValues.getValue())
    {
        char txt[100];
        glColor3f( 0.5, 0.5, 0.5);
        for ( unsigned int i=0;i<xto.size();i++ )
        {
            const Vec3& grad = dw[m_reps[i*nbRef+showFromIndex.getValue()%dw.size()]][i];
            sprintf( txt, "( %i, %i, %i)", (int)(grad[0]*scale), (int)(grad[1]*scale), (int)(grad[2]*scale));
            sofa::helper::gl::GlText::draw ( txt, xto[i], textScale );
        }
    }

    // Display weights for each points
    if ( showWeightsValues.getValue())
    {
        glColor3f( 1.0, 1.0, 1.0);
        for ( unsigned int i=0;i<xto.size();i++ )
            sofa::helper::gl::GlText::draw ( (int)(m_weights[m_reps[i*nbRef+showFromIndex.getValue()%m_weights.size()]][i]*scale), xto[i], textScale );
    }

    // Display weights gradients for each points
    if ( showGradients.getValue())
    {
        glColor3f ( 0.0, 1.0, 0.3 );
        glBegin ( GL_LINES );
        for ( unsigned int j = 0; j < xto.size(); j++ )
        {
            const GeoCoord& gradMap = dw[m_reps[j*nbRef+showFromIndex.getValue()%dw.size()]][j];
            const Coord& point = xto[j];
            glVertex3f ( point[0], point[1], point[2] );
            glVertex3f ( point[0] + gradMap[0] * showGradientsScaleFactor.getValue(), point[1] + gradMap[1] * showGradientsScaleFactor.getValue(), point[2] + gradMap[2] * showGradientsScaleFactor.getValue() );
        }
        glEnd();
    }
    //*/

}



template <class TIn, class TOut>
void SkinningMapping<TIn, TOut>::getLocalCoord( Coord& result, const typename defaulttype::StdRigidTypes<N, InReal>::Coord& inCoord, const Coord& coord) const
{
  result = inCoord.getOrientation().inverseRotate ( coord - inCoord.getCenter() );
}



// Generic Apply (old one in .inl)
template <class TIn, class TOut>
template<class TCoord>
typename enable_if<Equal<typename SkinningMapping<TIn, TOut>::RigidType::Coord, TCoord > >::type
SkinningMapping<TIn, TOut>::_apply( typename Out::VecCoord& out, const sofa::helper::vector<typename RigidType::Coord>& in)
{
    const vector<int>& m_reps = repartition.getValue();
    const VVD& m_weights = weights.getValue();

        rotatedPoints.resize ( initPos.size() );
        out.resize ( initPos.size() / nbRefs.getValue() );
        for ( unsigned int i=0 ; i<out.size(); i++ )
        {
            out[i] = Coord();
            for ( unsigned int j = 0; j < nbRefs.getValue(); ++j)
            {
                const int& idx=nbRefs.getValue() *i+j;
                const int& idxReps=m_reps[idx];

                // Save rotated points for applyJ/JT
                rotatedPoints[idx] = in[idxReps].getOrientation().rotate ( initPos[idx] );

                // And add each reference frames contributions to the new position out[i]
                out[i] += ( in[idxReps ].getCenter() + rotatedPoints[idx] ) * m_weights[idxReps][i];
            }
            
        }
}


template <class TIn, class TOut>
template<class TDeriv>
typename enable_if<Equal<typename SkinningMapping<TIn, TOut>::RigidType::Deriv, TDeriv> >::type SkinningMapping<TIn, TOut>::_applyJ( typename Out::VecDeriv& out, const sofa::helper::vector<typename RigidType::Deriv>& in)
{
    const vector<int>& m_reps = repartition.getValue();
    const VVD& m_weights = weights.getValue();
    const VecCoord& xto = *this->toModel->getX();
    out.resize ( xto.size() );
    Deriv v,omega;

    if ( ! ( maskTo->isInUse() ) )
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
                out[i] += ( v - cross ( rotatedPoints[idx],omega ) ) * m_weights[idxReps][i];
            }
        }
    }
    else
    {
        typedef helper::ParticleMask ParticleMask;
        const ParticleMask::InternalStorage &indices=maskTo->getEntries();

        ParticleMask::InternalStorage::const_iterator it;
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
                out[i] += ( v - cross ( rotatedPoints[idx],omega ) ) * m_weights[idxReps][i];
            }
        }
    }
}


template <class TIn, class TOut>
template<class TDeriv>
typename enable_if<Equal<typename SkinningMapping<TIn, TOut>::RigidType::Deriv, TDeriv> >::type SkinningMapping<TIn, TOut>::_applyJT( sofa::helper::vector<typename RigidType::Deriv>& out, const typename Out::VecDeriv& in)
{
    const vector<int>& m_reps = repartition.getValue();
    const VVD& m_weights = weights.getValue();

    Deriv v,omega;
    if ( ! ( maskTo->isInUse() ) )
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
                out[idxReps].getVCenter() += v * m_weights[idxReps][i];
                out[idxReps].getVOrientation() += omega * m_weights[idxReps][i];
            }
        }
    }
    else
    {
        typedef helper::ParticleMask ParticleMask;
        const ParticleMask::InternalStorage &indices=maskTo->getEntries();

        ParticleMask::InternalStorage::const_iterator it;
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
                out[idxReps].getVCenter() += v * m_weights[idxReps][i];
                out[idxReps].getVOrientation() += omega * m_weights[idxReps][i];

                maskFrom->insertEntry ( idxReps );
            }
        }
    }
}


template <class TIn, class TOut>
template<class TMatrixDeriv>
typename enable_if<Equal<typename SkinningMapping<TIn, TOut>::RigidType::MatrixDeriv, TMatrixDeriv> >::type SkinningMapping<TIn, TOut>::_applyJT_Matrix( typename RigidType::MatrixDeriv& out, const typename Out::MatrixDeriv& in)
{
    const vector<int>& m_reps = repartition.getValue();
    const VVD& m_weights = weights.getValue();
    const unsigned int nbr = nbRefs.getValue();
    const unsigned int nbp = this->fromModel->getX()->size();
    Deriv omega;
    typename In::VecDeriv v;
    vector<bool> flags;

    typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();

    for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
    {
        v.clear();
        v.resize(nbp);
        flags.clear();
        flags.resize(nbp);

        typename In::MatrixDeriv::RowIterator o = out.end();

        typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();

        for (typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin(); colIt != colItEnd; ++colIt)
        {
            unsigned int indexPoint = colIt.index();
            Deriv data = ( Deriv ) colIt.val();

            for (unsigned int m = 0 ; m < nbr; m++)
            {
                omega = cross(rotatedPoints[nbr * indexPoint + m], data);
                flags[m_reps[nbr * indexPoint + m]] = true;
                v[m_reps[nbr * indexPoint + m]].getVCenter() += data * m_weights[m_reps[nbr * indexPoint + m]][indexPoint];
                v[m_reps[nbr * indexPoint + m]].getVOrientation() += omega * m_weights[m_reps[nbr * indexPoint + m]][indexPoint];
            }

            for (unsigned int j = 0 ; j < nbp; j++)
            {
                if (flags[j])
                {
                    // Create an unique new line for each contraint
                    if (o == out.end())
                    {
                        o = out.writeLine(rowIt.index());
                    }

                    o.addCol(j, v[j]);
                }
            }
        }
    }
}




} // namespace mapping

} // namespace component

} // namespace sofa

#endif

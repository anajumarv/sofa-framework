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
#ifndef SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_PARTIALLINEARMOVEMENTCONSTRAINT_INL
#define SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_PARTIALLINEARMOVEMENTCONSTRAINT_INL

#include <sofa/component/projectiveconstraintset/PartialLinearMovementConstraint.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/core/behavior/ProjectiveConstraintSet.inl>
#include <sofa/simulation/common/Simulation.h>
#include <sofa/helper/gl/template.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <iostream>

#include <sofa/component/topology/PointSubset.h>



namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{

using namespace core::topology;

using namespace sofa::defaulttype;
using namespace sofa::helper;
using namespace sofa::core::behavior;


// Define TestNewPointFunction
template< class DataTypes>
bool PartialLinearMovementConstraint<DataTypes>::FCTestNewPointFunction(int /*nbPoints*/, void* param, const sofa::helper::vector< unsigned int > &, const sofa::helper::vector< double >& )
{
    PartialLinearMovementConstraint<DataTypes> *fc= (PartialLinearMovementConstraint<DataTypes> *)param;
	if (fc) {
		return true;
	}else{
		return false;
	}
}

// Define RemovalFunction
template< class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::FCRemovalFunction(int pointIndex, void* param)
{
    PartialLinearMovementConstraint<DataTypes> *fc= (PartialLinearMovementConstraint<DataTypes> *)param;
	if (fc) {
		fc->removeIndex((unsigned int) pointIndex);
	}
	return;
} 

template <class DataTypes>
PartialLinearMovementConstraint<DataTypes>::PartialLinearMovementConstraint()
: core::behavior::ProjectiveConstraintSet<DataTypes>(NULL)
, m_indices( initData(&m_indices,"indices","Indices of the constrained points") )
, m_keyTimes(  initData(&m_keyTimes,"keyTimes","key times for the movements") )
, m_keyMovements(  initData(&m_keyMovements,"movements","movements corresponding to the key times") )
, movedDirections( initData(&movedDirections,"movedDirections","for each direction, 1 if moved, 0 if free") )
, showMovement( initData(&showMovement, (bool)false, "showMovement", "Visualization of the movement to be applied to constrained dofs."))
, linearMovementBetweenNodesInIndices( initData(&linearMovementBetweenNodesInIndices, (bool)false, "linearMovementBetweenNodesInIndices", "Take into account the linear movement between the constrained points"))
, mainIndice( initData(&mainIndice, "mainIndice", "The main indice node in the list of constrained nodes, it defines how to apply the linear movement between this constrained nodes "))
, minDepIndice( initData(&minDepIndice, "minDepIndice", "The indice node in the list of constrained nodes, which is imposed the minimum displacment "))
, maxDepIndice( initData(&maxDepIndice, "maxDepIndice", "The indice node in the list of constrained nodes, which is imposed the maximum displacment "))
, m_imposedDisplacmentOnMacroNodes(  initData(&m_imposedDisplacmentOnMacroNodes,"imposedDisplacmentOnMacroNodes","The imposed displacment on macro nodes") )
, X0 ( initData ( &X0,(Real) 0.0,"X0","Size of specimen in X-direction" ) )
, Y0 ( initData ( &Y0,(Real) 0.0,"Y0","Size of specimen in Y-direction" ) )
, Z0 ( initData ( &Z0,(Real) 0.0,"Z0","Size of specimen in Z-direction" ) )
{
    // default to indice 0
    m_indices.beginEdit()->push_back(0);
    m_indices.endEdit();

	//default valueEvent to 0
	m_keyTimes.beginEdit()->push_back( 0.0 );
	m_keyTimes.endEdit();
	m_keyMovements.beginEdit()->push_back( Deriv() );
	m_keyMovements.endEdit();
    VecBool movedDirection;
    for( unsigned i=0; i<NumDimensions; i++)
        movedDirection[i] = true;
    movedDirections.setValue(movedDirection);
}


// Handle topological changes
template <class DataTypes> void PartialLinearMovementConstraint<DataTypes>::handleTopologyChange()
{	
	std::list<const TopologyChange *>::const_iterator itBegin=topology->beginChange();
	std::list<const TopologyChange *>::const_iterator itEnd=topology->endChange();

	m_indices.beginEdit()->handleTopologyEvents(itBegin,itEnd,this->getMState()->getSize());

}

template <class DataTypes>
PartialLinearMovementConstraint<DataTypes>::~PartialLinearMovementConstraint()
{
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::clearIndices()
{
    m_indices.beginEdit()->clear();
    m_indices.endEdit();
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::addIndex(unsigned int index)
{
    m_indices.beginEdit()->push_back(index);
    m_indices.endEdit();
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::removeIndex(unsigned int index)
{
    removeValue(*m_indices.beginEdit(),index);
    m_indices.endEdit();
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::clearKeyMovements()
{
	m_keyTimes.beginEdit()->clear();
    m_keyTimes.endEdit();
    m_keyMovements.beginEdit()->clear();
    m_keyMovements.endEdit();
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::addKeyMovement(Real time, Deriv movement)
{
    m_keyTimes.beginEdit()->push_back( time );
    m_keyTimes.endEdit();
	m_keyMovements.beginEdit()->push_back( movement );
    m_keyMovements.endEdit();
}

// -- Constraint interface


template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::init()
{
    this->core::behavior::ProjectiveConstraintSet<DataTypes>::init();

	topology = this->getContext()->getMeshTopology();

	// Initialize functions and parameters
	topology::PointSubset my_subset = m_indices.getValue();

	my_subset.setTestFunction(FCTestNewPointFunction);
	my_subset.setRemovalFunction(FCRemovalFunction);

	my_subset.setTestParameter( (void *) this );
	my_subset.setRemovalParameter( (void *) this );

	x0.resize(0);
	nextM = prevM = Deriv();

	currentTime = -1.0;
	finished = false;
}


template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::reset()
{
	nextT = prevT = 0.0;
	nextM = prevM = Deriv();

	currentTime = -1.0;
	finished = false;
}


template <class DataTypes> template <class DataDeriv>
void PartialLinearMovementConstraint<DataTypes>::projectResponseT(DataDeriv& dx)
{
        Real cT = (Real) this->getContext()->getTime();
        VecBool movedDirection = movedDirections.getValue();
        if ((cT != currentTime) || !finished)
        {
                findKeyTimes();
        }

        if (finished && nextT != prevT)
        {
                const SetIndexArray & indices = m_indices.getValue().getArray();

                //set the motion to the Dofs
                for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
                {
                    for( unsigned j=0; j<NumDimensions; j++)
                        if(movedDirection[j]) dx[*it][j] = (Real) 0.0;
                }
        }
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::projectResponse(VecDeriv& res)
{
  projectResponseT(res);
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::projectResponse(SparseVecDeriv& res)
{
  projectResponseT(res);
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::projectVelocity(VecDeriv& dx)
{
	Real cT = (Real) this->getContext()->getTime(); 
	if ((cT != currentTime) || !finished)
	{
		findKeyTimes();
	}

	if (finished && nextT != prevT) 
	{
	  	const SetIndexArray & indices = m_indices.getValue().getArray();

	  	//set the motion to the Dofs 
	  	for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
	  	{
			dx[*it] = (nextM - prevM)*(1.0 / (nextT - prevT));
	  	}	
	}
}


template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::projectPosition(VecCoord& x)
{
	Real cT = (Real) this->getContext()->getTime(); 

	//initialize initial Dofs positions, if it's not done
	if (x0.size() == 0){
		const SetIndexArray & indices = m_indices.getValue().getArray();
		x0.resize( x.size() );
		for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
			x0[*it] = x[*it];
	}

	if ((cT != currentTime) || !finished)
	{
		findKeyTimes();
	}
    VecBool movedDirection = movedDirections.getValue();
	//if we found 2 keyTimes, we have to interpolate a velocity (linear interpolation)
	if(finished && nextT != prevT)
	{
		const SetIndexArray & indices = m_indices.getValue().getArray();

		Real dt = (cT - prevT) / (nextT - prevT);
		Deriv m = prevM + (nextM-prevM)*dt;

		//set the motion to the Dofs 
        if(linearMovementBetweenNodesInIndices.getValue())
        {

                const helper::vector<Real> &imposedDisplacmentOnMacroNodes = this->m_imposedDisplacmentOnMacroNodes.getValue();
                Real a = X0.getValue();
                Real b = Y0.getValue();
                Real c = Z0.getValue();
                bool case2d=false;
                if((a==0.0)||(b==0.0)||(c==0.0)) case2d=true;
                if(a==0.0) {a=b;b=c;}
                if(b==0.0) {b=c;}

                for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
                {
                    for( unsigned j=0; j< NumDimensions; j++)
                    {
                        if(movedDirection[j])
                        {
                            if(case2d)
                            {
                                x[*it][j] = x0[*it][j] + (1.0/(a*b))*((a-x0[*it][0])*(b-x0[*it][1])*imposedDisplacmentOnMacroNodes[0]+   ///< N1
                                                                      x0[*it][0]*(b-x0[*it][1])*imposedDisplacmentOnMacroNodes[1]+         ///< N2
                                                                      x0[*it][0]*x0[*it][1]*imposedDisplacmentOnMacroNodes[2]+              ///< N3
                                                                      (a-x0[*it][0])*x0[*it][1]*imposedDisplacmentOnMacroNodes[3])*m[j];    ///< N4
                                                           /*   4|----------------|3
                                                                 |                |
                                                                 |                |
                                                                 |                |
                                                                1|----------------|2     */
                            }
                            else ///< case3d
                            {   //        |Y
                                // 	      5---------8
                                //       /|	       /|
                                //      / |	      / |
                                //     6--|------7  |
                                //     |  |/	 |  |
                                //     |  1------|--4--->X
                                //     | / 	     | /
                                //     |/	     |/
                                //     2---------3
                                //   Z/
                                //

                                x[*it][j] = x0[*it][j] + (1.0/(a*b*c))*(
                                        (a-x0[*it][0])*(b-x0[*it][1])*(c-x0[*it][2])*imposedDisplacmentOnMacroNodes[0]+    ///< N1
                                        (a-x0[*it][0])*(b-x0[*it][1])*x0[*it][2]*imposedDisplacmentOnMacroNodes[1]+        ///< N2
                                        x0[*it][0]*(b-x0[*it][1])*x0[*it][2]*imposedDisplacmentOnMacroNodes[2]+            ///< N3
                                        x0[*it][0]*(b-x0[*it][1])*(c-x0[*it][2])*imposedDisplacmentOnMacroNodes[3]+        ///< N4
                                        (a-x0[*it][0])*x0[*it][1]*(c-x0[*it][2])*imposedDisplacmentOnMacroNodes[4]+        ///< N5
                                        (a-x0[*it][0])*x0[*it][1]*x0[*it][2]*imposedDisplacmentOnMacroNodes[5]+            ///< N6
                                        x0[*it][0]*x0[*it][1]*x0[*it][2]*imposedDisplacmentOnMacroNodes[6]+                ///< N7
                                        x0[*it][0]*x0[*it][1]*(c-x0[*it][2])*imposedDisplacmentOnMacroNodes[7]             ///< N8

                                        )*m[j];

                            }
                        }
                    }
                }                       

        }
        else
        {
            for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
            {
                for( unsigned j=0; j< NumDimensions; j++)
                    if(movedDirection[j]) x[*it][j] = x0[*it][j] + m[j] ;
            }
        }
	}
}

template <class DataTypes>
void PartialLinearMovementConstraint<DataTypes>::findKeyTimes()
{
	Real cT = (Real) this->getContext()->getTime();
	finished = false;

	if(m_keyTimes.getValue().size() != 0 && cT >= *m_keyTimes.getValue().begin() && cT <= *m_keyTimes.getValue().rbegin())
	{
		nextT = *m_keyTimes.getValue().begin();
		prevT = nextT;

		typename helper::vector<Real>::const_iterator it_t = m_keyTimes.getValue().begin();
		typename VecDeriv::const_iterator it_m = m_keyMovements.getValue().begin();
	 
		//WARNING : we consider that the key-events are in chronological order
		//here we search between which keyTimes we are, to know which are the motion to interpolate
		while( it_t != m_keyTimes.getValue().end() && !finished)
		{
			if( *it_t <= cT) {
				prevT = *it_t;
				prevM = *it_m;
			}
			else {
				nextT = *it_t;
				nextM = *it_m;
				finished = true;
			}
			it_t++;
			it_m++;
		}
	}
}


//display the path the constrained dofs will go through
  template <class DataTypes>
  void PartialLinearMovementConstraint<DataTypes>::draw()
  {
      if (!this->getContext()->getShowBehaviorModels() || m_keyTimes.getValue().size() == 0 ) return;
      if (showMovement.getValue())
      {
          glDisable (GL_LIGHTING);
          glPointSize(10);
          glColor4f (1,0.5,0.5,1);
          glBegin (GL_LINES);
          const SetIndexArray & indices = m_indices.getValue().getArray();
          if(linearMovementBetweenNodesInIndices.getValue())
          {
              const VecCoord& x = *this->mstate->getX();
                  for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
                  {
                      gl::glVertexT(x0[*it]);
                      gl::glVertexT(x[*it]);
                  }

          }
          else
          {

              for (unsigned int i=0 ; i<m_keyMovements.getValue().size()-1 ; i++)
              {
                  for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
                  {
                      gl::glVertexT(x0[*it]+m_keyMovements.getValue()[i]);
                      gl::glVertexT(x0[*it]+m_keyMovements.getValue()[i+1]);
                  }
              }
          }
          glEnd();
      }
      else
      {
          const VecCoord& x = *this->mstate->getX();

          sofa::helper::vector< Vector3 > points;
          Vector3 point;
          unsigned int sizePoints= (Coord::static_size <=3)?Coord::static_size:3;
          const SetIndexArray & indices = m_indices.getValue().getArray();
          for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
          {
              for (unsigned int s=0;s<sizePoints;++s) point[s] = x[*it][s];
              points.push_back(point);
          }
          simulation::getSimulation()->DrawUtility.drawPoints(points, 10, Vec<4,float>(1,0.5,0.5,1));
      }
  }

// Specialization for rigids
template <>
void PartialLinearMovementConstraint<Rigid3dTypes >::draw();
template <>
void PartialLinearMovementConstraint<Rigid3fTypes >::draw();

} // namespace constraint

} // namespace component

} // namespace sofa

#endif


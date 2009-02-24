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

#include <sofa/component/topology/PointSubset.h>
#include <sofa/component/topology/PointSetTopologyChange.h> 

namespace sofa
{

namespace component
{

namespace topology
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////implementation//////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void PointSubset::handleTopologyEvents( std::list<  const core::componentmodel::topology::TopologyChange *>::const_iterator changeIt, 
										std::list< const core::componentmodel::topology::TopologyChange *>::const_iterator &end,
										const unsigned int totalPointSetArraySize) 
{
	setTotalPointSetArraySize(totalPointSetArraySize);
	while( changeIt != end )
	{
		core::componentmodel::topology::TopologyChangeType changeType = (*changeIt)->getChangeType();

		switch( changeType ) 
		{
		case core::componentmodel::topology::POINTSINDICESSWAP:
		{
			unsigned int i1 = ( static_cast< const PointsIndicesSwap * >( *changeIt ) )->index[0];
			unsigned int i2 = ( static_cast< const PointsIndicesSwap* >( *changeIt ) )->index[1];
			swap( i1, i2 );
			break;
		}
		case core::componentmodel::topology::POINTSADDED:
		{
			unsigned int nbPoints = ( static_cast< const PointsAdded * >( *changeIt ) )->getNbAddedVertices();
			helper::vector< helper::vector< unsigned int > > ancestors = ( static_cast< const PointsAdded * >( *changeIt ) )->ancestorsList;
			helper::vector< helper::vector< double       > > coefs     = ( static_cast< const PointsAdded * >( *changeIt ) )->coefs;
			add( nbPoints, ancestors, coefs);
			lastPointIndex+=nbPoints;
			break;
		}
		case core::componentmodel::topology::POINTSREMOVED:
		{
			const sofa::helper::vector<unsigned int> tab = ( static_cast< const PointsRemoved * >( *changeIt ) )->getArray();
			remove( tab );
			break;
		}
		case core::componentmodel::topology::POINTSRENUMBERING:
		{
			const sofa::helper::vector<unsigned int> &tab = ( static_cast< const PointsRenumbering * >( *changeIt ) )->getinv_IndexArray();
			renumber( tab );
			break;
		}
		default:
			// Ignore events that are not point related.
			break;
		}; // switch( changeType )

		++changeIt;
	} // while( changeIt != last; )
}

void PointSubset::swap( unsigned int i1, unsigned int i2 ) 
{
	helper::vector<unsigned int>::iterator it= find(m_subset.begin(),m_subset.end(),i1);
	if (it!=m_subset.end()) 
		(*it)=i2;
}

void PointSubset::add( unsigned int nbPoints, 
					  const sofa::helper::vector< sofa::helper::vector< unsigned int > >& ancestors, 
					  const sofa::helper::vector< sofa::helper::vector< double > >& coefs) 
{
	// Using default values
	unsigned int size = m_subset.size();
	bool test;
	for (unsigned int i = 0; i < nbPoints; ++i)
	{
		if (ancestors.empty() || coefs.empty())
		{
			const sofa::helper::vector< unsigned int > empty_vecint;
			const sofa::helper::vector< double > empty_vecdouble;
			test=(*m_testNewPointFunc)( size + i, m_testParam, empty_vecint, empty_vecdouble);
		}
		else
			test=(*m_testNewPointFunc)( size + i, m_testParam, ancestors[i], coefs[i] );

		if (test)
			m_subset.push_back( size+i );
	}
}

void PointSubset::remove( const sofa::helper::vector<unsigned int> &index) 
{
	unsigned int it1;
	unsigned int it2;

	for (unsigned int i = 0; i < index.size(); ++i)
	{							
		it1=0;
		while(it1<m_subset.size())
		{
			if(m_subset[it1]==index[i])
			{
				break;
			}
			else
			{
				it1+=1;
			}
		}		

		if (it1<m_subset.size()) 
		{					
			it2=0;
			while(it2<m_subset.size())
			{
				if(m_subset[it2]==lastPointIndex)
				{
					break;
				}
				else
				{
					it2+=1;
				}
			}	

			if (it2<m_subset.size()) 
			{						
				m_subset[it2]=index[i];
			}

			m_subset[it1]=m_subset[m_subset.size()-1];
			m_removalFunc( index[i], m_removeParam); 
			m_subset.resize(m_subset.size()-1);

		}
		else
		{
			it2=0;
			while(it2<m_subset.size())
			{
				if(m_subset[it2]==lastPointIndex)
				{
					break;
				}
				else
				{
					it2+=1;
				}
			}

			if (it2<m_subset.size()) 
			{						
				m_subset[it2]=index[i];
			}
		}
		--lastPointIndex;
	}
}

void PointSubset::renumber( const sofa::helper::vector<unsigned int> &index ) 
{
	for (unsigned int i = 0; i < m_subset.size(); ++i)
	{
		m_subset[i] =index[m_subset[i] ];
	}
}

/// Needed to be compliant with Data
std::ostream& operator<< (std::ostream& os, const PointSubset& pointSubset)
{
	return pointSubset.m_subset.write(os);
}

/// Needed to be compliant with Data
std::istream& operator>>(std::istream& i, PointSubset& pointSubset)
{
	return pointSubset.m_subset.read(i);
}

} // namespace topology

} // namespace component

} // namespace sofa


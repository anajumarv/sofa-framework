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
#include <sofa/component/collision/SphereTreeModel.h>
#include <sofa/helper/io/SphereLoader.h>
#include <sofa/component/collision/CubeModel.h>
#include <sofa/core/ObjectFactory.h>
#include <iostream>
#include <sofa/helper/system/gl.h>
#include <sofa/helper/system/glut.h>
#include <sofa/helper/system/FileRepository.h>
#include <sofa/component/container/MechanicalObject.inl>
#include <sofa/core/State.h>








namespace sofa
{

namespace component
{

namespace collision
{

// Required for compilation purposes
SOFA_DECL_CLASS(SphereTreeModel) 

using namespace sofa::defaulttype;
using namespace std;



int SphereTreeModelClass = core::RegisterObject("CollisionModel based on a hierarchy of bounding spheres")
.add< SphereTreeModel >()
.addAlias("SphereTree")
;

SphereTreeModel::SphereTreeModel()
: filename(initData(&filename,"filename","Sphere tree description file (.sph)")) 
,defaultRadius(initData(&defaultRadius, 1.0, "radius","Default radius."))
,scale(initData(&scale,1.0,"scale","Uniform scale of the spheres"))
,translation(initData(&translation,Vector3(),"translation","Translation of the spheres"))
,mstate(NULL)
{
}


void SphereTreeModel::init()
{
  mstate = dynamic_cast< core::behavior::MechanicalState<DataTypes>* > (this->getContext()->getMechanicalState());
  if (filename.getValue() != "") load(filename.getFullPath().c_str());
  applyScale(scale.getValue());
  applyTranslation(translation.getValue()[0],translation.getValue()[1],translation.getValue()[2]);

}

void SphereTreeModel::resize(int size)
{   // Correcting sizes
	mstate->resize(size);
	this->core::CollisionModel::resize(size);
	//this->elems.resize(size);
	
	if ((int)radius.size() < size)
	{
		radius.reserve(size);
		while ((int)radius.size() < size)
			radius.push_back(defaultRadius.getValue());
	}
	else
	{
		radius.resize(size);
	}
}

int SphereTreeModel::addSphere(const Vector3& pos, SReal radius)
{
	int i = size;
	resize(i+1);
	setSphere(i, pos, radius);
	return i;
}

void SphereTreeModel::setSphere(int i, const Vector3& pos, SReal r)
{
	if ((unsigned)i >= (unsigned) size) return;
	helper::WriteAccessor<Data<VecCoord> > xData = *mstate->write(core::VecCoordId::position());
	xData.wref()[i] = pos;
	radius[i] = r;
}

bool SphereTreeModel::load(const char* filename)
{
	this->resize(0);
	return loadSphereTree(filename);
}

bool SphereTreeModel::loadSphereTree( const char *fileName )
{	
  std::ifstream inFile;
  
  std::string fileInRepository(fileName);
  if ( !sofa::helper::system::DataRepository.findFile ( fileInRepository ) )
    return false;

  fileInRepository = sofa::helper::system::DataRepository.getFile ( fileInRepository);	
  
  inFile.open(fileInRepository.c_str());
  if (inFile.fail()) 
  {
    serr << "Fail to open file sph" << sendl;
    return false;
  }
  levels = 0; degree = 0;
  inFile >> levels >> degree;
  
  if (!levels || !degree)
    return false;

  int numnodes = 1;
  int numSpheres=0;
 
  double x, y, z, r, errDec;
  for (int level = 0; level < levels; level++ ) 
  { 
//     bool isLeaf = (level==levels-1);
    for (int i = 0; i < numnodes; i++) 
	{  
      inFile >> x >> y >> z >> r >> errDec;

      if (inFile.fail()) 
	  {
        serr << "Error: incorrect file format sph " << sendl;
        return false;
      }
        
		resize( numSpheres+1 ); //need to resize the "x" vector of DOF's of the sphereTree

	   // Collision element iterator
		SingleSphere s(this, numSpheres);
		s.setCenter(x,y,z);
		radius[numSpheres] = r; 
	   			
#if 0
		SphereData sphData;
		//sphData.isLeaf = isLeaf;

		if (isLeaf) 
		{
			sphData.leaf = core::CollisionElementIterator(this,numSpheres);
		}
		else
		{
			sphData.leaf = core::CollisionElementIterator();
		}

		sphData.node = s;
	
		this->elems.push_back( sphData );
#endif
		numSpheres=numSpheres+1;   	    
	   	  
    } // end numnodes
    numnodes *= degree;
  } // end for
#if 0
   //Create relationships
  numnodes = 1;
  int node = 0;
  int nodeinit = 0;
  int firstchild;
  for (int level = 0; level < levels-1; level++ ) 
  {	
	 for (int i = 0; i < numnodes; i++) 
	 {	
		while ( node < nodeinit ) // <=
		{
		    firstchild = node * degree + 1;
			for ( int deg = 0; deg <= degree; deg++) 
			{  
				//core::CollisionElementIterator *s = this->nodes[firstchild + deg];
				SphereData s = this->elems[ firstchild + deg ];
				core::CollisionElementIterator begin(this, (firstchild+deg) );	
				core::CollisionElementIterator end(this, (firstchild+degree) );
				//Setting first child.
				if ( deg == 0 ) this->elems[node].subcells.first = begin;

				//Setting end for children 
				if ( deg == degree ) this->elems[node].subcells.second = end;

			} // endfor degree.
		node++;	
		} // endwhile	
	} // end for numnodes
    numnodes *= degree;
    nodeinit = node+numnodes;
  } //end for levels
#endif
  inFile.close();
  return true; 
}

void SphereTreeModel::applyScale(const double sx)
{
  mstate->applyScale(sx,sx,sx);
  sout << "Applying scale " << sx << " to " << size << " spheres" << sendl;
	for (int i=0;i<size;i++) radius[i] *= sx;
}

void SphereTreeModel::applyTranslation(const double dx, const double dy, const double dz)
{
  for( int index = 0; index < mstate->getSize(); ++index)
  {
    // Collision element iterator
    SingleSphere s(this, index);
    s.translate(dx,dy,dz);

  }
  
}

void SphereTreeModel::draw(const core::visual::VisualParams* ,int index)
{
	Vector3 p = (*mstate->getX())[index];
	glPushMatrix();
	glTranslated(p[0], p[1], p[2]);
	glutWireSphere(radius[index], 16, 8);
	glPopMatrix();
}

void SphereTreeModel::draw(const core::visual::VisualParams* vparams)
{  
	float color = 1.0f;
	if ( getContext()->getShowCollisionModels())
	{
		glDisable(GL_LIGHTING);
		if (!isSimulated())
			glColor4f(1.0f, 1.0f, 1.0f, color);
		else
			glColor4f(1.0f, 1.0f, 0.0f, color);
	
			//if (color < 1.0f)
			//{
			//	glEnable(GL_BLEND);
			//	glDepthMask(0);
			//}

		int numnodes = 1;
		int node = 0;
		for (int level = 0; level < levels; level++ ) 
		{	
			if ( level < levels-1 ) {
				glColor4f(1.0f, color, color*color, (1.0 - color) );
				color *= 0.5f;
			}
			else glColor4f(0.0f, 0.0f, 1.0f, 1.0); 

			for (int i = 0; i < numnodes; i++) 
			{	
				draw(vparams,node );
				node++;	
			} 
			numnodes *= degree;
			

		} //end for levels

			//if (color < 1.0f)
			//{
			//	glDisable(GL_BLEND);
			//	glDepthMask(1);
			//}

	}
	if (isActive() && getPrevious()!=NULL && getContext()->getShowBoundingCollisionModels())
		getPrevious()->draw(vparams);
}

void SphereTreeModel::computeBoundingTree(int /*maxDepth*/)
{
	// To do:
	// The tree is obtained from an off line proccess. T
	// This function should be used to update the tree in case of deformable bodies.
}

void SphereTreeModel::computeContinuousBoundingTree(double /*dt*/, int /*maxDepth*/)
{	// To do
	sout << "SphereTreeModel::computeContinuousBoundingTree - not implemented" << endl;
}

std::pair<core::CollisionElementIterator,core::CollisionElementIterator> SphereTreeModel::getInternalChildren(int index) const
{
    //return elems[index].subcells;
    int firstchild = index*degree+1;
    if (firstchild >= size)
        return std::make_pair(core::CollisionElementIterator(),core::CollisionElementIterator());
    int lastchild = firstchild+degree-1;
    if (lastchild >= size) lastchild = size-1; // note that this cannot happen if the last level of the tree is full
    return std::make_pair(core::CollisionElementIterator(const_cast<SphereTreeModel*>(this),firstchild),core::CollisionElementIterator(const_cast<SphereTreeModel*>(this),lastchild+1));
}

bool SphereTreeModel::isLeaf( int index ) const
{
    int firstchild = index*degree+1;
    return (firstchild >= size);
    //return !elems[index].subcells.first.valid();
    //core::CollisionElementIterator i1 = elems[index].leaf;
    //if ( i1.valid() ) return true;
    //else return false;
}


} // namespace collision

} // namespace component

} // namespace sofa


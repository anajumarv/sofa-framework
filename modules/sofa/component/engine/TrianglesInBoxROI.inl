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
#ifndef SOFA_COMPONENT_ENGINE_TRIANGLESINBOXROI_INL
#define SOFA_COMPONENT_ENGINE_TRIANGLESINBOXROI_INL

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/component/engine/TrianglesInBoxROI.h>
#include <sofa/helper/gl/template.h>
#include <sofa/helper/gl/BasicShapes.h>

namespace sofa
{

namespace component
{

namespace engine
{

using namespace sofa::helper;
using namespace sofa::defaulttype;
using namespace core::objectmodel;

template <class DataTypes>
TrianglesInBoxROI<DataTypes>::TrianglesInBoxROI()
: isVisible( initData (&isVisible, bool (true), "isVisible", "is Visible ?") ),
  boxes( initData(&boxes, "box", "Box defined by xmin,ymin,zmin, xmax,ymax,zmax") )
, f_X0( initData (&f_X0, "rest_position", "Rest position coordinates of the degrees of freedom") )
, f_triangles(initData(&f_triangles, "triangles", "List of triangle indices"))
, f_indices( initData(&f_indices,"indices","Indices of the triangles contained in the ROI") )
, _drawSize( initData(&_drawSize,0.0,"drawSize","rendering size for box and triangles") )
{
    boxes.beginEdit()->push_back(Vec6(0,0,0,1,1,1));
    boxes.endEdit();

    f_indices.beginEdit()->push_back(0);
    f_indices.endEdit();
}

template <class DataTypes>
void TrianglesInBoxROI<DataTypes>::init()
{
    if (!f_X0.isSet())
    {
	MechanicalState<DataTypes>* mstate;
	this->getContext()->get(mstate);
	if (mstate)
	{
	    BaseData* parent = mstate->findField("rest_position");
	    if (parent)
	    {
		f_X0.setParent(parent);
		f_X0.setReadOnly(true);
	    }
	}
    }
    if (!f_triangles.isSet())
    {
        BaseMeshTopology* topology = dynamic_cast<BaseMeshTopology*>(getContext()->getTopology());
        if (topology != NULL)
        {
            BaseData* parent = topology->findField("triangles");
            if (parent != NULL)
            {
                f_triangles.setParent(parent);
                f_triangles.setReadOnly(true);
            }
            else
            {
                sout << "ERROR: Topology " << topology->getName() << " does not contain triangles" << sendl;
            }
        }
        else
        {
            sout << "ERROR: Topology not found. Triangles in box can not be computed" << sendl;
        }
    }
    addInput(&f_X0);
    addInput(&f_triangles);
    addOutput(&f_indices);

    setDirtyValue();
}

template <class DataTypes>
void TrianglesInBoxROI<DataTypes>::reinit()
{
    update();
}

template <class DataTypes>
bool TrianglesInBoxROI<DataTypes>::containsTriangle(const Vec6& b, const BaseMeshTopology::Triangle& triangle)
{
    for (unsigned int i=0; i<triangle.size(); ++i)
    {
        Real x=0.0,y=0.0,z=0.0;
        DataTypes::get(x,y,z,(*x0)[triangle[i]]);
        if(!(x >= b[0] && x <= b[3] && y >= b[1] && y <= b[4] && z >= b[2] && z <= b[5]))
        {
            return false;
        }
    }
    return true;
}

template <class DataTypes>
void TrianglesInBoxROI<DataTypes>::update()
{
    cleanDirty();
    helper::vector<Vec6>& vb = *(boxes.beginEdit());

    for (unsigned int bi=0;bi<vb.size();++bi)
    {
        if (vb[bi][0] > vb[bi][3]) std::swap(vb[bi][0],vb[bi][3]);
        if (vb[bi][1] > vb[bi][4]) std::swap(vb[bi][1],vb[bi][4]);
        if (vb[bi][2] > vb[bi][5]) std::swap(vb[bi][2],vb[bi][5]);
    }

    boxes.endEdit();

    SetTriangle& indices = *(f_indices.beginEdit());
    indices.clear();

    x0 = &f_X0.getValue();

    const BaseMeshTopology::SeqTriangles* triangles = &f_triangles.getValue();

    for(unsigned int i=0; i<triangles->size(); ++i)
    {
        for (unsigned int bi=0;bi<vb.size();++bi)
        {
            const Vec6& b=vb[bi];
            if (containsTriangle(b, (*triangles)[i]))
            {
                indices.push_back(i);
            }
        }
    }

    f_indices.endEdit();
}

template <class DataTypes>
void TrianglesInBoxROI<DataTypes>::draw()
{
    if (!this->getContext()->getShowBehaviorModels() || !isVisible.getValue())
        return;

    if( _drawSize.getValue() > 0)
    {
	const BaseMeshTopology::SeqTriangles& triangles = f_triangles.getValue();
	const SetTriangle& indices = f_indices.getValue();
	const VecCoord& x = *x0;
	glDisable(GL_LIGHTING);
	glLineWidth((GLfloat)_drawSize.getValue());
	glColor4f(1,0,1,1);
        glBegin(GL_LINES);
	for(unsigned int i=0; i<indices.size(); ++i)
	{
	    for (unsigned int j=0;j<3;++j)
	    {
		helper::gl::glVertexT(x[triangles[indices[i]][j]]);
		helper::gl::glVertexT(x[triangles[indices[i]][(j+1)%3]]);
	    }
	}
        glEnd();
	glColor4f(0.75f,0,0.75f,1);
        ///draw the boxes
        glBegin(GL_LINES);
        const helper::vector<Vec6>& vb=boxes.getValue();
        for (unsigned int bi=0;bi<vb.size();++bi)
        {
            const Vec6& b=vb[bi];
            const Real& Xmin=b[0];
            const Real& Xmax=b[3];
            const Real& Ymin=b[1];
            const Real& Ymax=b[4];
            const Real& Zmin=b[2];
            const Real& Zmax=b[5];
            glVertex3d(Xmin,Ymin,Zmin);
            glVertex3d(Xmin,Ymin,Zmax);
            glVertex3d(Xmin,Ymin,Zmin);
            glVertex3d(Xmax,Ymin,Zmin);
            glVertex3d(Xmin,Ymin,Zmin);
            glVertex3d(Xmin,Ymax,Zmin);
            glVertex3d(Xmin,Ymax,Zmin);
            glVertex3d(Xmax,Ymax,Zmin);
            glVertex3d(Xmin,Ymax,Zmin);
            glVertex3d(Xmin,Ymax,Zmax);
            glVertex3d(Xmin,Ymax,Zmax);
            glVertex3d(Xmin,Ymin,Zmax);
            glVertex3d(Xmin,Ymin,Zmax);
            glVertex3d(Xmax,Ymin,Zmax);
            glVertex3d(Xmax,Ymin,Zmax);
            glVertex3d(Xmax,Ymax,Zmax);
            glVertex3d(Xmax,Ymin,Zmax);
            glVertex3d(Xmax,Ymin,Zmin);
            glVertex3d(Xmin,Ymax,Zmax);
            glVertex3d(Xmax,Ymax,Zmax);
            glVertex3d(Xmax,Ymax,Zmin);
            glVertex3d(Xmax,Ymin,Zmin);
            glVertex3d(Xmax,Ymax,Zmin);
            glVertex3d(Xmax,Ymax,Zmax);
        }
        glEnd();
	glLineWidth(1.0f);
    }
}

template <class DataTypes>
bool TrianglesInBoxROI<DataTypes>::addBBox(double* minBBox, double* maxBBox)
{
    const helper::vector<Vec6>& vb=boxes.getValue();
    for (unsigned int bi=0;bi<vb.size();++bi)
    {
        const Vec6& b=vb[bi];
        if (b[0] < minBBox[0]) minBBox[0] = b[0];
        if (b[1] < minBBox[1]) minBBox[1] = b[1];
        if (b[2] < minBBox[2]) minBBox[2] = b[2];
        if (b[3] > maxBBox[0]) maxBBox[0] = b[3];
        if (b[4] > maxBBox[1]) maxBBox[1] = b[4];
        if (b[5] > maxBBox[2]) maxBBox[2] = b[5];
    }
     return true;
}

} // namespace engine

} // namespace component

} // namespace sofa

#endif

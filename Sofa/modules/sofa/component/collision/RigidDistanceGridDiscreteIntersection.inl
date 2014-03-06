/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
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
#ifndef SOFA_COMPONENT_COLLISION_RIGIDDISTANCEGRIDDISCRETEINTERSECTION_INL
#define SOFA_COMPONENT_COLLISION_RIGIDDISTANCEGRIDDISCRETEINTERSECTION_INL
#include <sofa/helper/system/config.h>
#include <sofa/component/collision/RigidDistanceGridDiscreteIntersection.h>
#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/collision/Intersection.inl>
//#include <sofa/component/collision/ProximityIntersection.h>
#include <sofa/helper/proximity.h>
#include <iostream>
#include <algorithm>



namespace sofa
{

namespace component
{

namespace collision
{

using namespace sofa::defaulttype;
using namespace sofa::core::collision;


template<class T>
bool RigidDistanceGridDiscreteIntersection::testIntersection(RigidDistanceGridCollisionElement&, TSphere<T>&)
{
    return true;
}

template<class T>
int RigidDistanceGridDiscreteIntersection::computeIntersection(RigidDistanceGridCollisionElement& e1, TSphere<T>& e2, OutputVector* contacts)
{
    DistanceGrid* grid1 = e1.getGrid();
    bool useXForm = e1.isTransformed();
    const Vector3& t1 = e1.getTranslation();
    const Matrix3& r1 = e1.getRotation();

    const double d0 = e1.getProximity() + e2.getProximity() + intersection->getContactDistance() + e2.r();
    const SReal margin = 0.001f + (SReal)d0;

    Vector3 p2 = e2.center();
    DistanceGrid::Coord p1;

    if (useXForm)
    {
        p1 = r1.multTranspose(p2-t1);
    }
    else p1 = p2;

    if (!grid1->inBBox( p1, margin )) return 0;
    if (!grid1->inGrid( p1 ))
    {
        intersection->serr << "WARNING: margin less than "<<margin<<" in DistanceGrid "<<e1.getCollisionModel()->getName()<<intersection->sendl;
        return 0;
    }

    SReal d = grid1->interp(p1);
    if (d >= margin) return 0;

    Vector3 grad = grid1->grad(p1); // note that there are some redundant computations between interp() and grad()
    grad.normalize();

    //p1 -= grad * d; // push p1 back to the surface

    contacts->resize(contacts->size()+1);
    DetectionOutput *detection = &*(contacts->end()-1);
#ifdef DETECTIONOUTPUT_BARYCENTRICINFO
    detection->baryCoords[0] = p1;
    detection->baryCoords[1].clear();
#endif
    detection->normal = (useXForm) ? r1 * grad : grad; // normal in global space from p1's surface
    detection->value = d - d0;
    detection->elem.first = e1;
    detection->elem.second = e2;
    detection->id = e2.getIndex();
    detection->point[0] = Vector3(p1) - grad * d;
    detection->point[1] = e2.getContactPointByNormal( detection->normal );
    return 1;
}

} // namespace collision

} // namespace component

} // namespace sofa

#endif

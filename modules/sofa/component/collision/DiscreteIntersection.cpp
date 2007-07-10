/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
#include <sofa/helper/system/config.h>
#include <sofa/helper/FnDispatcher.inl>
#include <sofa/component/collision/DiscreteIntersection.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/componentmodel/collision/Intersection.inl>
#include <sofa/component/collision/RayPickInteractor.h>
#include <sofa/component/collision/ProximityIntersection.h>
#include <sofa/component/collision/proximity.h>
#include <iostream>
#include <algorithm>



namespace sofa
{

namespace component
{

namespace collision
{

using namespace sofa::defaulttype;
using namespace sofa::core::componentmodel::collision;

SOFA_DECL_CLASS(DiscreteIntersection)

int DiscreteIntersectionClass = core::RegisterObject("TODO-DiscreteIntersectionClass")
        .add< DiscreteIntersection >()
        ;


DiscreteIntersection::DiscreteIntersection()
{
    intersectors.add<CubeModel,       CubeModel,         DiscreteIntersection, false> (this);
    intersectors.add<SphereModel,     SphereModel,       DiscreteIntersection, false> (this);
    intersectors.add<SphereModel,     RayModel,          DiscreteIntersection, true>  (this);
    intersectors.add<SphereModel,     RayPickInteractor, DiscreteIntersection, true>  (this);
    intersectors.add<SphereTreeModel, RayPickInteractor, DiscreteIntersection, true>  (this);
    intersectors.add<SphereTreeModel, SphereTreeModel,   DiscreteIntersection, false> (this);
    intersectors.add<SphereTreeModel, CubeModel,         DiscreteIntersection, true>  (this);
    intersectors.add<SphereTreeModel, TriangleModel,     DiscreteIntersection, true>  (this);
    //intersectors.add<SphereTreeModel, SphereModel,       DiscreteIntersection, true>  (this);
    //intersectors.add<SphereModel,     TriangleModel,     DiscreteIntersection, true>  (this);
    //intersectors.add<TriangleModel,   TriangleModel,     DiscreteIntersection, false> (this);
    intersectors.add<RigidDistanceGridCollisionModel, RigidDistanceGridCollisionModel, DiscreteIntersection, false> (this);
    intersectors.add<RigidDistanceGridCollisionModel, PointModel,                      DiscreteIntersection, true>  (this);
    intersectors.add<RigidDistanceGridCollisionModel, TriangleModel,                   DiscreteIntersection, true>  (this);
    intersectors.add<RigidDistanceGridCollisionModel, RayModel,                        DiscreteIntersection, true>  (this);
    intersectors.add<RigidDistanceGridCollisionModel, RayPickInteractor,               DiscreteIntersection, true>  (this);
    intersectors.add<FFDDistanceGridCollisionModel,   RigidDistanceGridCollisionModel, DiscreteIntersection, true>  (this);
}

/// Return the intersector class handling the given pair of collision models, or NULL if not supported.
ElementIntersector* DiscreteIntersection::findIntersector(core::CollisionModel* object1, core::CollisionModel* object2)
{
    return intersectors.get(object1, object2);
}

bool DiscreteIntersection::testIntersection(Sphere& sph1, Sphere& sph2)
{
    //std::cout<<"Collision between Sphere - Sphere"<<std::endl;
    Vector3 sph1Pos(sph1.center());
    Vector3 sph2Pos(sph2.center());
    double radius1 = sph1.r(), radius2 = sph2.r();
    Vector3 tmp = sph1Pos - sph2Pos;

    return (tmp.norm2() < (radius1 + radius2) * (radius1 + radius2));
}

bool DiscreteIntersection::testIntersection(SingleSphere& sph1, SingleSphere& sph2)
{
    //std::cout<<"Collision between Sphere - Sphere"<<std::endl;
    Vector3 sph1Pos(sph1.center());
    Vector3 sph2Pos(sph2.center());

    double radius1 = sph1.r(), radius2 = sph2.r();
    Vector3 tmp = sph1Pos - sph2Pos;

    return (tmp.norm2() < (radius1 + radius2) * (radius1 + radius2));
}

bool DiscreteIntersection::testIntersection( SingleSphere& sph1, Cube& cube)
{
    // Values of the "aligned" bounding box
    Vector3 Bmin = cube.minVect();
    Vector3 Bmax = cube.maxVect();
    // Center of sphere
    Vector3 ctr(sph1.center());
    // Square of radius
    double r2 = sph1.r()*sph1.r();
    // Distance
    double dmin = 0;

    for ( int i = 0; i<3; i++)
    {
        if ( ctr[i] < Bmin[i] ) 	dmin += (ctr[i]-Bmin[i])*(ctr[i]-Bmin[i]);
        else if ( ctr[i] > Bmax[i] ) dmin += (ctr[i]-Bmax[i])*(ctr[i]-Bmax[i]);
    }

    if (dmin <= r2 ) return true;
    else return false;
}

bool DiscreteIntersection::testIntersection( SingleSphere& sph, Triangle& triangle)
{
    // todo
    double EPSILON = 0.00001;
    //Vertices of the triangle:
    Vector3 p0 = triangle.p1();
    Vector3 p1 = triangle.p2();
    Vector3 p2 = triangle.p3();

    // Center of the sphere
    const Vector3 sphCenter(sph.center());
    // Radius of the sphere
    const double r = sph.r();

    //Normal to the plane (plane spanned by tree points of the triangle)
    Vector3 normal = cross( (p1 - p0), (p2 - p0) );
    normal.normalize();

    //Distance from the center of the sphere to the plane.
    double distance = sphCenter*normal - normal*p0;

    //Projection of the center of the sphere onto the plane
    Vector3 projPoint = sphCenter - normal*distance;

    //Distance correction in case is negative.
    if (distance < 0.0)
        distance = -distance;

    //Distance to the sphere:
    distance -= r;

    //If the distance is positive, the point has been proyected outside
    //the sphere and hence the plane does not intersect the sphere
    //and so the triangle (that spanned the plane) cannot be inside the sphere.
    if (distance  > EPSILON)
    {
        return false;
    }

    //However, if the plane has intersected the sphere, then it is
    //neccesary to check if the proyected point "projPoint" is inside
    //the triangle.
#define SAMESIDE(ap1,ap2,ap3,ap4) (((cross((ap4-ap3),(ap1-ap3))) * (cross((ap4-ap3),(ap2-ap3)))) >= 0)
    if ( (SAMESIDE(projPoint,p0,p1,p2) && SAMESIDE(projPoint,p1,p0,p2) && SAMESIDE(projPoint,p2,p0,p1)))
    {
        return true;
    }
#undef SAMESIDE
    return false;
}

bool DiscreteIntersection::testIntersection(Cube& cube1, Cube& cube2)
{
    const Vector3& minVect1 = cube1.minVect();
    const Vector3& minVect2 = cube2.minVect();
    const Vector3& maxVect1 = cube1.maxVect();
    const Vector3& maxVect2 = cube2.maxVect();

    for (int i=0; i<3; i++)
    {
        if (minVect1[i] > maxVect2[i] || minVect2[i] > maxVect1[i])
            return false;
    }

    //std::cout << "Box <"<<minVect1[0]<<","<<minVect1[1]<<","<<minVect1[2]<<">-<"<<maxVect1[0]<<","<<maxVect1[1]<<","<<maxVect1[2]
    //  <<"> collide with Box "<<minVect2[0]<<","<<minVect2[1]<<","<<minVect2[2]<<">-<"<<maxVect2[0]<<","<<maxVect2[1]<<","<<maxVect2[2]<<">"<<std::endl;
    return true;
}

bool DiscreteIntersection::testIntersection(Sphere& sph1, Ray& ray2)
{
    //std::cout<<"intersectionSphereRay: Collision between Sphere - Ray"<<std::endl;
    // Center of the sphere
    const Vector3 sph1Pos(sph1.center());
    // Radius of the sphere
    const double radius1 = sph1.r();

    const Vector3 ray2Origin(ray2.origin());
    const Vector3 ray2Direction(ray2.direction());
    const double length2 = ray2.l();
    const Vector3 tmp = sph1Pos - ray2Origin;
    const double rayPos = tmp*ray2Direction;
    const double rayPosInside = std::max(std::min(rayPos,length2),0.0);
    const double dist2 = tmp.norm2() - (rayPosInside*rayPosInside);
    return (dist2 < (radius1*radius1));
}


bool DiscreteIntersection::testIntersection(SingleSphere& sph1, Ray& ray2)
{
    // Center of the sphere
    const Vector3 sph1Pos(sph1.center());
    // Radius of the sphere
    const double radius1 = sph1.r();

    const Vector3 ray2Origin(ray2.origin());
    const Vector3 ray2Direction(ray2.direction());
    const double length2 = ray2.l();
    const Vector3 tmp = sph1Pos - ray2Origin;
    const double rayPos = tmp*ray2Direction;
    const double rayPosInside = std::max(std::min(rayPos,length2),0.0);
    const double dist2 = tmp.norm2() - (rayPosInside*rayPosInside);
    return (dist2 < (radius1*radius1));
}


//bool DiscreteIntersection::testIntersection(Sphere&, Triangle&)
//{
//	std::cout<<"Collision between Sphere - Triangle"<<std::endl;
//	return false;
//}

//bool DiscreteIntersection::testIntersection(Triangle& t1, Triangle& t2)
//{
//	std::cout<<"Collision between Triangle - Triangle"<<std::endl;
//	return false;
//}

int DiscreteIntersection::computeIntersection(Cube&, Cube&, DetectionOutputVector&)
{
    return 0; /// \todo
}

int DiscreteIntersection::computeIntersection( SingleSphere& sph, Triangle& triangle, DetectionOutputVector& contacts)
{
    double EPSILON = 0.00001;
    //Vertices of the triangle:
    Vector3 p0 = triangle.p1();
    Vector3 p1 = triangle.p2();
    Vector3 p2 = triangle.p3();

    // Center of the sphere
    const Vector3 sphCenter(sph.center());
    // Radius of the sphere
    const double r = sph.r();

    //Normal to the plane (plane spanned by tree points of the triangle)
    Vector3 normal = cross( (p1 - p0), (p2 - p0) );
    normal.normalize();

    //Distance from the center of the sphere to the plane.
    double distance = sphCenter*normal - normal*p0;

    //Projection of the center of the sphere onto the plane
    Vector3 projPoint = sphCenter - normal*distance;

    //Distance correction in case is negative.
    if (distance < 0.0)
        distance = -distance;

    //Distance to the sphere:
    distance -= r;

    //If the distance is positive, the point has been proyected outside
    //the sphere and hence the plane does not intersect the sphere
    //and so the triangle (that spanned the plane) cannot be inside the sphere.
    if (distance  > EPSILON)
    {
        return 0;
    }

    //However, if the plane has intersected the sphere, then it is
    //neccesary to check if the proyected point "projPoint" is inside
    //the triangle.
#define SAMESIDE(ap1,ap2,ap3,ap4) (((cross((ap4-ap3),(ap1-ap3))) * (cross((ap4-ap3),(ap2-ap3)))) >= 0)
    if ( (SAMESIDE(projPoint,p0,p1,p2) && SAMESIDE(projPoint,p1,p0,p2) && SAMESIDE(projPoint,p2,p0,p1)))
    {
        contacts.resize(contacts.size()+1);
        DetectionOutput *detection = &*(contacts.end()-1);
        detection->normal = -normal;
        detection->point[1] = projPoint;
        detection->point[0] = sph.center() - detection->normal * sph.r();

        detection->distance = -distance;
        //detection->elem.first = triangle;
        //detection->elem.second = sph;
        detection->elem.first = sph;
        detection->elem.second = triangle;
        detection->id = sph.getIndex();
        return 1;
    }
#undef SAMESIDE

    //// The projected sphere center is not in the triangle. Verify if
    //// the edges are colliding the sphere (check if they are secant to the sphere)
    // RayModel edges;
    ////Edge 0
    // Vector3 dir = p1 - p0;
    // double length = dir.norm();
    // edges.addRay(p0,dir,length);
    ////Edge1
    // dir = p1 - p2;
    // length = dir.norm();
    // edges.addRay(p1,dir,length);
    // //Edge2
    // dir = p2 - p0;
    // length = dir.norm();
    // edges.addRay(p2,dir,length);
    //
    // detection = distCorrectionSingleSphereRay( sph,edges.getRay(0));
    //if ( detection != NULL )
    //{
    //	detection->elem.first = triangle;
    //	detection->elem.second = sph;
    //	return detection;
    //}

    //detection = distCorrectionSingleSphereRay( sph,edges.getRay(1));
    //if ( detection != NULL )
    //{
    //	detection->elem.first = triangle;
    //	detection->elem.second = sph;
    //	return detection;
    //}
    // detection = distCorrectionSingleSphereRay( sph,edges.getRay(2));
    //	if ( detection != NULL )
    //{
    //	detection->elem.first = triangle;
    //	detection->elem.second = sph;
    //	return detection;
    //}

    return 0; // No intersection: passed all tests for intersections !
}


int DiscreteIntersection::computeIntersection(Sphere& sph1, Sphere& sph2, DetectionOutputVector& contacts)
{
    double r = sph1.r() + sph2.r();
    Vector3 dist = sph2.center() - sph1.center();

    if (dist.norm2() >= r*r)
        return 0;

    contacts.resize(contacts.size()+1);
    DetectionOutput *detection = &*(contacts.end()-1);
    detection->normal = dist;
    double distSph1Sph2 = detection->normal.norm();
    detection->normal /= distSph1Sph2;
    detection->point[0] = sph1.center() + detection->normal * sph1.r();
    detection->point[1] = sph2.center() - detection->normal * sph2.r();

    detection->distance = distSph1Sph2 - r;
    detection->elem.first = sph1;
    detection->elem.second = sph2;
    detection->id = (sph1.getCollisionModel()->getSize() > sph2.getCollisionModel()->getSize()) ? sph1.getIndex() : sph2.getIndex();

    return 1;
}

int DiscreteIntersection::computeIntersection(SingleSphere& sph1, SingleSphere& sph2, DetectionOutputVector& contacts)
{
    double r = sph1.r() + sph2.r();
    Vector3 dist = sph2.center() - sph1.center();

    if (dist.norm2() >= r*r)
        return 0;

    contacts.resize(contacts.size()+1);
    DetectionOutput *detection = &*(contacts.end()-1);
    detection->normal = dist;
    double distSph1Sph2 = detection->normal.norm();
    detection->normal /= distSph1Sph2;
    detection->point[0] = sph1.center() + detection->normal * sph1.r();
    detection->point[1] = sph2.center() - detection->normal * sph2.r();

    detection->distance = distSph1Sph2 - r;
    detection->elem.first = sph1;
    detection->elem.second = sph2;
    detection->id = (sph1.getCollisionModel()->getSize() > sph2.getCollisionModel()->getSize()) ? sph1.getIndex() : sph2.getIndex();

    return 1;
}

int DiscreteIntersection::computeIntersection(SingleSphere& /*sph1*/, Cube& /*cube*/, DetectionOutputVector& /*contacts*/)
{
    //to do
    return 0;
}


int DiscreteIntersection::computeIntersection(Sphere& sph1, Ray& ray2, DetectionOutputVector& contacts)
{
    // Center of the sphere
    const Vector3 sph1Pos(sph1.center());
    // Radius of the sphere
    const double radius1 = sph1.r();

    const Vector3 ray2Origin(ray2.origin());
    const Vector3 ray2Direction(ray2.direction());
    const double length2 = ray2.l();
    const Vector3 tmp = sph1Pos - ray2Origin;
    const double rayPos = tmp*ray2Direction;
    const double rayPosInside = std::max(std::min(rayPos,length2),0.0);
    const double dist2 = tmp.norm2() - (rayPosInside*rayPosInside);
    if (dist2 >= (radius1*radius1))
        return 0;

    const double dist = sqrt(dist2);

    contacts.resize(contacts.size()+1);
    DetectionOutput *detection = &*(contacts.end()-1);

    detection->point[1] = ray2Origin + ray2Direction*rayPosInside;
    detection->normal = detection->point[1] - sph1Pos;
    detection->normal /= dist;
    detection->point[0] = sph1Pos + detection->normal * radius1;
    detection->distance = dist - radius1;
    detection->elem.first = sph1;
    detection->elem.second = ray2;
    detection->id = ray2.getIndex();

    return 1;
}

int DiscreteIntersection::computeIntersection(SingleSphere& sph1, Ray& ray2, DetectionOutputVector& contacts)
{
    // Center of the sphere
    const Vector3 sph1Pos(sph1.center());
    // Radius of the sphere
    const double radius1 = sph1.r();

    const Vector3 ray2Origin(ray2.origin());
    const Vector3 ray2Direction(ray2.direction());
    const double length2 = ray2.l();
    const Vector3 tmp = sph1Pos - ray2Origin;
    const double rayPos = tmp*ray2Direction;
    const double rayPosInside = std::max(std::min(rayPos,length2),0.0);
    const double dist2 = tmp.norm2() - (rayPosInside*rayPosInside);
    if (dist2 >= (radius1*radius1))
        return 0;

    const double dist = sqrt(dist2);

    contacts.resize(contacts.size()+1);
    DetectionOutput *detection = &*(contacts.end()-1);

    detection->point[1] = ray2Origin + ray2Direction*rayPosInside;
    detection->normal = detection->point[1] - sph1Pos;
    detection->normal /= dist;
    detection->point[0] = sph1Pos + detection->normal * radius1;
    detection->distance = dist - radius1;
    detection->elem.first = sph1;
    detection->elem.second = ray2;
    detection->id = ray2.getIndex();

    return 1;
}
//int DiscreteIntersection::computeIntersection(Sphere&, Triangle&, DetectionOutputVector&)
//{
//	std::cout<<"Distance correction between Sphere - Triangle"<<std::endl;
//	return 0;
//}

//int DiscreteIntersection::computeIntersection(Triangle&, Triangle&, DetectionOutputVector&)
//{
//	std::cout<<"Distance correction between Triangle - Triangle"<<std::endl;
//	return 0;
//}

bool DiscreteIntersection::testIntersection(RigidDistanceGridCollisionElement&, RigidDistanceGridCollisionElement&)
{
    return true;
}

//#define DEBUG_XFORM

int DiscreteIntersection::computeIntersection(RigidDistanceGridCollisionElement& e1, RigidDistanceGridCollisionElement& e2, DetectionOutputVector& contacts)
{
    int nc = 0;
    DistanceGrid* grid1 = e1.getGrid();
    DistanceGrid* grid2 = e2.getGrid();
    bool useXForm = e1.isTransformed() || e2.isTransformed();
    const Vector3& t1 = e1.getTranslation();
    const Matrix3& r1 = e1.getRotation();
    const Vector3& t2 = e2.getTranslation();
    const Matrix3& r2 = e2.getRotation();

    const DistanceGrid::Real margin = 0.001f; //e1.getProximity() + e2.getProximity();

    // transform from grid1 to grid2
    Vec3f translation;
    Mat3x3f rotation;

    if (useXForm)
    {
        // p = t1+r1*p1 = t2+r2*p2
        // r2*p2 = t1-t2+r1*p1
        // p2 = r2t*(t1-p2) + r2t*r1*p1
        translation = r2.multTranspose(t1-t2);
        rotation = r2.multTranspose ( r1 );
    }
    else rotation.identity();

    // first points of e1 against distance field of e2
    const DistanceGrid::VecCoord& x1 = grid1->meshPts;
    if (!x1.empty() && e1.getCollisionModel()->usePoints.getValue())
    {
        if (grid2->isCube() && grid1->isCube())
        {
            const DistanceGrid::Real cubeDim2 = grid2->getCubeDim();
            const DistanceGrid::Real cubeDim2Margin = cubeDim2+margin;
            // translation is the position of cube1 center in cube2 space
            // we use its largest component as the dominant contact face normal
            /// \TODO use the relative velocity as an additionnal factor
            Vector3 normal = translation;
            normal[2] *= 1.1f; // we like Z contact better ;)
            if (rabs(normal[0]) > rabs(normal[1]))
            {
                if (rabs(normal[0]) > rabs(normal[2]))
                    normal = Vector3(normal[0]>0.0f?1.0f:-1.0f,0.0f,0.0f);
                else
                    normal = Vector3(0.0f,0.0f,normal[2]>0.0f?1.0f:-1.0f);
            }
            else
            {
                if (rabs(normal[1]) > rabs(normal[2]))
                    normal = Vector3(0.0f,normal[1]>0.0f?1.0f:-1.0f,0.0f);
                else
                    normal = Vector3(0.0f,0.0f,normal[2]>0.0f?1.0f:-1.0f);
            }

            Vector3 gnormal = r2 * -normal; // normal in global space from p1's surface
            // special case: if normal in global frame is nearly vertical, make it so
            if (gnormal[2] < -0.99f) gnormal = Vector3(0.0f, 0.0f, -1.0f);
            else if (gnormal[2] >  0.99f) gnormal = Vector3(0.0f, 0.0f,  1.0f);

            for (unsigned int i=0; i<x1.size(); i++)
            {
                DistanceGrid::Coord p1 = x1[i];
                DistanceGrid::Coord p2 = translation + rotation*p1;
                if (p2[0] < -cubeDim2Margin || p2[0] > cubeDim2Margin ||
                    p2[1] < -cubeDim2Margin || p2[1] > cubeDim2Margin ||
                    p2[2] < -cubeDim2Margin || p2[2] > cubeDim2Margin)
                    continue;
                double d = p2*normal - cubeDim2;

                //p2 -= normal * d; // push p2 to the surface

                contacts.resize(contacts.size()+1);
                DetectionOutput *detection = &*(contacts.end()-1);

                detection->point[0] = Vector3(p1);
                detection->point[1] = Vector3(p2) - normal*d;
                detection->normal = gnormal;
                detection->distance = d;
                detection->elem.first = e1;
                detection->elem.second = e2;
                detection->id = i;
                ++nc;
            }
        }
        else
        {
            for (unsigned int i=0; i<x1.size(); i++)
            {
                DistanceGrid::Coord p1 = x1[i];
                DistanceGrid::Coord p2 = translation + rotation*p1;
#ifdef DEBUG_XFORM
                DistanceGrid::Coord p1b = rotation.multTranspose(p2-translation);
                DistanceGrid::Coord gp1 = t1+r1*p1;
                DistanceGrid::Coord gp2 = t2+r2*p2;
                if ((p1b-p1).norm2() > 0.0001f)
                    std::cerr << "ERROR1a: " << p1 << " -> " << p2 << " -> " << p1b << std::endl;
                if ((gp1-gp2).norm2() > 0.0001f)
                    std::cerr << "ERROR1b: " << p1 << " -> " << gp1 << "    " << p2 << " -> " << gp2 << std::endl;
#endif

                if (!grid2->inBBox( p2, margin )) continue;

                float d = grid2->interp(p2);
                if (d >= margin) continue;

                Vector3 grad = grid2->grad(p2); // note that there are some redundant computations between interp() and grad()
                grad.normalize();

                //p2 -= grad * d; // push p2 back to the surface

                contacts.resize(contacts.size()+1);
                DetectionOutput *detection = &*(contacts.end()-1);

                detection->point[0] = Vector3(p1);
                detection->point[1] = Vector3(p2) - grad * d;
                detection->normal = r2 * -grad; // normal in global space from p1's surface
                detection->distance = d;
                detection->elem.first = e1;
                detection->elem.second = e2;
                detection->id = i;
                ++nc;
            }
        }
    }

    // then points of e2 against distance field of e1
    const DistanceGrid::VecCoord& x2 = grid2->meshPts;
    const int i0 = x1.size();
    if (!x2.empty() && e2.getCollisionModel()->usePoints.getValue())
    {
        if (grid1->isCube() && grid2->isCube())
        {
            const DistanceGrid::Real cubeDim1 = grid1->getCubeDim();
            const DistanceGrid::Real cubeDim1Margin = cubeDim1+margin;
            // -rotationT*translation is the position of cube2 center in cube1 space
            // we use its largest component as the dominant contact face normal
            /// \TODO use the relative velocity as an additionnal factor

            Vector3 normal = rotation.multTranspose(-translation);
            normal[2] *= 1.1f; // we like Z contact better ;)
            if (rabs(normal[0]) > rabs(normal[1]))
            {
                if (rabs(normal[0]) > rabs(normal[2]))
                    normal = Vector3(normal[0]>0.0f?1.0f:-1.0f,0.0f,0.0f);
                else
                    normal = Vector3(0.0f,0.0f,normal[2]>0.0f?1.0f:-1.0f);
            }
            else
            {
                if (rabs(normal[1]) > rabs(normal[2]))
                    normal = Vector3(0.0f,normal[1]>0.0f?1.0f:-1.0f,0.0f);
                else
                    normal = Vector3(0.0f,0.0f,normal[2]>0.0f?1.0f:-1.0f);
            }

            Vector3 gnormal = r1 * normal; // normal in global space from p1's surface
            // special case: if normal in global frame is nearly vertical, make it so
            if (gnormal[2] < -0.99f) gnormal = Vector3(0.0f, 0.0f, -1.0f);
            else if (gnormal[2] >  0.99f) gnormal = Vector3(0.0f, 0.0f,  1.0f);

            for (unsigned int i=0; i<x2.size(); i++)
            {
                DistanceGrid::Coord p2 = x2[i];
                DistanceGrid::Coord p1 = rotation.multTranspose(p2-translation);
                if (p1[0] < -cubeDim1Margin || p1[0] > cubeDim1Margin ||
                    p1[1] < -cubeDim1Margin || p1[1] > cubeDim1Margin ||
                    p1[2] < -cubeDim1Margin || p1[2] > cubeDim1Margin)
                    continue;
                double d = p1*normal - cubeDim1;

                //p1 -= normal * d; // push p1 to the surface

                contacts.resize(contacts.size()+1);
                DetectionOutput *detection = &*(contacts.end()-1);

                detection->point[0] = Vector3(p1) - normal * d;
                detection->point[1] = Vector3(p2);
                detection->normal = gnormal;
                detection->distance = d;
                detection->elem.first = e1;
                detection->elem.second = e2;
                detection->id = i0+i;
                ++nc;
            }
        }
        else
        {
            for (unsigned int i=0; i<x2.size(); i++)
            {
                DistanceGrid::Coord p2 = x2[i];
                DistanceGrid::Coord p1 = rotation.multTranspose(p2-translation);
#ifdef DEBUG_XFORM
                DistanceGrid::Coord p2b = translation + rotation*p1;
                DistanceGrid::Coord gp1 = t1+r1*p1;
                DistanceGrid::Coord gp2 = t2+r2*p2;
                if ((p2b-p2).norm2() > 0.0001f)
                    std::cerr << "ERROR2a: " << p2 << " -> " << p1 << " -> " << p2b << std::endl;
                else if ((gp1-gp2).norm2() > 0.0001f)
                    std::cerr << "ERROR2b: " << p1 << " -> " << gp1 << "    " << p2 << " -> " << gp2 << std::endl;
#endif

                if (!grid1->inBBox( p1, margin )) continue;

                float d = grid1->interp(p1);
                if (d >= margin) continue;

                Vector3 grad = grid1->grad(p1); // note that there are some redundant computations between interp() and grad()
                grad.normalize();

                //p1 -= grad * d; // push p1 back to the surface

                contacts.resize(contacts.size()+1);
                DetectionOutput *detection = &*(contacts.end()-1);

                detection->point[0] = Vector3(p1) - grad * d;
                detection->point[1] = Vector3(p2);
                detection->normal = r1 * grad; // normal in global space from p1's surface
                detection->distance = d;
                detection->elem.first = e1;
                detection->elem.second = e2;
                detection->id = i0+i;
                ++nc;
            }
        }
    }
    return nc;
}

bool DiscreteIntersection::testIntersection(RigidDistanceGridCollisionElement&, Point&)
{
    return true;
}

int DiscreteIntersection::computeIntersection(RigidDistanceGridCollisionElement& e1, Point& e2, DetectionOutputVector& contacts)
{
    DistanceGrid* grid1 = e1.getGrid();
    bool useXForm = e1.isTransformed();
    const Vector3& t1 = e1.getTranslation();
    const Matrix3& r1 = e1.getRotation();

    const DistanceGrid::Real margin = 0.001f; //e1.getProximity() + e2.getProximity();

    Vector3 p2 = e2.p();
    DistanceGrid::Coord p1;

    if (useXForm)
    {
        p1 = r1.multTranspose(p2-t1);
    }
    else p1 = p2;

    if (!grid1->inBBox( p1, margin )) return 0;

    float d = grid1->interp(p1);
    if (d >= margin) return 0;

    Vector3 grad = grid1->grad(p1); // note that there are some redundant computations between interp() and grad()
    grad.normalize();

    //p1 -= grad * d; // push p1 back to the surface

    contacts.resize(contacts.size()+1);
    DetectionOutput *detection = &*(contacts.end()-1);

    detection->point[0] = Vector3(p1) - grad * d;
    detection->point[1] = Vector3(p2);
    detection->normal = (useXForm) ? r1 * grad : grad; // normal in global space from p1's surface
    detection->distance = d;
    detection->elem.first = e1;
    detection->elem.second = e2;
    detection->id = e2.getIndex();
    return 1;
}

bool DiscreteIntersection::testIntersection(RigidDistanceGridCollisionElement&, Triangle&)
{
    return true;
}

int DiscreteIntersection::computeIntersection(RigidDistanceGridCollisionElement& e1, Triangle& e2, DetectionOutputVector& contacts)
{
    const int f2 = e2.flags();
    if (!(f2&TriangleModel::FLAG_POINTS)) return 0; // no points associated with this triangle
    DistanceGrid* grid1 = e1.getGrid();
    bool useXForm = e1.isTransformed();
    const Vector3& t1 = e1.getTranslation();
    const Matrix3& r1 = e1.getRotation();

    const DistanceGrid::Real margin = 0.001f; //e1.getProximity() + e2.getProximity();

    if (f2&TriangleModel::FLAG_P1)
    {
        Vector3 p2 = e2.p1();
        DistanceGrid::Coord p1;

        if (useXForm)
        {
            p1 = r1.multTranspose(p2-t1);
        }
        else p1 = p2;

        if (!grid1->inBBox( p1, margin )) return 0;

        float d = grid1->interp(p1);
        if (d >= margin) return 0;

        Vector3 grad = grid1->grad(p1); // note that there are some redundant computations between interp() and grad()
        grad.normalize();

        //p1 -= grad * d; // push p1 back to the surface

        contacts.resize(contacts.size()+1);
        DetectionOutput *detection = &*(contacts.end()-1);

        detection->point[0] = Vector3(p1) - grad * d;
        detection->point[1] = Vector3(p2);
        detection->normal = (useXForm) ? r1 * grad : grad; // normal in global space from p1's surface
        detection->distance = d;
        detection->elem.first = e1;
        detection->elem.second = e2;
        detection->id = e2.getIndex()*3+0;
    }

    if (f2&TriangleModel::FLAG_P2)
    {
        Vector3 p2 = e2.p2();
        DistanceGrid::Coord p1;

        if (useXForm)
        {
            p1 = r1.multTranspose(p2-t1);
        }
        else p1 = p2;

        if (!grid1->inBBox( p1, margin )) return 0;

        float d = grid1->interp(p1);
        if (d >= margin) return 0;

        Vector3 grad = grid1->grad(p1); // note that there are some redundant computations between interp() and grad()
        grad.normalize();

        //p1 -= grad * d; // push p1 back to the surface

        contacts.resize(contacts.size()+1);
        DetectionOutput *detection = &*(contacts.end()-1);

        detection->point[0] = Vector3(p1) - grad * d;
        detection->point[1] = Vector3(p2);
        detection->normal = (useXForm) ? r1 * grad : grad; // normal in global space from p1's surface
        detection->distance = d;
        detection->elem.first = e1;
        detection->elem.second = e2;
        detection->id = e2.getIndex()*3+1;
    }

    if (f2&TriangleModel::FLAG_P3)
    {
        Vector3 p2 = e2.p3();
        DistanceGrid::Coord p1;

        if (useXForm)
        {
            p1 = r1.multTranspose(p2-t1);
        }
        else p1 = p2;

        if (!grid1->inBBox( p1, margin )) return 0;

        float d = grid1->interp(p1);
        if (d >= margin) return 0;

        Vector3 grad = grid1->grad(p1); // note that there are some redundant computations between interp() and grad()
        grad.normalize();

        //p1 -= grad * d; // push p1 back to the surface

        contacts.resize(contacts.size()+1);
        DetectionOutput *detection = &*(contacts.end()-1);

        detection->point[0] = Vector3(p1) - grad * d;
        detection->point[1] = Vector3(p2);
        detection->normal = (useXForm) ? r1 * grad : grad; // normal in global space from p1's surface
        detection->distance = d;
        detection->elem.first = e1;
        detection->elem.second = e2;
        detection->id = e2.getIndex()*3+2;
    }
    return 1;
}

bool DiscreteIntersection::testIntersection(RigidDistanceGridCollisionElement& /*e1*/, Ray& /*e2*/)
{
    return true;
}

int DiscreteIntersection::computeIntersection(RigidDistanceGridCollisionElement& e1, Ray& e2, DetectionOutputVector& contacts)
{
    Vector3 rayOrigin(e2.origin());
    Vector3 rayDirection(e2.direction());
    const double rayLength = e2.l();

    int nc = 0;
    DistanceGrid* grid1 = e1.getGrid();
    bool useXForm = e1.isTransformed();

    if (useXForm)
    {
        const Vector3& t1 = e1.getTranslation();
        const Matrix3& r1 = e1.getRotation();
        rayOrigin = r1.multTranspose(rayOrigin-t1);
        rayDirection = r1.multTranspose(rayDirection);
        // now ray infos are in grid1 space
    }

    double l0 = 0;
    double l1 = rayLength;
    Vector3 r0 = rayOrigin;
    Vector3 r1 = rayOrigin + rayDirection*l1;

    DistanceGrid::Coord bbmin = grid1->getBBMin(), bbmax = grid1->getBBMax();
    // clip along each axis
    for (int c=0; c<3 && l1>l0; c++)
    {
        if (rayDirection[c] > 0)
        {
            // test if the ray is inside
            if (r1[c] < bbmin[c] || r0[c] > bbmax[c])
            { l1 = 0; break; }
            if (r0[c] < bbmin[c])
            {
                // intersect with p[c] == bbmin[c] plane
                double l = (bbmin[c]-rayOrigin[c]) / rayDirection[c];
                if(l0 < l)
                {
                    l0 = l;
                    r0 = rayOrigin + rayDirection*l0;
                }
            }
            if (r1[c] > bbmax[c])
            {
                // intersect with p[c] == bbmax[c] plane
                double l = (bbmax[c]-rayOrigin[c]) / rayDirection[c];
                if(l1 > l)
                {
                    l1 = l;
                    r1 = rayOrigin + rayDirection*l1;
                }
            }
        }
        else
        {
            // test if the ray is inside
            if (r0[c] < bbmin[c] || r1[c] > bbmax[c])
            { l1 = 0; break; }
            if (r0[c] > bbmax[c])
            {
                // intersect with p[c] == bbmax[c] plane
                double l = (bbmax[c]-rayOrigin[c]) / rayDirection[c];
                if(l0 < l)
                {
                    l0 = l;
                    r0 = rayOrigin + rayDirection*l0;
                }
            }
            if (r1[c] < bbmin[c])
            {
                // intersect with p[c] == bbmin[c] plane
                double l = (bbmin[c]-rayOrigin[c]) / rayDirection[c];
                if(l1 > l)
                {
                    l1 = l;
                    r1 = rayOrigin + rayDirection*l1;
                }
            }
        }

    }

    if (l0 < l1)
    {
        // some part of the ray is inside the grid
        Vector3 p = rayOrigin + rayDirection*l0;
        double dist = grid1->interp(p);
        double epsilon = grid1->getCellWidth().norm()*0.1f;
        while (l0 < l1 && (dist > 0 || dist < -epsilon))
        {
            l0 += dist;
            p = rayOrigin + rayDirection*l0;
            dist = grid1->interp(p);
        }
        if (dist < 0)
        {
            // intersection found

            contacts.resize(contacts.size()+1);
            DetectionOutput *detection = &*(contacts.end()-1);

            detection->point[0] = p;
            detection->point[1] = e2.origin() + e2.direction()*l0;
            detection->normal = -e2.direction(); // normal in global space from p1's surface
            detection->distance = dist;
            detection->elem.first = e1;
            detection->elem.second = e2;
            detection->id = e2.getIndex();
            ++nc;
        }
    }
    return nc;
}

bool DiscreteIntersection::testIntersection(FFDDistanceGridCollisionElement&, RigidDistanceGridCollisionElement&)
{
    return true;
}

//#define DEBUG_XFORM

int DiscreteIntersection::computeIntersection(FFDDistanceGridCollisionElement& e1, RigidDistanceGridCollisionElement& e2, DetectionOutputVector& contacts)
{
    int nc = 0;
    //DistanceGrid* grid1 = e1.getGrid();
    DistanceGrid* grid2 = e2.getGrid();
    vector<FFDDistanceGridCollisionModel::DeformedCube>& cubes1 = e1.getCollisionModel()->getDeformCubes(e1.getIndex());
    bool useXForm = e2.isTransformed();
    //const Vector3& t1 = e1.getTranslation();
    //const Matrix3& r1 = e1.getRotation();
    const Vector3& t2 = e2.getTranslation();
    const Matrix3& r2 = e2.getRotation();

    const DistanceGrid::Real margin = 0.001f; //e1.getProximity() + e2.getProximity();

    // transform from grid1 to grid2
    Vec3f translation;
    Mat3x3f rotation;

    if (useXForm)
    {
        translation = r2.multTranspose(-t2);
        rotation = r2; rotation.transpose();
    }
    else rotation.identity();

    // first points of e1 against distance field of e2
    if (e1.getCollisionModel()->usePoints.getValue())
    {
        for (unsigned int c=0; c<cubes1.size(); c++)
        {
            DistanceGrid::Coord p1 = cubes1[c].center;
            DistanceGrid::Coord p2 = translation + rotation*p1;
            if (!grid2->inBBox( p2, margin + cubes1[c].radius )) continue;
            cubes1[c].update();
            const std::vector<DistanceGrid::Coord>& x1 = cubes1[c].points;
            for (unsigned int i=0; i<x1.size(); i++)
            {
                p1 = x1[i];
                p2 = translation + rotation*p1;

                if (!grid2->inBBox( p2, margin )) continue;

                float d = grid2->interp(p2);
                if (d >= margin) continue;

                Vector3 grad = grid2->grad(p2); // note that there are some redundant computations between interp() and grad()
                grad.normalize();

                //p2 -= grad * d; // push p2 back to the surface

                contacts.resize(contacts.size()+1);
                DetectionOutput *detection = &*(contacts.end()-1);

                detection->point[0][0] = cubes1[c].initC0[0] + (cubes1[c].initC1[0]-cubes1[c].initC0[0])*cubes1[c].baryPoints[i][0];
                detection->point[0][1] = cubes1[c].initC0[1] + (cubes1[c].initC1[1]-cubes1[c].initC0[1])*cubes1[c].baryPoints[i][1];
                detection->point[0][2] = cubes1[c].initC0[2] + (cubes1[c].initC1[2]-cubes1[c].initC0[2])*cubes1[c].baryPoints[i][2];
                detection->point[1] = Vector3(p2) - grad * d;
                detection->normal = r2 * -grad; // normal in global space from p1's surface
                detection->distance = d;
                detection->elem.first = e1;
                detection->elem.second = e2;
                detection->id = i * cubes1.size() + c;
                ++nc;
            }
        }
    }

    // then points of e2 against distance field of e1

    const DistanceGrid::VecCoord& x2 = grid2->meshPts;
    if (!x2.empty() && e2.getCollisionModel()->usePoints.getValue())
    {
        // \TODO ...
    }
    return nc;
}


} // namespace collision

} // namespace component

} // namespace sofa


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
#ifndef SOFA_COMPONENT_MAPPING_IMPLICITSURFACEMAPPING_H
#define SOFA_COMPONENT_MAPPING_IMPLICITSURFACEMAPPING_H

#include <sofa/core/Mapping.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/component/topology/MeshTopology.h>
#include <vector>

namespace sofa
{

namespace component
{

namespace mapping
{

//using namespace core::componentmodel::behavior;

template <class In, class Out>
class ImplicitSurfaceMapping : public core::Mapping<In, Out>, public topology::MeshTopology
{
public:
    typedef core::Mapping<In, Out> Inherit;
    typedef typename Out::VecCoord OutVecCoord;
    typedef typename Out::VecDeriv OutVecDeriv;
    typedef typename Out::Coord OutCoord;
    typedef typename Out::Deriv OutDeriv;
    typedef typename OutCoord::value_type OutReal;
    typedef typename In::VecCoord InVecCoord;
    typedef typename In::VecDeriv InVecDeriv;
    typedef typename In::Coord InCoord;
    typedef typename In::Deriv InDeriv;
    typedef typename InCoord::value_type InReal;

    ImplicitSurfaceMapping(In* from, Out* to)
        : Inherit(from, to), mStep(0.5), mRadius(2.0), mIsoValue(0.5), mGridMin(-100,-100,-100), mGridMax(100, 100, 100)
    {
    }

    virtual ~ImplicitSurfaceMapping()
    {
    }

    virtual void parse(core::objectmodel::BaseObjectDescription* arg);

    double getStep() const { return mStep; }
    void setStep(double val) { mStep = val; }

    double getRadius() const { return mRadius; }
    void setRadius(double val) { mRadius = val; }

    double getIsoValue() const { return mIsoValue; }
    void setIsoValue(double val) { mIsoValue = val; }

    const InCoord& getGridMin() const { return mGridMin; }
    void setGridMin(const InCoord& val) { mGridMin = val; }
    void setGridMin(double x, double y, double z) { mGridMin = InCoord((InReal)x,(InReal)y,(InReal)z); }

    const InCoord& getGridMax() const { return mGridMax; }
    void setGridMax(const InCoord& val) { mGridMax = val; }
    void setGridMax(double x, double y, double z) { mGridMax = InCoord((InReal)x,(InReal)y,(InReal)z); }

    void apply( OutVecCoord& out, const InVecCoord& in );

    void applyJ( OutVecDeriv& out, const InVecDeriv& in );

    //void applyJT( InVecDeriv& out, const OutVecDeriv& in );
protected:
    double mStep;
    double mRadius;
    double mIsoValue;

    InCoord mGridMin;
    InCoord mGridMax;

    // Marching cube data

    /// For each cube, store the vertex indices on each 3 first edges, and the data value
    struct CubeData
    {
        int p[3];
        OutReal data;
    };

    std::vector<CubeData> planes;
    typename std::vector<CubeData>::iterator P0; /// Pointer to first plane
    typename std::vector<CubeData>::iterator P1; /// Pointer to second plane

    void newPlane();

    template<int C>
    int addPoint(OutVecCoord& out, int x,int y,int z, OutReal v0, OutReal v1, OutReal iso)
    {
        int p = out.size();
        OutCoord pos = OutCoord((OutReal)x,(OutReal)y,(OutReal)z);
        pos[C] -= (iso-v0)/(v1-v0);
        out.resize(p+1);
        out[p] = pos * mStep;
        return p;
    }

    int addFace(int p1, int p2, int p3, int nbp)
    {
        if ((unsigned)p1<(unsigned)nbp &&
            (unsigned)p2<(unsigned)nbp &&
            (unsigned)p3<(unsigned)nbp)
        {
            SeqTriangles& triangles = *seqTriangles.beginEdit();
            int f = triangles.size();
            triangles.push_back(Triangle(p1, p3, p2));
            seqTriangles.endEdit();
            return f;
        }
        else
        {
            std::cerr << "ERROR: Invalid face "<<p1<<" "<<p2<<" "<<p3<<std::endl;
            return -1;
        }
    }

};

// MARCHING CUBE TABLES
// ( table copied from an article of Paul Bourke )
// based on code by Cory Gene Bloyd

/* Convention:

         Z
         ^
         |
         4----4----5
        /|        /|
       7 |       5 |
      /  8      /  9
     7---+6----6   |
     |   |     |   |
     |   0----0+---1--> X
    11  /     10  /
     | 3       | 1
     |/        |/
     3----2----2
    /
   /
 |_
Y

*/

extern const int MarchingCubeEdgeTable[256];
extern const int MarchingCubeTriTable[256][16];

} // namespace mapping

} // namespace component

} // namespace sofa

#endif

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
#ifndef SOFA_DEFAULTTYPE_VEC3TYPES_H
#define SOFA_DEFAULTTYPE_VEC3TYPES_H

#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/vector.h>
#include <iostream>



namespace sofa
{

namespace defaulttype
{

using helper::vector;

template<class TCoord, class TDeriv, class TReal = typename TCoord::value_type>
class StdVectorTypes
{
public:
    typedef TCoord Coord;
    typedef TDeriv Deriv;
    typedef TReal Real;
    typedef vector<Coord> VecCoord;
    typedef vector<Deriv> VecDeriv;


    template <class T>
    class SparseData
    {
    public:
        SparseData(unsigned int _index, const T& _data): index(_index), data(_data) {};
        unsigned int index;
        T data;
    };

    typedef SparseData<Coord> SparseCoord;
    typedef SparseData<Deriv> SparseDeriv;

    typedef vector<SparseCoord> SparseVecCoord;
    typedef vector<SparseDeriv> SparseVecDeriv;

    //! All the Constraints applied to a state Vector
    typedef	vector<SparseVecDeriv> VecConst;

    static void set(Coord& c, double x, double y, double z)
    {
        c[0] = (typename Coord::value_type)x;
        c[1] = (typename Coord::value_type)y;
        c[2] = (typename Coord::value_type)z;
    }

    static void get(double& x, double& y, double& z, const Coord& c)
    {
        x = (double) c[0];
        y = (double) c[1];
        z = (double) c[2];
    }

    static void add(Coord& c, double x, double y, double z)
    {
        c[0] += (typename Coord::value_type)x;
        c[1] += (typename Coord::value_type)y;
        c[2] += (typename Coord::value_type)z;
    }

    static const char* Name();
};

template<class T>
class ExtVector
{
public:
    typedef T              value_type;
    typedef unsigned int   size_type;

protected:
    value_type* data;
    size_type   maxsize;
    size_type   cursize;

public:
    ExtVector() : data(NULL), maxsize(0), cursize(0) {}
    virtual ~ExtVector() {}
    void setData(value_type* d, size_type s) { data=d; maxsize=s; cursize=s; }
    value_type& operator[](size_type i) { return data[i]; }
    const value_type& operator[](size_type i) const { return data[i]; }
    size_type size() const { return cursize; }
    bool empty() const { return cursize==0; }
    virtual void resize(size_type size)
    {
        if (size <= maxsize)
            cursize = size;
        else
        {
            cursize = maxsize;
            std::cerr << "Error: invalide resize request ("<<size<<">"<<maxsize<<") on external vector.\n";
        }
    }
};

template<class TCoord, class TDeriv, class TReal = typename TCoord::value_type>
class ExtVectorTypes
{
public:
    typedef TCoord Coord;
    typedef TDeriv Deriv;
    typedef TReal Real;
    typedef ExtVector<Coord> VecCoord;
    typedef ExtVector<Deriv> VecDeriv;

    template <class T>
    class SparseData
    {
    public:
        SparseData(unsigned int _index, T& _data): index(_index), data(_data) {};
        unsigned int index;
        T data;
    };

    typedef SparseData<Coord> SparseCoord;
    typedef SparseData<Deriv> SparseDeriv;

    typedef vector<SparseCoord> SparseVecCoord;
    typedef vector<SparseDeriv> SparseVecDeriv;

    //! All the Constraints applied to a state Vector
    typedef	vector<SparseVecDeriv> VecConst;


    static void set(Coord& c, double x, double y, double z)
    {
        c[0] = (typename Coord::value_type)x;
        c[1] = (typename Coord::value_type)y;
        c[2] = (typename Coord::value_type)z;
    }

    static void get(double& x, double& y, double& z, const Coord& c)
    {
        x = (double) c[0];
        y = (double) c[1];
        z = (double) c[2];
    }

    static void add(Coord& c, double x, double y, double z)
    {
        c[0] += (typename Coord::value_type)x;
        c[1] += (typename Coord::value_type)y;
        c[2] += (typename Coord::value_type)z;
    }

    static const char* Name();
};

typedef StdVectorTypes<Vec3d,Vec3d,double> Vec3dTypes;
typedef StdVectorTypes<Vec3f,Vec3f,float> Vec3fTypes;
typedef Vec3dTypes Vec3Types;

template<>
inline const char* Vec3dTypes::Name()
{
    return "Vec3d";
}

template<>
inline const char* Vec3fTypes::Name()
{
    return "Vec3f";
}

typedef ExtVectorTypes<Vec3d,Vec3d,double> ExtVec3dTypes;
typedef ExtVectorTypes<Vec3f,Vec3f,float> ExtVec3fTypes;
typedef Vec3dTypes ExtVec3Types;

template<>
inline const char* ExtVec3dTypes::Name()
{
    return "ExtVec3d";
}

template<>
inline const char* ExtVec3fTypes::Name()
{
    return "ExtVec3f";
}

} // namespace defaulttype

} // namespace sofa

#endif

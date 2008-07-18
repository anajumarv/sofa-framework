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
#ifndef SOFA_COMPONENT_FORCEFIELD_TRIANGULARANISOTROPICFEMFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_TRIANGULARANISOTROPICFEMFORCEFIELD_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/componentmodel/behavior/ForceField.h>
#include <sofa/core/componentmodel/topology/BaseMeshTopology.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/component/forcefield/TriangularFEMForceField.h>
#include <sofa/component/topology/TriangleData.h>
#include <sofa/component/topology/EdgeData.h>
#include <sofa/component/topology/PointData.h>
#include <NewMAT/newmat.h>
#include <NewMAT/newmatap.h>



namespace sofa
{
namespace component
{
namespace forcefield
{

using namespace sofa::defaulttype;
using sofa::helper::vector;
using namespace sofa::component::topology;


template<class DataTypes>
class TriangularAnisotropicFEMForceField : public sofa::component::forcefield::TriangularFEMForceField<DataTypes>, public virtual core::objectmodel::BaseObject
{

public:

    typedef sofa::component::forcefield::TriangularFEMForceField<DataTypes> Inherited;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::VecReal VecReal;
    typedef VecCoord Vector;
    typedef typename DataTypes::Coord    Coord   ;
    typedef typename DataTypes::Deriv    Deriv   ;
    typedef typename Coord::value_type   Real    ;
    typedef typename Inherited::TriangleInformation   TriangleInformation  ;

    typedef sofa::core::componentmodel::topology::BaseMeshTopology::index_type Index;
    typedef sofa::core::componentmodel::topology::BaseMeshTopology::Triangle Element;
    typedef sofa::core::componentmodel::topology::BaseMeshTopology::SeqTriangles VecElement;

    static const int SMALL = 1;		///< Symbol of small displacements triangle solver
    static const int LARGE = 0;		///< Symbol of large displacements triangle solver

    void init();
    void reinit();
    void draw();

    TriangularAnisotropicFEMForceField();
    void computeMaterialStiffness(int i, Index& a, Index& b, Index& c);

    Data<Real> f_poisson2;
    Data<Real> f_young2;
    Data<Real> f_theta;

    VecDeriv fiberDirRefs;

    static void TRQSTriangleCreationFunction (int , void* , TriangleInformation &, const Triangle& , const sofa::helper::vector< unsigned int > &, const sofa::helper::vector< double >&);

};


} // namespace forcefield

} // namespace component

} // namespace sofa

#endif

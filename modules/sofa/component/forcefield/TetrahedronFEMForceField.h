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
#ifndef SOFA_COMPONENT_FORCEFIELD_TETRAHEDRONFEMFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_TETRAHEDRONFEMFORCEFIELD_H

#include <sofa/core/componentmodel/behavior/ForceField.h>
#include <sofa/component/MechanicalObject.h>
#include <sofa/core/VisualModel.h>
#include <sofa/component/topology/MeshTopology.h>
#include <sofa/component/topology/FittedRegularGridTopology.h>
#include <sofa/helper/vector.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>

namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::defaulttype;
using sofa::helper::vector;

/** Compute Finite Element forces based on tetrahedral elements.
*/
template<class DataTypes>
class TetrahedronFEMForceField : public core::componentmodel::behavior::ForceField<DataTypes>, public core::VisualModel
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef VecCoord Vector;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;

    typedef topology::MeshTopology::index_type Index;
    typedef topology::MeshTopology::Tetra Element;
    typedef topology::MeshTopology::SeqTetras VecElement;

    static const int SMALL = 0;   ///< Symbol of small displacements tetrahedron solver
    static const int LARGE = 1;   ///< Symbol of large displacements tetrahedron solver
    static const int POLAR = 2;   ///< Symbol of polar displacements tetrahedron solver

protected:
    //component::MechanicalObject<DataTypes>* object;

    typedef Vec<12, Real> Displacement;		///< the displacement vector

    typedef Mat<6, 6, Real> MaterialStiffness;	///< the matrix of material stiffness
    typedef vector<MaterialStiffness> VecMaterialStiffness;         ///< a vector of material stiffness matrices
    VecMaterialStiffness _materialsStiffnesses;					///< the material stiffness matrices vector

    typedef Mat<12, 6, Real> StrainDisplacement;	///< the strain-displacement matrix
    typedef vector<StrainDisplacement> VecStrainDisplacement;		///< a vector of strain-displacement matrices
    VecStrainDisplacement _strainDisplacements;					   ///< the strain-displacement matrices vector

    typedef Mat<3, 3, Real> Transformation; ///< matrix for rigid transformations like rotations


    typedef Mat<12, 12, Real> StiffnessMatrix;
    //typedef typename matrix<Real,rectangle<>,compressed<>,row_major >::type CompressedMatrix;
    //CompressedMatrix *_stiffnesses;


    typedef std::pair<int,Real> Col_Value;
    typedef vector< Col_Value > CompressedValue;
    typedef vector< CompressedValue > CompressedMatrix;
    CompressedMatrix _stiffnesses;
    double m_potentialEnergy;

    //just for draw forces
    VecDeriv _forces;

    topology::MeshTopology* _mesh;
    topology::FittedRegularGridTopology* _trimgrid;
    const VecElement *_indexedElements;
    VecCoord _initialPoints; ///< the intial positions of the points
    int _method; ///< the computation method of the displacements
    Real _poissonRatio;
    Real _youngModulus;
    Real _dampingRatio;
    bool _updateStiffnessMatrix;
    bool _assembling;

public:
    TetrahedronFEMForceField()
        : _mesh(NULL), _trimgrid(NULL)
        , _indexedElements(NULL)
        , _method(0)
        , _poissonRatio(0.45f)
        , _youngModulus(5000)
        , _dampingRatio(0)
        , _updateStiffnessMatrix(true)
        , _assembling(false)
    {
    }

    void parse(core::objectmodel::BaseObjectDescription* arg);

    void setPoissonRatio(Real val) { this->_poissonRatio = val; }

    void setYoungModulus(Real val) { this->_youngModulus = val; }

    void setMethod(int val) { this->_method = val; }

    void setUpdateStiffnessMatrix(bool val) { this->_updateStiffnessMatrix = val; }

    void setComputeGlobalMatrix(bool val) { this->_assembling= val; }

//	component::MechanicalObject<DataTypes>* getObject() { return object; }

    virtual void init();

    virtual void addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v);

    virtual void addDForce (VecDeriv& df, const VecDeriv& dx);

    virtual double getPotentialEnergy(const VecCoord& x);

    virtual void contributeToMatrixDimension(unsigned int * const, unsigned int * const);
    virtual void computeMatrix(sofa::defaulttype::SofaBaseMatrix *, double, double, double, unsigned int &);
    virtual void computeVector(sofa::defaulttype::SofaBaseVector *, unsigned int &);
    virtual void matResUpdatePosition(sofa::defaulttype::SofaBaseVector *, unsigned int &);


    // -- VisualModel interface
    void draw();
    void initTextures() { }
    void update() { }

protected:

    void computeStrainDisplacement( StrainDisplacement &J, Coord a, Coord b, Coord c, Coord d );
    Real peudo_determinant_for_coef ( const Mat<2, 3, Real>&  M );

    void computeStiffnessMatrix( StiffnessMatrix& S,StiffnessMatrix& SR,const MaterialStiffness &K, const StrainDisplacement &J, const Transformation& Rot );

    void computeMaterialStiffness(int i, Index&a, Index&b, Index&c, Index&d);

    void computeForce( Displacement &F, const Displacement &Depl, const MaterialStiffness &K, const StrainDisplacement &J );

////////////// small displacements method
    void initSmall(int i, Index&a, Index&b, Index&c, Index&d);
    void accumulateForceSmall( Vector& f, const Vector & p, typename VecElement::const_iterator elementIt, Index elementIndex );
    void accumulateDampingSmall( Vector& f, Index elementIndex );
    void applyStiffnessSmall( Vector& f, Real h, const Vector& x, int i=0, Index a=0,Index b=1,Index c=2,Index d=3  );

////////////// large displacements method
    vector<fixed_array<Coord,4> > _rotatedInitialElements;   ///< The initials positions in its frame
    vector<Transformation> _rotations;
    void initLarge(int i, Index&a, Index&b, Index&c, Index&d);
    void computeRotationLarge( Transformation &r, const Vector &p, const Index &a, const Index &b, const Index &c);
    void accumulateForceLarge( Vector& f, const Vector & p, typename VecElement::const_iterator elementIt, Index elementIndex );
    void accumulateDampingLarge( Vector& f, Index elementIndex );
    void applyStiffnessLarge( Vector& f, Real h, const Vector& x, int i=0, Index a=0,Index b=1,Index c=2,Index d=3 );

////////////// polar decomposition method
    vector<Transformation> _initialTransformation;
    void initPolar(int i, Index&a, Index&b, Index&c, Index&d);
    void accumulateForcePolar( Vector& f, const Vector & p, typename VecElement::const_iterator elementIt, Index elementIndex );
    void applyStiffnessPolar( Vector& f, Real h, const Vector& x, int i=0, Index a=0,Index b=1,Index c=2,Index d=3  );
};

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif

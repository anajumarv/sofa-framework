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
#ifndef SOFA_COMPONENT_MECHANICALOBJECT_H
#define SOFA_COMPONENT_MECHANICALOBJECT_H

#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/objectmodel/XField.h>
#include <sofa/core/objectmodel/VField.h>
#include <vector>
#include <assert.h>
#include <fstream>

namespace sofa
{

namespace component
{


using namespace core::componentmodel::behavior;
using namespace core::objectmodel;

/// This class can be overridden if needed for additionnal storage within template specializations.
template<class DataTypes>
class MechanicalObjectInternalData
{
public:
};

template <class DataTypes>
class MechanicalObject : public MechanicalState<DataTypes>
{
public:
    typedef MechanicalState<DataTypes> Inherited;
    typedef typename Inherited::VecId VecId;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::SparseDeriv SparseDeriv;
    typedef typename DataTypes::SparseVecDeriv SparseVecDeriv;
    typedef typename DataTypes::VecConst VecConst;

protected:
    VecCoord* x;
    VecDeriv* v;
    VecDeriv* f;
    VecDeriv* dx;
    VecCoord* x0;
    VecDeriv* v0;
    VecDeriv* internalForces;
    VecDeriv* externalForces;
    VecCoord* xfree; // stores the position of the mechanical objet after a free movement (p.e. gravity action)
    VecDeriv* vfree; // stores the velocity of the mechanical objet after a free movement (p.e. gravity action)

    // Constraints stored in the Mechanical State
    // The storage is a SparseMatrix
    // Each constraint (Type TConst) contains the index of the related DOF
    VecConst *c;

    double translation[3];
    double scale;

    /// @name Integration-related data
    /// @{

    std::vector< VecCoord * > vectorsCoord;
    std::vector< VecDeriv * > vectorsDeriv;
    int vsize; ///< Number of elements to allocate in vectors

    void setVecCoord(unsigned int index, VecCoord* v);
    void setVecDeriv(unsigned int index, VecDeriv* v);

    /// @}

    MechanicalObjectInternalData<DataTypes> data;

    std::ofstream* m_gnuplotFileX;
    std::ofstream* m_gnuplotFileV;

    class Loader;

public:

    MechanicalObject();
    MechanicalObject& operator = ( const MechanicalObject& );

    virtual ~MechanicalObject();

    virtual bool load(const char* filename);

    virtual void parse ( BaseObjectDescription* arg );
    virtual void parseFields ( const std::map<std::string,std::string*>& str );

    XField<DataTypes>* const f_X;
    VField<DataTypes>* const f_V;

    VecCoord* getX()  { f_X->beginEdit(); return x;  }
    VecDeriv* getV()  { f_V->beginEdit(); return v;  }
    VecDeriv* getF()  { return f;  }
    VecDeriv* getDx() { return dx; }
    VecConst* getC() { return c;}
    VecCoord* getXfree() { return xfree; }
    VecDeriv* getVfree()  { return vfree;  }

    const VecCoord* getX()  const { return x;  }
    const VecCoord* getX0()  const { return x0;  }
    const VecDeriv* getV()  const { return v;  }
    const VecDeriv* getV0()  const { return v0;  }
    const VecDeriv* getF()  const { return f;  }
    const VecDeriv* getDx() const { return dx; }
    const VecConst* getC() const { return c; }
    const VecCoord* getXfree() const { return xfree; }
    const VecDeriv* getVfree()  const { return vfree;  }

    virtual void init();

    virtual void reset();

    virtual void writeState( std::ostream& out );

    virtual void initGnuplot();
    virtual void exportGnuplot(double time);

    virtual void resize( int vsize);

    int getSize() const
    {
        return vsize;
    }

    /** \brief Overwrite values at index outputIndex by the ones at inputIndex.
     *
     */
    void replaceValue (const int inputIndex, const int outputIndex);



    /** \brief Exchange values at indices idx1 and idx2.
     *
     */
    void swapValues (const int idx1, const int idx2);



    /** \brief Reorder values according to parameter.
     *
     * Result of this method is :
     * newValue[ i ] = oldValue[ index[i] ];
     */
    void renumberValues( const std::vector<unsigned int> &index );



    /** \brief Replace the value at index by the sum of the ancestors values weithed by the coefs.
     *
     * Sum of the coefs should usually equal to 1.0
     */
    void computeWeightedValue( const unsigned int i, const std::vector< unsigned int >& ancestors, const std::vector< double >& coefs);



    void applyTranslation (double dx, double dy, double dz);

    void applyScale (double s);

    /// Get the indices of the particles located in the given bounding box
    void getIndicesInSpace(std::vector<unsigned>& indices, Real xmin, Real xmax, Real ymin, Real ymax, Real zmin, Real zmax) const;


    // new : get compliance on the constraints
    virtual void getCompliance(double dt, double **w, double *dfree, int &numContact);
    // apply contact force AND compute the subsequent dX
    virtual void applyContactForce(double *f);
    virtual void resetContactForce(void);

    virtual void addDxToCollisionModel(void);


    /// @name Integration related methods
    /// @{

    virtual void beginIntegration(double dt);

    virtual void endIntegration(double dt);

    virtual void accumulateForce();

    VecCoord* getVecCoord(unsigned int index);

    VecDeriv* getVecDeriv(unsigned int index);

    virtual void vAlloc(VecId v);

    virtual void vFree(VecId v);

    virtual void vOp(VecId v, VecId a = VecId::null(), VecId b = VecId::null(), double f=1.0);

    virtual double vDot(VecId a, VecId b);

    virtual void setX(VecId v);

    virtual void setXfree(VecId v);

    virtual void setV(VecId v);

    virtual void setF(VecId v);

    virtual void setDx(VecId v);

    virtual void setC(VecId v);

    virtual void resetForce();

    virtual void resetConstraint();

    /// @}

    /// @name Debug
    /// @{
    virtual void printDOF( VecId, std::ostream& =std::cerr );
    virtual unsigned printDOFWithElapsedTime(VecId, unsigned =0, unsigned =0, std::ostream& =std::cerr );
    /// @}
};

} // namespace component

} // namespace sofa

#endif

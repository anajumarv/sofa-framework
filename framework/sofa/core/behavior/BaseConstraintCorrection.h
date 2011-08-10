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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_CORE_BEHAVIOR_BASECONSTRAINTCORRECTION_H
#define SOFA_CORE_BEHAVIOR_BASECONSTRAINTCORRECTION_H

#include <sofa/core/objectmodel/BaseObject.h>

#include <sofa/core/ConstraintParams.h>

namespace sofa
{
namespace defaulttype
{
class BaseMatrix;
class BaseVector;
} // namespace defaulttype
} // namespace sofa

namespace sofa
{

namespace core
{

namespace behavior
{

/// @TODO All methods in this class need to be commented

/**
 *  \brief Component computing contact forces within a simulated body using the compliance method.
 */
class BaseConstraintCorrection : public virtual objectmodel::BaseObject
{
public:
    SOFA_CLASS(BaseConstraintCorrection, objectmodel::BaseObject);

    virtual ~BaseConstraintCorrection() {}

    /// @name Compliance Matrix API
    /// @{

    virtual void addComplianceInConstraintSpace(const ConstraintParams *, defaulttype::BaseMatrix* W) = 0;

    /// Fill the matrix m with the full Compliance Matrix
    virtual void getComplianceMatrix(defaulttype::BaseMatrix* m) const = 0;

    /// For multigrid approach => constraints are merged
    virtual void getComplianceWithConstraintMerge(defaulttype::BaseMatrix* /*Wmerged*/, std::vector<int> & /*constraint_merge*/)
    {
        sout << "getComplianceWithConstraintMerge is not implemented yet " << sendl;
    }

    virtual void CudaGetCompliance(defaulttype::BaseMatrix* W)
    {
        sout << "warning : CudaGetCompliance(defaulttype::BaseMatrix* W) is not implemented in " << this->getTypeName() << sendl;
        addComplianceInConstraintSpace(ConstraintParams::defaultInstance(), W); // par defaut si la methode cuda n'est pas implementé on resoud sur CPU
    }

    /// @}


    /// Compute motion correction from the constraint resolution (LCP) calculated force
    ///
    /// @param cparams
    /// @param x is the position result VecId
    /// @param v is the velocity result VecId
    /// @param f is the motion space force vector
    /// @param lambda is the constraint space force vector
    virtual void computeAndApplyMotionCorrection(const ConstraintParams * /*cparams*/, MultiVecCoordId /*x*/, MultiVecDerivId /*v*/, MultiVecDerivId /*f*/, const defaulttype::BaseVector * /*lambda*/) = 0;

    /// Compute position correction from the constraint resolution (LCP) calculated force
    ///
    /// @param cparams
    /// @param x is the position result VecId
    /// @param f is the motion space force vector
    /// @param lambda is the constraint space force vector
    virtual void computeAndApplyPositionCorrection(const ConstraintParams * /*cparams*/, MultiVecCoordId /*x*/, MultiVecDerivId /*f*/, const defaulttype::BaseVector * /*lambda*/) = 0;

    /// Compute velocity correction from the constraint resolution (LCP) calculated force
    ///
    /// @param cparams
    /// @param v is the velocity result VecId
    /// @param f is the motion space force vector
    /// @param lambda is the constraint space force vector
    virtual void computeAndApplyVelocityCorrection(const ConstraintParams * /*cparams*/, MultiVecDerivId /*v*/, MultiVecDerivId /*f*/, const defaulttype::BaseVector * /*lambda*/) = 0;

    /// Apply predictive constraint force
    ///
    /// @param cparams
    /// @param f is the motion space force vector
    /// @param lambda is the constraint space force vector
    virtual void applyPredictiveConstraintForce(const ConstraintParams * /*cparams*/, MultiVecDerivId /*f*/, const defaulttype::BaseVector * /*lambda*/) = 0;


    /// @name Deprecated API
    /// @{

    virtual void applyContactForce(const defaulttype::BaseVector *f) = 0;

    virtual void resetContactForce() = 0;

    /// @}


    /// @name Unbuilt constraint system during resolution
    /// @{

    virtual bool hasConstraintNumber(int /*index*/) {return true;}

    virtual void resetForUnbuiltResolution(double * /*f*/, std::list<unsigned int>& /*renumbering*/) {}

    virtual void addConstraintDisplacement(double * /*d*/, int /*begin*/, int /*end*/) {}

    virtual void setConstraintDForce(double * /*df*/, int /*begin*/, int /*end*/, bool /*update*/) {}	  // f += df

    virtual void getBlockDiagonalCompliance(defaulttype::BaseMatrix* /*W*/, int /*begin*/,int /*end*/)
    {
        sout << "warning : getBlockDiagonalCompliance(defaulttype::BaseMatrix* W) is not implemented in " << this->getTypeName() << sendl;
    }

    /// @}
};

} // namespace behavior

} // namespace core

} // namespace sofa

#endif

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
#ifndef SOFA_CORE_BEHAVIOR_MULTIMATRIXACCESSOR_H
#define SOFA_CORE_BEHAVIOR_MULTIMATRIXACCESSOR_H

#include <sofa/defaulttype/BaseMatrix.h>
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <sofa/core/behavior/BaseMechanicalMapping.h>

namespace sofa
{

namespace core
{

namespace behavior
{

/// Abstract class allowing to find the part of the mechanical matrix
/// associated with a given MechanicalState or pair of MechanicalState
/// (for interaction forces and mappings).
///
class SOFA_CORE_API MultiMatrixAccessor
{
public:
    virtual ~MultiMatrixAccessor();

    /// Simple structure holding a reference to the submatrix related to one MechanicalState
    class MatrixRef
    {
    public:
        defaulttype::BaseMatrix* matrix;
        unsigned int offset;
        MatrixRef() : matrix(NULL), offset(0) {}
        defaulttype::BaseMatrix* operator->() const { return matrix; }
        bool operator!() const { return matrix == NULL; }
        operator bool() const { return matrix != NULL; }
    };

    /// Simple structure holding a reference to the submatrix related to the interactions between two MechanicalStates
    class InteractionMatrixRef
    {
    public:
        defaulttype::BaseMatrix* matrix;
        unsigned int offRow, offCol;
        InteractionMatrixRef() : matrix(NULL), offRow(0), offCol(0) {}
        defaulttype::BaseMatrix* operator->() const { return matrix; }
        bool operator!() const { return matrix == NULL; }
        operator bool() const { return matrix != NULL; }
    };

    virtual void addMechanicalState(const BaseMechanicalState* mstate);
    virtual void addMechanicalMapping(const BaseMechanicalMapping* mapping);
    virtual void addMappedMechanicalState(const BaseMechanicalState* mstate);
    
    virtual int getGlobalDimension() const = 0;
    virtual int getGlobalOffset(const BaseMechanicalState* mstate) const = 0;

    virtual MatrixRef getMatrix(const BaseMechanicalState* mstate) const = 0;
    virtual InteractionMatrixRef getMatrix(const BaseMechanicalState* mstate1, const BaseMechanicalState* mstate2) const = 0;

};

} // namespace behavior

} // namespace core

} // namespace sofa

#endif

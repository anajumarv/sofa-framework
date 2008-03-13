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
#ifndef SOFA_CORE_OBJECTMODEL_VFIELD_H
#define SOFA_CORE_OBJECTMODEL_VFIELD_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/objectmodel/Field.h>

namespace sofa
{

namespace core
{

namespace objectmodel
{

/**
 *  \brief Access to the getV() vector of a MechanicalObject.
 *
 *  @author François Faure
 */
template<class DataTypes>
class VField : public Field<typename DataTypes::VecDeriv>
{
public:
    typedef typename DataTypes::VecDeriv VecDeriv;
    
    VField(VecDeriv** ptr, char* name)
    : Field<VecDeriv>(0,name)
    , m_vecPtr(ptr)
    {
    }

    ~VField()
    {}

    VecDeriv* beginEdit()
    {
        this->ptr = *m_vecPtr;
        //std::cerr<<"VField<DataTypes>::beginEdit()"<<std::endl;
        return this->Field<VecDeriv>::beginEdit();
    }

protected:
    VecDeriv** m_vecPtr; 
};

} // namespace objectmodel

} // namespace core

} // namespace sofa

#endif

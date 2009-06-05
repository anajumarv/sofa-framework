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
#ifndef SOFA_CORE_COMPONENTMODEL_BEHAVIOR_LMCONSTRAINT_INL
#define SOFA_CORE_COMPONENTMODEL_BEHAVIOR_LMCONSTRAINT_INL

#include <sofa/core/componentmodel/behavior/LMConstraint.h>
#include <sofa/core/objectmodel/BaseNode.h>

namespace sofa
{

namespace core
{

namespace componentmodel
{

namespace behavior
{

template<class DataTypes1,class DataTypes2>
LMConstraint<DataTypes1,DataTypes2>::~LMConstraint()
{
}

template<class DataTypes1,class DataTypes2>
void LMConstraint<DataTypes1,DataTypes2>::init()
{
    BaseLMConstraint::init();


    if (object1 != NULL && object2 != NULL)
    {
        //Constraint created by passing Mechanical State directly, need to find the name of the path to be able to save the scene eventually

        if (object1->getContext() != getContext())
        {
            sofa::core::objectmodel::BaseContext *context = NULL;
            sofa::core::objectmodel::BaseNode*    currentNode = dynamic_cast< sofa::core::objectmodel::BaseNode *>(object1->getContext());

            std::string object_name=currentNode->getPathName();
            if (context != NULL) this->pathObject1.setValue(object_name);
        }


        if (object2->getContext() != getContext())
        {
            sofa::core::objectmodel::BaseContext *context = NULL;
            sofa::core::objectmodel::BaseNode*    currentNode = dynamic_cast< sofa::core::objectmodel::BaseNode *>(object2->getContext());

            std::string object_name=currentNode->getPathName();
            if (context != NULL) this->pathObject2.setValue(object_name);
        }
    }
}


} // namespace behavior

} // namespace componentmodel

} // namespace core

} // namespace sofa

#endif

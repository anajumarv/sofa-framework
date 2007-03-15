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
#include <sofa/simulation/tree/xml/NodeElement.h>
#include <sofa/simulation/tree/xml/ObjectElement.h>
#include <sofa/simulation/tree/xml/Element.inl>

namespace sofa
{

namespace simulation
{

namespace tree
{

namespace xml
{

using namespace sofa::defaulttype;

//template class Factory< std::string, xml::BaseElement, Node<xml::BaseElement*>* >;

NodeElement::NodeElement(const std::string& name, const std::string& type, BaseElement* parent)
    : Element<core::objectmodel::BaseNode>(name, type, parent)
{
}

NodeElement::~NodeElement()
{
}

bool NodeElement::setParent(BaseElement* newParent)
{
    if (newParent != NULL && dynamic_cast<NodeElement*>(newParent)==NULL)
        return false;
    else
        return Element<core::objectmodel::BaseNode>::setParent(newParent);
}

bool NodeElement::initNode()
{
    if (!Element<core::objectmodel::BaseNode>::initNode()) return false;
    if (getObject()!=NULL && getParent()!=NULL && dynamic_cast<core::objectmodel::BaseNode*>(getParent()->getBaseObject())!=NULL)
    {
        std::cout << "Adding Child "<<getName()<<" to "<<getParent()->getName()<<std::endl;
        dynamic_cast<core::objectmodel::BaseNode*>(getParent()->getBaseObject())->addChild(getObject());
    }
    return true;
}

bool NodeElement::init()
{
    bool res = Element<core::objectmodel::BaseNode>::init();
    /*
    if (getObject()!=NULL)
    {
    	for (child_iterator<> it = begin();
    				it != end(); ++it)
    	{
    		objectmodel::BaseObject* obj = dynamic_cast<objectmodel::BaseObject*>(it->getBaseObject());
    		if (obj!=NULL)
    		{
    			std::cout << "Adding Object "<<it->getName()<<" to "<<getName()<<std::endl;
    			getObject()->addObject(obj);
    		}
    	}
    }
    */
    return res;
}

SOFA_DECL_CLASS(Node)

helper::Creator<BaseElement::NodeFactory, NodeElement> NodeNodeClass("Node");
helper::Creator<BaseElement::NodeFactory, NodeElement> NodeBodyClass("Body");
helper::Creator<BaseElement::NodeFactory, NodeElement> NodeGClass("G");

const char* NodeElement::getClass() const
{
    return NodeNodeClass.c_str();
}

} // namespace xml

} // namespace tree

} // namespace simulation

} // namespace sofa


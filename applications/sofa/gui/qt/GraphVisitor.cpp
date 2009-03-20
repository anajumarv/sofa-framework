/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "GraphVisitor.h"

#include <sstream>

#include <tinyxml.cpp>
#include <tinyxmlerror.cpp>
#include <tinystr.cpp>
#include <tinyxmlparser.cpp>

#include <algorithm>

#ifndef SOFA_QT4
typedef QListViewItem Q3ListViewItem;
#endif
namespace sofa
{

  namespace gui
  {

    namespace qt
    {

      bool cmpTime(const dataTime &a, const dataTime &b){ return a.time > b.time;};
      bool GraphVisitor::load(std::string &file)
      {	
	//Open it using TinyXML
	TiXmlDocument doc(file.c_str());
	bool loadOk = doc.LoadFile();
	if (!loadOk) return false;
	
	TiXmlHandle hDoc(&doc);
	TiXmlNode* pElem;
	//Getting the root of the file
	pElem=hDoc.FirstChildElement().Element();
	
	// should always have a valid root but handle gracefully if it does
	if (!pElem) return false;
	
        totalTime = getTotalTime(pElem);

        componentsTime.clear();
        visitorsTime.clear();
        

	openNode( pElem, NULL, NULL);


        std::sort(componentsTime.begin(),componentsTime.end(),cmpTime);
        std::sort(visitorsTime.begin(),visitorsTime.end(),cmpTime);

        std::sort(componentsTimeTotal.begin(),componentsTimeTotal.end(),cmpTime);
        std::sort(visitorsTimeTotal.begin(),visitorsTimeTotal.end(),cmpTime);


        if (totalTimeMax<totalTime)
          {
            totalTimeMax=totalTime;
            componentsTimeMax=componentsTime;
            visitorsTimeMax=visitorsTime;
          }

        //        window->pieChart->setChart(visitorsTime, visitorsTime.size());
        window->setCharts(componentsTime,componentsTimeMax,componentsTimeTotal,
                          visitorsTime,visitorsTimeMax,visitorsTimeTotal);
	return true;	   
      }
      

      void GraphVisitor::openAttribute      ( TiXmlElement* element, Q3ListViewItem* item)
      {
	if (!element) return;
	TiXmlAttribute* attribute=element->FirstAttribute();
	while (attribute)
	  {
	    std::string nameOfAttribute(attribute->Name());
	    std::string valueOfAttribute(attribute->Value());

	    addInformation(item, nameOfAttribute, valueOfAttribute);
	    attribute=attribute->Next();
	  }
	
      }



      void GraphVisitor::openTime      ( TiXmlNode* node, Q3ListViewItem* item)
      {        
        TiXmlElement* element=node->ToElement();
	if (!element) return;
	TiXmlAttribute* attribute=element->FirstAttribute();
        std::string valueOfAttribute(attribute->Value());
        double timeSec=atof(valueOfAttribute.c_str());
        double time = 100.0*timeSec/totalTime;
        std::ostringstream s;
        s.setf(std::ios::fixed, std::ios::floatfield);
        s.precision(3);

        s << time << "%";

        TiXmlNode* parent = node->Parent();
        if (parent)
          {
            std::string nodeType = parent->Value();
            if (nodeType == "Component")
              {
                TiXmlAttribute* attribute=parent->ToElement()->FirstAttribute();
                std::string componentName, componentType, componentPtr;
                while (attribute)
                  {
                    std::string nameOfAttribute(attribute->Name());
                    std::string valueOfAttribute(attribute->Value());
                    if (nameOfAttribute=="name")
                      componentName=valueOfAttribute;
                    else if (nameOfAttribute=="type")
                      componentType=valueOfAttribute;
                    else if (nameOfAttribute=="ptr")
                      componentPtr=valueOfAttribute;
                    attribute=attribute->Next();
                  }
                if (std::find(visitedNode.begin(), visitedNode.end(), componentName) == visitedNode.end())
                  {
                    dataTime t(timeSec-timeComponentsBelow.back(), componentType, componentName, componentPtr);
                    std::vector< dataTime >::iterator it=std::find(componentsTime.begin(),componentsTime.end(),t);
                    if (it != componentsTime.end()) it->time += t.time;
                    else componentsTime.push_back(t);


                    it=std::find(componentsTimeTotal.begin(),componentsTimeTotal.end(),t);
                    if (it != componentsTimeTotal.end()) it->time += t.time;
                    else componentsTimeTotal.push_back(t);


                    visitedNode.push_back(componentName);
                  }
              }
            else  
              { 
                if (std::find(visitedNode.begin(), visitedNode.end(),nodeType) == visitedNode.end())
                  {                 
                    dataTime t(timeSec, nodeType);
                    std::vector< dataTime >::iterator it=std::find(visitorsTime.begin(),visitorsTime.end(),t);
                    if (it != visitorsTime.end()) it->time += timeSec;
                    else visitorsTime.push_back(t);
   
                    it=std::find(visitorsTimeTotal.begin(),visitorsTimeTotal.end(),t);
                    if (it != visitorsTimeTotal.end()) it->time += t.time;
                    else visitorsTimeTotal.push_back(t);
   

                    visitedNode.push_back(nodeType);
                  }
              }
          }
        
        timeComponentsBelow.back() = timeSec;

        addTime(item,  s.str());
      }
      
      double GraphVisitor::getTotalTime(TiXmlNode* node)
      {

	for ( TiXmlNode* child = node->FirstChild(); child != 0; child = child->NextSibling()) 
	  {
            std::string nameOfNode=child->Value();
            if (nameOfNode == "TotalTime")
              {
                TiXmlAttribute* attribute=child->ToElement()->FirstAttribute();
                std::string valueOfAttribute(attribute->Value());
                return atof(valueOfAttribute.c_str());          
              } 
	  }
        return 1;
      }

      Q3ListViewItem* GraphVisitor::openNode( TiXmlNode* node, Q3ListViewItem* parent, Q3ListViewItem* elementAbove)
      {
	if (!node) return NULL;

        unsigned int sizeVisitedNode=visitedNode.size();
	std::string nameOfNode=node->Value();
	int typeOfNode=node->Type();
	Q3ListViewItem *graphNode=NULL;
	switch (typeOfNode)
	  {
	  case TiXmlNode::DOCUMENT:
	    break;

	  case TiXmlNode::ELEMENT:
            if (nameOfNode == "Time")
              {
                openTime( node, parent);
              }
            else
              {
                graphNode = addNode(parent, elementAbove, nameOfNode);
                openAttribute( node->ToElement(), graphNode);
              }
	    break;

	  case TiXmlNode::COMMENT:
	    graphNode = addComment(parent, elementAbove, nameOfNode); 
	    break;

	  case TiXmlNode::UNKNOWN:
	    break;

	  case TiXmlNode::TEXT:
	    break;

	  case TiXmlNode::DECLARATION:
	    break;
	  default:
	    break;

	  }

	Q3ListViewItem *element=NULL;
        timeComponentsBelow.push_back(0);

	for ( TiXmlNode* child = node->FirstChild(); child != 0; child = child->NextSibling()) 
	  {
	    element = openNode( child, graphNode, element);
	  }
        double t=timeComponentsBelow.back();

        timeComponentsBelow.pop_back();

        if (!timeComponentsBelow.empty()) timeComponentsBelow.back() += t;

        if (sizeVisitedNode != visitedNode.size()) visitedNode.resize(sizeVisitedNode);
	return graphNode;
      }


      Q3ListViewItem *GraphVisitor::addNode(Q3ListViewItem *parent, Q3ListViewItem *elementAbove, std::string name)
      {
	Q3ListViewItem *item=NULL;
	if (!parent)
	  {
	    //Add a Root
	    item=new Q3ListViewItem(graph, QString(name.c_str()));
	    item->setOpen(true);
	  }
	else
	  {
	    //Add a child to a node
	    item=new Q3ListViewItem(parent,elementAbove, QString(name.c_str()));
	  }
        QPixmap*  icon=WindowVisitor::getPixmap(WindowVisitor::getComponentType(name));
        if (icon) item->setPixmap(0,*icon);
	item->setMultiLinesEnabled(true);
	return item;     
      }

      void GraphVisitor::addTime(Q3ListViewItem *element, std::string info)
      {
	if (!element) return;
        element->setText(1, QString( info.c_str()));
      }

      void GraphVisitor::addInformation(Q3ListViewItem *element, std::string name, std::string info)
      {
	if (!element) return;
	if (element->text(0) == QString("Node"))
	  element->setText(0, QString(info.c_str()));
	else  if (element->text(0) == QString("Component"))
	  element->setText(0, QString(info.c_str()));
	else
	  {
	    if (element->text(2).isEmpty())
	      {
		element->setText(2, QString( name.c_str()));
		element->setText(3, QString( info.c_str()));
	      }
	    else
	      {
                if (name != "ptr")
                  {
                    QString nameQt = element->text(2) + QString("\n") + QString( name.c_str());
                    QString infoQt = element->text(3) + QString("\n") + QString( info.c_str());

                    element->setText(2, nameQt);
                    element->setText(3, infoQt);
                  }
	      }
	  }
      }
      
      Q3ListViewItem *GraphVisitor::addComment(Q3ListViewItem *element,Q3ListViewItem *elementAbove,  std::string comment)
      {
	if (!element) return NULL;
	Q3ListViewItem *result = new Q3ListViewItem(element, elementAbove,QString(comment.c_str()));
	result->setPixmap(0,*WindowVisitor::getPixmap(WindowVisitor::COMMENT));
	result->setSelectable(false);
	result->setMultiLinesEnabled(true);
	return result;
      }


    }
  }
}

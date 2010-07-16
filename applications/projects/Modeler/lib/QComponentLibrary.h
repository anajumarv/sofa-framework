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
#ifndef SOFA_QCOMPONENTLIBRARY_H
#define SOFA_QCOMPONENTLIBRARY_H


#include <sofa/core/ComponentLibrary.h>

#ifdef SOFA_QT4
#include <Q3Header>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#else
#include <qheader.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlayout.h>
#endif


namespace sofa
{

namespace gui
{

namespace qt
{

using sofa::core::ComponentLibrary;
typedef sofa::core::ObjectFactory::ClassEntryPtr ClassEntryPtr;

class QComponentLibrary : virtual public QWidget, public ComponentLibrary
{

    Q_OBJECT
public:
    typedef QGridLayout ComponentLayout;
    typedef QPushButton ComponentLabel;
    typedef QComboBox   ComponentTemplates;
public:
    QComponentLibrary(QWidget *parent, ComponentLayout *layout, const std::string &componentName, const std::string &categoryName, ClassEntryPtr& entry, const std::vector< std::string > &exampleFiles);
    ~QComponentLibrary();

    void endConstruction();


    void setDisplayed(bool b);

    void setLayout(ComponentLayout *l) {layout = l;}

    QWidget *getQWidget() { return this;};
protected:
    //--------------------------------------------
    //Qt Data
    ComponentLayout    *layout;
    ComponentLabel     *label;
    ComponentTemplates *templates;

public slots:
    void componentPressed();

signals:
    void componentDragged( std::string description, std::string templateName, ClassEntryPtr& entry);
};

}
}
}

#endif

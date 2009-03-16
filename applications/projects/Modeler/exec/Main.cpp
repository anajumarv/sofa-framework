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
#include <iostream>
#include <fstream>

#include <qapplication.h>
#include <sofa/simulation/tree/TreeSimulation.h>

#include "../lib/SofaModeler.h"
#include <sofa/helper/system/glut.h>
// ---------------------------------------------------------------------
// ---
// ---------------------------------------------------------------------

int main(int argc, char** argv)
{	
  glutInit(&argc,argv);
  QApplication* application = new QApplication(argc, argv);	
  (void)application;

  sofa::simulation::setSimulation(new sofa::simulation::tree::TreeSimulation());
  
  sofa::gui::qt::SofaModeler* sofaModeler = new sofa::gui::qt::SofaModeler();
  application->setMainWidget(sofaModeler);
  sofaModeler->show();

  for (int i=1;i<argc;++i)
    {
      //Try to open the simulations passed in command line
      sofaModeler->fileOpen(std::string(argv[i]));
    }
  if (argc <= 1 ) sofaModeler->newTab();

  return application->exec();
}

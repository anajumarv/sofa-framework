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
#ifndef SOFA_SIMULATION_TREE_COLORS_H
#define SOFA_SIMULATION_TREE_COLORS_H

namespace sofa
{

namespace simulation
{

namespace tree
{

namespace Colors
{

enum
{
    NODE = 0,
    OBJECT = 1,
    CONTEXT = 2,
    BMODEL = 3,
    CMODEL = 4,
    MMODEL = 5,
    CONSTRAINT = 6,
    IFFIELD = 7,
    FFIELD = 8,
    SOLVER = 9,
    COLLISION = 10,
    MMAPPING = 11,
    MAPPING = 12,
    MASS = 13,
    TOPOLOGY = 14,
    VMODEL = 15,
};

// See http://www.graphviz.org/doc/info/colors.html
// The following is mostly the "set312" colors

static const char* COLOR[16]=
{
    /*Node                  =*/ "#dedede", // color 9
    /*Object                =*/ "#ffffff", // white
    /*Context               =*/ "#d7191c", // color spectral4/1
    /*BehaviorModel         =*/ "#93ff49", // color 7 (brighter)
    /*CollisionModel        =*/ "#fccde5", // color 8
    /*MechanicalState       =*/ "#8dd3c7", // color 1
    /*Constraint            =*/ "#fdb462", // color 6
    /*InteractionForceField =*/ "#fb8072", // color 4
    /*ForceField            =*/ "#bebada", // color 3
    /*Solver                =*/ "#b3de69", // color 7
    /*CollisionPipeline     =*/ "#bc80bd", // color 10
    /*MechanicalMapping     =*/ "#2b83da", // color spectral4/4
    /*Mapping               =*/ "#80b1d3", // color 5
    /*Mass                  =*/ "#ffffb3", // color 2
    /*Topology              =*/ "#ffed6f", // color 12
    /*VisualModel           =*/ "#eefdea", // color 11 (brighter)
};

}

} // namespace tree

} // namespace simulation

} // namespace sofa

#endif

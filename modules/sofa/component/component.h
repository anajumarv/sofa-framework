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
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_H
#define SOFA_COMPONENT_H

#include <sofa/helper/system/config.h>

#ifdef SOFA_BUILD_COMPONENT
#  define SOFA_COMPONENT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_BASE
#  define SOFA_COMPONENT_BASE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_BASE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_COMMON
#  define SOFA_COMPONENT_COMMON_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_COMMON_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_GENERAL
#  define SOFA_COMPONENT_GENERAL_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_GENERAL_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_ADVANCED
#  define SOFA_COMPONENT_ADVANCED_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_ADVANCED_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_ADVANCED_DEV
#  define SOFA_COMPONENT_ADVANCED_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_ADVANCED_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_MISC
#  define SOFA_COMPONENT_MISC_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_MISC_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_MISC_DEV
#  define SOFA_COMPONENT_MISC_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_MISC_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_ADVANCED_CONSTRAINT
#  define SOFA_ADVANCED_CONSTRAINT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_ADVANCED_CONSTRAINT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_ADVANCED_FEM
#  define SOFA_ADVANCED_FEM_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_ADVANCED_FEM_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_ADVANCED_INTERACTION
#  define SOFA_ADVANCED_INTERACTION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_ADVANCED_INTERACTION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BASE_COLLISION
#  define SOFA_BASE_COLLISION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BASE_COLLISION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BASE_LINEAR_SOLVER
#  define SOFA_BASE_LINEAR_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BASE_LINEAR_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BASE_ANIMATION_LOOP
#  define SOFA_BASE_ANIMATION_LOOP_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BASE_ANIMATION_LOOP_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BASE_MECHANICS
#  define SOFA_BASE_MECHANICS_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BASE_MECHANICS_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BASE_TOPOLOGY
#  define SOFA_BASE_TOPOLOGY_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BASE_TOPOLOGY_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BASE_VISUAL
#  define SOFA_BASE_VISUAL_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BASE_VISUAL_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BOUNDARY_CONDITION
#  define SOFA_BOUNDARY_CONDITION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BOUNDARY_CONDITION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_CONSTRAINT
#  define SOFA_CONSTRAINT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_CONSTRAINT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_DEFORMABLE
#  define SOFA_DEFORMABLE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_DEFORMABLE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_DENSE_SOLVER
#  define SOFA_DENSE_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_DENSE_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_EIGEN2_SOLVER
#  define SOFA_EIGEN2_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_EIGEN2_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_ENGINE
#  define SOFA_ENGINE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_ENGINE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_EULERIAN_FLUID
#  define SOFA_EULERIAN_FLUID_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_EULERIAN_FLUID_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_EXPLICIT_ODE_SOLVER
#  define SOFA_EXPLICIT_ODE_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_EXPLICIT_ODE_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_EXPORTER
#  define SOFA_EXPORTER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_EXPORTER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_GRAPH_COMPONENT
#  define SOFA_GRAPH_COMPONENT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_GRAPH_COMPONENT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_HAPTICS
#  define SOFA_HAPTICS_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_HAPTICS_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_IMPLICIT_ODE_SOLVER
#  define SOFA_IMPLICIT_ODE_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_IMPLICIT_ODE_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_LOADER
#  define SOFA_LOADER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_LOADER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MESH_COLLISION
#  define SOFA_MESH_COLLISION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MESH_COLLISION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC
#  define SOFA_MISC_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_COLLISION
#  define SOFA_MISC_COLLISION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_COLLISION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_COLLISION_DEV
#  define SOFA_MISC_COLLISION_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_COLLISION_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_DEV
#  define SOFA_MISC_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_ENGINE
#  define SOFA_MISC_ENGINE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_ENGINE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_FEM
#  define SOFA_MISC_FEM_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_FEM_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_FEM_DEV
#  define SOFA_MISC_FEM_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_FEM_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_FORCEFIELD
#  define SOFA_MISC_FORCEFIELD_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_FORCEFIELD_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_FORCEFIELD_DEV
#  define SOFA_MISC_FORCEFIELD_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_FORCEFIELD_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_MAPPING
#  define SOFA_MISC_MAPPING_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_MAPPING_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_MAPPING_DEV
#  define SOFA_MISC_MAPPING_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_MAPPING_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_SOLVER
#  define SOFA_MISC_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_SOLVER_DEV
#  define SOFA_MISC_SOLVER_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_SOLVER_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_TOPOLOGY
#  define SOFA_MISC_TOPOLOGY_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_TOPOLOGY_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MISC_TOPOLOGY_DEV
#  define SOFA_MISC_TOPOLOGY_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MISC_TOPOLOGY_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MJED_FEM
#  define SOFA_MJED_FEM_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MJED_FEM_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_NON_UNIFORM_FEM
#  define SOFA_NON_UNIFORM_FEM_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_NON_UNIFORM_FEM_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_NON_UNIFORM_FEM_DEV
#  define SOFA_NON_UNIFORM_FEM_DEV_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_NON_UNIFORM_FEM_DEV_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_OBJECT_INTERACTION
#  define SOFA_OBJECT_INTERACTION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_OBJECT_INTERACTION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_OPENGL_VISUAL
#  define SOFA_OPENGL_VISUAL_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_OPENGL_VISUAL_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_PARDISO_SOLVER
#  define SOFA_PARDISO_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_PARDISO_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_PRECONDITIONER
#  define SOFA_PRECONDITIONER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_PRECONDITIONER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_RIGID
#  define SOFA_RIGID_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_RIGID_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_SIMPLE_FEM
#  define SOFA_SIMPLE_FEM_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_SIMPLE_FEM_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_SPARSE_SOLVER
#  define SOFA_SPARSE_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_SPARSE_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_SPH_FLUID
#  define SOFA_SPH_FLUID_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_SPH_FLUID_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_TAUCS_SOLVER
#  define SOFA_TAUCS_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_TAUCS_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_TOPOLOGY_MAPPING
#  define SOFA_TOPOLOGY_MAPPING_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_TOPOLOGY_MAPPING_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_USER_INTERACTION
#  define SOFA_USER_INTERACTION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_USER_INTERACTION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_VALIDATION
#  define SOFA_VALIDATION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_VALIDATION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_VOLUMETRIC_DATA
#  define SOFA_VOLUMETRIC_DATA_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_VOLUMETRIC_DATA_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_MANIFOLDTOPOLOGIES
#  define SOFA_MANIFOLD_TOPOLOGIES_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_MANIFOLD_TOPOLOGIES_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#endif //SOFA_COMPONENT_H

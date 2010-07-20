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

#include <sofa/core/Multi2Mapping.inl>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/behavior/MappedModel.h>
#include <sofa/defaulttype/RigidTypes.h>


namespace sofa
{

namespace core
{

  using namespace sofa::defaulttype;
  using namespace core::behavior;


  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes>, MechanicalState<Vec3dTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes>, MechanicalState<Vec3fTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Vec3dTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Vec3dTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Vec3fTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Vec3fTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Vec3dTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Vec3dTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Vec3fTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Vec3fTypes> >;



  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes>, MechanicalState<Rigid3dTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes>, MechanicalState<Rigid3fTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Rigid3dTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Rigid3dTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Rigid3fTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3dTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Rigid3fTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Rigid3dTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Rigid3dTypes> >;

  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3dTypes>, MechanicalState<Rigid3fTypes> >;
  template class Multi2Mapping< MechanicalState<Vec3fTypes>, MechanicalState<Rigid3fTypes>, MechanicalState<Rigid3fTypes> >;
}

}

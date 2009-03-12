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
#include "HapticMapping.inl"
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/core/ObjectFactory.h>


namespace sofa{

	namespace component{

		namespace mapping{


		using namespace sofa::defaulttype;
		using namespace core::componentmodel::behavior;

		SOFA_DECL_CLASS(HapticMapping);
		// Register in the Factory
		int HapticMappingClass = core::RegisterObject("Haptic Mapping")
		#ifndef SOFA_FLOAT
		.add< HapticMapping< Rigid3dTypes > >()
		#endif
		#ifndef SOFA_DOUBLE
		.add< HapticMapping< Rigid3fTypes > >();
		#endif
		
		#ifndef SOFA_FLOAT
		template class HapticMapping<Rigid3dTypes>;
		#endif
		#ifndef SOFA_DOUBLE
		template class HapticMapping<Rigid3fTypes>;
		#endif


		} // namespace mapping

	} // namespace component

} // namespace sofa


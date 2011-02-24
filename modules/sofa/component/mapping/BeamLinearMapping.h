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
#ifndef SOFA_COMPONENT_MAPPING_BEAMLINEARMAPPING_H
#define SOFA_COMPONENT_MAPPING_BEAMLINEARMAPPING_H

#include <sofa/core/Mapping.h>

#include <sofa/component/component.h>

#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/VecTypes.h>


namespace sofa
{

namespace component
{

namespace mapping
{

template <class TIn, class TOut>
class BeamLinearMapping : public core::Mapping<TIn, TOut>
{
public:
	SOFA_CLASS(SOFA_TEMPLATE2(BeamLinearMapping,TIn,TOut), SOFA_TEMPLATE2(core::Mapping,TIn,TOut));

    typedef core::Mapping<TIn, TOut> Inherit;
    typedef TIn In;
    typedef TOut Out;
	typedef Out OutDataTypes;
	typedef typename Out::VecCoord VecCoord;
	typedef typename Out::VecDeriv VecDeriv;
	typedef typename Out::Coord Coord;
	typedef typename Out::Deriv Deriv;
    	
	typedef In InDataTypes;
	typedef typename In::Deriv InDeriv;

	typedef typename Coord::value_type Real;
	enum { N=OutDataTypes::spatial_dimensions };
	typedef defaulttype::Mat<N,N,Real> Mat;
	
protected:
	helper::vector<Coord> points;
	//Coord translation;
	//Real orientation[4];
	//Mat rotation;
	sofa::helper::vector<Real> beamLength;
	sofa::helper::vector<Coord> rotatedPoints0;
	sofa::helper::vector<Coord> rotatedPoints1;
public:
	//Data<unsigned> index;
    Data<bool> localCoord;
	
	BeamLinearMapping(core::State<In>* from, core::State<Out>* to)
	: Inherit(from, to)
	//, index(initData(&index,(unsigned)0,"index","input DOF index"))
	, localCoord(initData(&localCoord,true,"localCoord","true if initial coordinates are in the beam local coordinate system (i.e. a point at (10,0,0) is on the DOF number 10, whereas if this is false it is at whatever position on the beam where the distance from the initial DOF is 10)"))
	{
	}
	
	virtual ~BeamLinearMapping()
	{
	}
	
	void init();
	
	void apply(Data< typename Out::VecCoord >& out, const Data< typename In::VecCoord >& in, const core::MechanicalParams *mparams);
	
	void applyJ(Data< typename Out::VecDeriv >& out, const Data< typename In::VecDeriv >& in, const core::MechanicalParams *mparams);
	
	void applyJT(Data< typename In::VecDeriv >& out, const Data< typename Out::VecDeriv >& in, const core::MechanicalParams *mparams);

	void applyJT(Data< typename In::MatrixDeriv >& out, const Data< typename Out::MatrixDeriv >& in, const core::ConstraintParams *cparams);

	void draw();
};


using sofa::defaulttype::Vec3dTypes;
using sofa::defaulttype::Vec3fTypes;
using sofa::defaulttype::ExtVec3fTypes;
using sofa::defaulttype::Rigid3dTypes;
using sofa::defaulttype::Rigid3fTypes;

#if defined(WIN32) && !defined(SOFA_COMPONENT_MAPPING_BEAMLINEARMAPPING_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3dTypes, Vec3dTypes >;
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3dTypes, ExtVec3fTypes >;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3fTypes, Vec3fTypes >;
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3fTypes, ExtVec3fTypes >;
#endif

#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3dTypes, Vec3fTypes >;
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3fTypes, Vec3dTypes >;
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3dTypes, Vec3fTypes >;
extern template class SOFA_COMPONENT_MAPPING_API BeamLinearMapping< Rigid3fTypes, Vec3dTypes >;
#endif
#endif
#endif

} // namespace mapping

} // namespace component

} // namespace sofa

#endif


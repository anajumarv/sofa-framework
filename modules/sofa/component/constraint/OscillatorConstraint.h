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
#ifndef SOFA_COMPONENT_CONSTRAINT_OSCILLATORCONSTRAINT_H
#define SOFA_COMPONENT_CONSTRAINT_OSCILLATORCONSTRAINT_H

#include <sofa/core/componentmodel/behavior/Constraint.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/VisualModel.h>
#include <sofa/helper/vector.h>


namespace sofa
{

namespace component
{

namespace constraint
{

/** Apply sinusoidal trajectories to particles. Defined as \f$ x = x_m A \sin ( \omega t + \phi )\f$
	where \f$ x_m, A , \omega t , \phi \f$ are the mean value, the amplitude, the pulsation and the phase, respectively.
	*/
template <class DataTypes>
class OscillatorConstraint : public core::componentmodel::behavior::Constraint<DataTypes>, public core::VisualModel
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::Real Real;

protected:
    struct Oscillator
    {
        Coord mean;
        Deriv amplitude;
        Real pulsation;
        Real phase;
        Oscillator( const Coord& m, const Deriv& a, const Real& w, const Real& p )
            : mean(m), amplitude(a), pulsation(w), phase(p) {}
    };
    helper::vector< std::pair<unsigned,Oscillator> > constraints; ///< constrained particles


public:
    OscillatorConstraint();

    OscillatorConstraint(core::componentmodel::behavior::MechanicalState<DataTypes>* mstate);

    ~OscillatorConstraint();

    OscillatorConstraint<DataTypes>* addConstraint(unsigned index, const Coord& mean, const Deriv& amplitude, Real pulsation, Real phase);

    // -- Constraint interface
    void projectResponse(VecDeriv& dx);
    virtual void projectVelocity(VecDeriv& /*dx*/); ///< project dx to constrained space (dx models a velocity)
    virtual void projectPosition(VecCoord& /*x*/); ///< project x to constrained space (x models a position)

    // -- Visual interface
    void draw() {}

    void initTextures() { }

    void update() { }
};

} // namespace constraint

} // namespace component

} // namespace sofa

#endif

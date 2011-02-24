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
#include <sofa/simulation/common/MechanicalVPrintVisitor.h>
#include <sofa/simulation/common/Node.h>

namespace sofa
{

namespace simulation
{

MechanicalVPrintVisitor::MechanicalVPrintVisitor(ConstMultiVecId v, std::ostream& out, const core::ExecParams* params )
: Visitor(params)
, v_(v)
, out_(out)
{
}

Visitor::Result MechanicalVPrintVisitor::processNodeTopDown(simulation::Node* node)
{
    if( ! node->mechanicalState.empty() )
    {
    	ConstVecId id = v_.getId(node->mechanicalState);
        if (!id.isNull())
        {
            out_<<"[ ";
            (*node->mechanicalState).printDOF(id,out_);
            out_<<"] ";
        }
    }
    return Visitor::RESULT_CONTINUE;
}


MechanicalVPrintWithElapsedTimeVisitor::MechanicalVPrintWithElapsedTimeVisitor(ConstMultiVecId vid, unsigned time, std::ostream& out, const core::ExecParams* params )
: Visitor (params)
, v_(vid)
, count_(0)
, time_(time)
, out_(out)
{
}

Visitor::Result MechanicalVPrintWithElapsedTimeVisitor::processNodeTopDown(simulation::Node* node)
{
    if( ! node->mechanicalState.empty() )
    {
    	ConstVecId id = v_.getId(node->mechanicalState);
        if (!id.isNull())
        {
            count_+=(*node->mechanicalState).printDOFWithElapsedTime(id,count_,time_,out_);
        }
    }
    return Visitor::RESULT_CONTINUE;
}

} // namespace simulation

} // namespace sofa

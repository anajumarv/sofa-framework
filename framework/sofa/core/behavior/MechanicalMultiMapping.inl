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
#ifndef SOFA_CORE_BEHAVIOR_MECHANICALMULTIMAPPING_INL
#define SOFA_CORE_BEHAVIOR_MECHANICALMULTIMAPPING_INL


#include <sofa/core/MultiMapping.h>
#include <sofa/core/MultiMapping.inl>
#include <sofa/core/behavior/MechanicalMultiMapping.h>
#include <algorithm>
#include <functional>

namespace sofa
{

namespace core
{

namespace behavior
{

template <class In, class Out>  
MechanicalMultiMapping<In,Out>::MechanicalMultiMapping()
: MultiMapping<In,Out>()
, f_isMechanical( initData( &f_isMechanical, true, "isMechanical", "set to false if this mapping should only be used as a regular mapping instead of a mechanical mapping" ) )
{
}
          
          template <class In, class Out>
          bool MechanicalMultiMapping<In,Out>::isMechanical()
          {
              return this->f_isMechanical.getValue();
          }

          template <class In, class Out>
          void MechanicalMultiMapping<In,Out>::setMechanical(bool b)
          {
            f_isMechanical.setValue(b);
          }

          template < class In, class Out > 
          helper::vector<BaseMechanicalState*> MechanicalMultiMapping<In,Out>::getMechFrom() 
          {
            helper::vector<BaseMechanicalState*> mechFromVec;
            std::copy(this->fromModels.begin(), this->fromModels.end(), std::back_inserter(mechFromVec));
            return mechFromVec;
          }

          template < class In, class Out > 
          helper::vector<BaseMechanicalState*> MechanicalMultiMapping<In,Out>::getMechTo() 
          {
            helper::vector<BaseMechanicalState*> mechToVec;
            std::copy(this->toModels.begin(), this->toModels.end(), std::back_inserter(mechToVec));
            return mechToVec;
          }


         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::init()
         {
            this->updateMapping();
            propagateXfree();
         }
 
         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::propagateX()
         {
           if( this->fromModels.empty() || this->toModels.empty() ){
            return;
           }
           const VecId &idCoord = VecId::position();
           helper::vector<typename Out::VecCoord*> vecOutPos;
           getVecOutCoord(idCoord, vecOutPos);
           helper::vector<const typename In::VecCoord*> vecInPos;
           getConstVecInCoord(idCoord, vecInPos);

           apply ( vecOutPos, vecInPos);
         }

         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::propagateXfree()
         {
           if( this->fromModels.empty() || this->toModels.empty() ){
            return;
           }
           const VecId &idDeriv = VecId::freePosition();
           helper::vector<typename Out::VecDeriv*> vecOutVel;
           getVecOutDeriv(idDeriv, vecOutVel);
           helper::vector<const typename In::VecDeriv*> vecInVel;
           getConstVecInDeriv(idDeriv, vecInVel);

           applyJ( vecOutVel, vecInVel);
         }

         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::propagateDx()
         {
           if( this->fromModels.empty() || this->toModels.empty() ){
             return;
           }
           const VecId &idDeriv = VecId::dx();
           helper::vector<typename Out::VecDeriv*> vecOutVel;
           getVecOutDeriv(idDeriv, vecOutVel);
           helper::vector<const typename In::VecDeriv*> vecInVel;
           getConstVecInDeriv(idDeriv, vecInVel);

           applyJ( vecOutVel, vecInVel);
         }

         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::propagateV()
         {
           if( this->fromModels.empty() || this->toModels.empty() ){
             return;
           }
           const VecId &idDeriv = VecId::velocity();
           helper::vector<typename Out::VecDeriv*> vecOutVel;
           getVecOutDeriv(idDeriv, vecOutVel);
           helper::vector<const typename In::VecDeriv*> vecInVel;
           getConstVecInDeriv(idDeriv, vecInVel);

           applyJ( vecOutVel, vecInVel);
         }


         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::propagateA()
         {
           if( this->fromModels.empty() || this->toModels.empty() ){
             return;
           }
           const VecId &v = VecId::velocity();
           const VecId &dx = VecId::dx();

           helper::vector<typename Out::VecDeriv*> vecOutDx;
           getVecOutDeriv(dx, vecOutDx);
           helper::vector<const typename In::VecDeriv*> vecInV;
           getConstVecInDeriv(v, vecInV);
           helper::vector<const typename In::VecDeriv*> vecInDx;
           getConstVecInDeriv(dx, vecInDx);

           this->computeAccFromMapping( vecOutDx, vecInV, vecInDx );
         }

         template <class T, class Container>
         struct GetForceVectorFunctor
         {
           GetForceVectorFunctor(Container &vector):v(vector){};
           void operator()(T* mstate)
           {
             v.push_back(mstate->getVecDeriv(mstate->getForceId().index));
           }

         protected:
           Container &v;
         };


         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::accumulateForce()
         {
            if( this->fromModels.empty() || this->toModels.empty() ){
             return;
           }

            helper::vector<typename In::VecDeriv*> vecOutForce;
            GetForceVectorFunctor<In, helper::vector<typename In::VecDeriv*> > OutF(vecOutForce);
            std::for_each(this->fromModels.begin(), this->fromModels.end(), OutF);


            helper::vector<const typename Out::VecDeriv*> vecInForce;
            GetForceVectorFunctor<Out, helper::vector<const typename Out::VecDeriv*> > InF(vecInForce);
            std::for_each(this->toModels.begin(), this->toModels.end(), InF);

            this->applyJT(vecOutForce, vecInForce);
         }

         template < class In, class Out>
         void MechanicalMultiMapping<In,Out>::accumulateDf()
         {
           if( this->fromModels.empty() || this->toModels.empty() ){
             return;
           }

           helper::vector<typename In::VecDeriv*> vecOutForce;
           GetForceVectorFunctor<In, helper::vector<typename In::VecDeriv*> > OutF(vecOutForce);
           std::for_each(this->fromModels.begin(), this->fromModels.end(), OutF);

           helper::vector<const typename Out::VecDeriv*> vecInForce;
           GetForceVectorFunctor<Out, helper::vector<const typename Out::VecDeriv*> > InF(vecInForce);
           std::for_each(this->toModels.begin(), this->toModels.end(), InF);

           this->applyJT(vecOutForce, vecInForce);
         }
      
} // namespace behavior

} // namespace core

} // namespace sofa

#endif

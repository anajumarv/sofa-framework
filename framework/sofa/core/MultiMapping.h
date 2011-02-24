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
#ifndef SOFA_CORE_MULTIMAPPING_H
#define SOFA_CORE_MULTIMAPPING_H

#include <sofa/core/BaseMapping.h>
#include <sofa/core/State.h>
#include <sofa/core/core.h>
#include <sofa/core/VecId.h>
#include <sofa/core/objectmodel/ObjectRef.h>

namespace sofa
{

namespace core
{

/**
 *  \brief Specialized interface to describe many to many mapping. 
 *   Currently supports only one type for inputs and one type for outputs.
 */

template <class TIn, class TOut>
class MultiMapping : public BaseMapping
{
public:
    SOFA_CLASS(SOFA_TEMPLATE2(MultiMapping,TIn,TOut), BaseMapping);

    /// Input Model Type
    typedef TIn In;
    /// Output Model Type
    typedef TOut Out;

    typedef typename In::VecCoord InVecCoord;
    typedef typename In::VecDeriv InVecDeriv;
    typedef typename In::MatrixDeriv InMatrixDeriv;
    typedef Data<InVecCoord> InDataVecCoord;
    typedef Data<InVecDeriv> InDataVecDeriv;
    typedef Data<InMatrixDeriv> InDataMatrixDeriv;

    typedef typename Out::VecCoord OutVecCoord;
    typedef typename Out::VecDeriv OutVecDeriv;
    typedef typename Out::MatrixDeriv OutMatrixDeriv;
    typedef Data<OutVecCoord> OutDataVecCoord;
    typedef Data<OutVecDeriv> OutDataVecDeriv;
    typedef Data<OutMatrixDeriv> OutDataMatrixDeriv;

protected:
  /// Input Models container. New inputs are added through addInputModel(In* ).
    helper::vector<State<In>*> fromModels;
    /// Output Model container. New outputs are added through addOutputModel( Ou* )
    helper::vector<State<Out>*> toModels;

public:
    /// Constructor
    MultiMapping(helper::vector< State<In>* > in, helper::vector< State<Out>* > out);
    /// Destructor
    virtual ~MultiMapping() {};

	objectmodel::DataVectorObjectRef m_inputObjects;
	objectmodel::DataVectorObjectRef m_outputObjects;

    void addInputModel(State< In >* );
    void addOutputModel(State< Out >* );

    /// Return the reference to fromModels.
    helper::vector< State<In> *>& getFromModels();
    /// Return reference to toModels. 
    helper::vector< State<Out> *>& getToModels();

    /// Return a container of input models statically casted as BaseObject*
    helper::vector<BaseState* > getFrom();
    /// Return container of output model statically casted as BaseObject*.
    helper::vector<BaseState* > getTo();

    /// Get the source (upper) model.
    virtual helper::vector<behavior::BaseMechanicalState*> getMechFrom();

    /// Get the destination (lower, mapped) model.
    virtual helper::vector<behavior::BaseMechanicalState*> getMechTo();

    /// Apply ///
    /// Apply the mapping on position vectors.
    ///
    /// If the Mapping can be represented as a matrix J, this method computes
    /// $ out = J in $
    virtual void apply (MultiVecCoordId outPos, ConstMultiVecCoordId inPos, const MechanicalParams* mparams = MechanicalParams::defaultInstance() );

    /// This method must be reimplemented by all mappings.
    /// InPos and OutPos by default contains VecIds of type V_COORD.
    /// The size of InPos vector is the same as the number of fromModels.
    /// The size of OutPos vector is the same as the number of OutModels.
    virtual void apply(const helper::vector<OutDataVecCoord*>& dataVecOutPos, const helper::vector<const InDataVecCoord*>& dataVecInPos, const MechanicalParams* /* mparams */)
#ifdef SOFA_DEPRECATE_OLD_API
    = 0;
#else
    {
        //Not optimized at all...
        helper::vector<OutVecCoord*> vecOutPos;
        for(unsigned int i=0; i<dataVecOutPos.size(); i++)
            vecOutPos.push_back(dataVecOutPos[i]->beginEdit());

        helper::vector<const InVecCoord*> vecInPos;
        for(unsigned int i=0; i<dataVecInPos.size(); i++)
            vecInPos.push_back(&dataVecInPos[i]->getValue());

        this->apply(vecOutPos, vecInPos);

        //Really Not optimized at all...
        for(unsigned int i=0; i<dataVecOutPos.size(); i++)
            dataVecOutPos[i]->endEdit();

    }
    /// Compat Method
    /// @deprecated
    virtual void apply(const helper::vector<OutVecCoord*>&  /* outPos */, const helper::vector<const InVecCoord*>& /* inPos */) { };
#endif //SOFA_DEPRECATE_OLD_API

    /// ApplyJ ///
    /// Apply the mapping on derived (velocity, displacement) vectors.
    ///
    /// If the Mapping can be represented as a matrix J, this method computes
    /// $ out = J in $
    virtual void applyJ (MultiVecDerivId outVel, ConstMultiVecDerivId inVel, const MechanicalParams* mparams = MechanicalParams::defaultInstance() );

    /// This method must be reimplemented by all mappings.
    /// InDeriv and OutDeriv by default contains VecIds of type V_DERIV.
    /// The size of InDeriv vector is the same as the number of fromModels.
    /// The size of OutDeriv vector is the same as the number of OutModels.
    virtual void applyJ(const helper::vector<OutDataVecDeriv*>& dataVecOutVel, const helper::vector<const InDataVecDeriv*>& dataVecInVel, const MechanicalParams* /* mparams */)
#ifdef SOFA_DEPRECATE_OLD_API
    = 0;
#else
    {
        //Not optimized at all...
        helper::vector<OutVecDeriv*> vecOutVel;
        for(unsigned int i=0; i<dataVecOutVel.size(); i++)
            vecOutVel.push_back(dataVecOutVel[i]->beginEdit());

        helper::vector<const InVecDeriv*> vecInVel;
        for(unsigned int i=0; i<dataVecInVel.size(); i++)
            vecInVel.push_back(&dataVecInVel[i]->getValue());

        this->applyJ(vecOutVel, vecInVel);

        //Really Not optimized at all...
        for(unsigned int i=0; i<dataVecOutVel.size(); i++)
            dataVecOutVel[i]->endEdit();

    }
    /// Compat Method
    /// @deprecated
    virtual void applyJ(const helper::vector< OutVecDeriv*>& /* outDeriv */, const helper::vector<const InVecDeriv*>& /* inDeriv */) { };
#endif //SOFA_DEPRECATE_OLD_API

    /// ApplyJT (Force)///
    /// Apply the reverse mapping on force vectors.
    ///
    /// If the MechanicalMapping can be represented as a matrix J, this method computes
    /// $ out += J^t in $
    virtual void applyJT (MultiVecDerivId inForce, ConstMultiVecDerivId outForce, const MechanicalParams* mparams = MechanicalParams::defaultInstance() );

    /// This method must be reimplemented by all mappings.
    /// InDeriv and OutDeriv by default contains VecIds of type V_DERIV.
    /// The size of InDeriv vector is the same as the number of fromModels.
    /// The size of OutDeriv vector is the same as the number of OutModels.
    virtual void applyJT(const helper::vector<InDataVecDeriv*>& dataVecOutForce, const helper::vector<const OutDataVecDeriv*>& dataVecInForce, const MechanicalParams* /* mparams */)
#ifdef SOFA_DEPRECATE_OLD_API
    = 0;
#else
    {
        //Not optimized at all...
        helper::vector<InVecDeriv*> vecOutForce;
        for(unsigned int i=0; i<dataVecOutForce.size(); i++)
            vecOutForce.push_back(dataVecOutForce[i]->beginEdit());

        helper::vector<const OutVecDeriv*> vecInForce;
        for(unsigned int i=0; i<dataVecInForce.size(); i++)
            vecInForce.push_back(&dataVecInForce[i]->getValue());

        this->applyJT(vecOutForce, vecInForce);

        //Really Not optimized at all...
        for(unsigned int i=0; i<dataVecOutForce.size(); i++)
            dataVecOutForce[i]->endEdit();

    }
    /// Compat Method
    /// @deprecated
    virtual void applyJT(const helper::vector< InVecDeriv*>& /* outDeriv */, const helper::vector<const OutVecDeriv*>& /* inDeriv */) { };
#endif //SOFA_DEPRECATE_OLD_API

    /// ApplyJT (Constraint)///
    virtual void applyJT(MultiMatrixDerivId inConst, ConstMultiMatrixDerivId outConst, const ConstraintParams* cparams )
    {
        helper::vector<InDataMatrixDeriv*> matOutConst;
        getMatInDeriv(inConst, matOutConst);
        helper::vector<const OutDataMatrixDeriv*> matInConst;
        getConstMatOutDeriv(outConst, matInConst);

        this->applyJT(matOutConst, matInConst, cparams);
    }
    /// This method must be reimplemented by all mappings if they need to support constraints.
    virtual void applyJT( const helper::vector< InDataMatrixDeriv* >& dataMatOutConst, const helper::vector< const OutDataMatrixDeriv* >& dataMatInConst, const ConstraintParams* /* cparams */ )
#ifdef SOFA_DEPRECATE_OLD_API
    {
        serr << "This mapping does not support constraints" << sendl;
    }
#else
    {
        //Not optimized at all...
        helper::vector<InMatrixDeriv*> matOutConst;
        for(unsigned int i=0; i<dataMatOutConst.size(); i++)
            matOutConst.push_back(dataMatOutConst[i]->beginEdit());

        helper::vector<const OutMatrixDeriv*> matInConst;
        for(unsigned int i=0; i<dataMatInConst.size(); i++)
            matInConst.push_back(&dataMatInConst[i]->getValue());

        this->applyJT(matOutConst, matInConst);

        //Really Not optimized at all...
        for(unsigned int i=0; i<dataMatOutConst.size(); i++)
            dataMatOutConst[i]->endEdit();
    }
    /// Compat Method
    /// @deprecated
    virtual void applyJT( const helper::vector< InMatrixDeriv* >& /*outConst*/ , const helper::vector< const OutMatrixDeriv* >& /*inConst*/ )
    {
        serr << "This mapping does not support constraints" << sendl;
    }
#endif //SOFA_DEPRECATE_OLD_API

    /// computeAccFromMapping
    virtual void computeAccFromMapping(MultiVecDerivId outAcc, ConstMultiVecDerivId inVel, ConstMultiVecDerivId inAcc, const MechanicalParams* mparams = MechanicalParams::defaultInstance() )
    {
        helper::vector<OutDataVecDeriv*> vecOutAcc;
        getVecOutDeriv(outAcc, vecOutAcc);
        helper::vector<const InDataVecDeriv*> vecInVel;
        getConstVecInDeriv(inVel, vecInVel);
        helper::vector<const InDataVecDeriv*> vecInAcc;
        getConstVecInDeriv(inAcc, vecInAcc);

        this->computeAccFromMapping(vecOutAcc, vecInVel, vecInAcc, mparams);
    }
    /// This method must be reimplemented by all mappings if they need to support composite accelerations
    virtual void computeAccFromMapping( const helper::vector< OutDataVecDeriv*>& dataVecOutAcc,
                                        const helper::vector<const InDataVecDeriv*>& dataVecInVel,
                                        const helper::vector<const InDataVecDeriv*>& dataVecInAcc,
                                        const MechanicalParams* /* mparams */)
#ifdef SOFA_DEPRECATE_OLD_API
    {
    }
#else
    {
        //Not optimized at all...
        helper::vector<OutVecDeriv*> vecOutAcc;
        for(unsigned int i=0; i<dataVecOutAcc.size(); i++)
            vecOutAcc.push_back(dataVecOutAcc[i]->beginEdit());

        helper::vector<const InVecDeriv*> vecInVel;
        for(unsigned int i=0; i<dataVecInVel.size(); i++)
            vecInVel.push_back(&dataVecInVel[i]->getValue());
        helper::vector<const InVecDeriv*> vecInAcc;
        for(unsigned int i=0; i<dataVecInAcc.size(); i++)
            vecInAcc.push_back(&dataVecInAcc[i]->getValue());

        this->computeAccFromMapping(vecOutAcc, vecInVel, vecInAcc);

        //Really Not optimized at all...
        for(unsigned int i=0; i<dataVecOutAcc.size(); i++)
            dataVecOutAcc[i]->endEdit();
    }
    /// Compat Method
    /// @deprecated
    virtual void computeAccFromMapping( const helper::vector<typename Out::VecDeriv*>& /*outDx*/,
                                        const helper::vector<const typename In::VecDeriv*>& /*inV */,
                                        const helper::vector<const typename In::VecDeriv*>& /*inDx */ )
    {
    }
#endif //SOFA_DEPRECATE_OLD_API

    virtual void init();

    ///<TO REMOVE>
    /// Apply the mapping to position and velocity vectors.
    ///
    /// This method call the internal apply(helper::vector<VecId>& InPos, helper::vector<VecId>& OutPos)
    /// and applyJ(helper::vector<VecId>& InDeriv, helper::vector<VecId>& OutDeriv) methods.
    //virtual void updateMapping();

    /// Disable the mapping to get the original coordinates of the mapped model.
    ///
    /// It is for instance used in RigidMapping to get the local coordinates of the object.
    virtual void disable();

   
    
    virtual std::string getTemplateName() const
    {
      return templateName(this);
    }

    static std::string templateName(const MultiMapping<TIn, TOut>* = NULL);

	/// Pre-construction check method called by ObjectFactory.
    ///
    /// This implementation read the object1 and object2 attributes and check
    /// if they are compatible with the input and output models types of this
    /// mapping.
	template<class T>
	static bool canCreate(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
	{
		bool createResult = true;
		helper::vector< State<In>* > stin;
		helper::vector< State<Out>* > stout;

		createResult = sofa::core::objectmodel::VectorObjectRef::parseAll< State<In> >("input", arg, stin);
		createResult = createResult && sofa::core::objectmodel::VectorObjectRef::parseAll< State<Out> >("output", arg, stout);
	
		//If one of the parsing failed
		if(!createResult)
			return false;

		return BaseMapping::canCreate(obj, context, arg);
	}

	/// Construction method called by ObjectFactory.
	///
	/// This implementation read the input and output attributes to
	/// find the input and output models of this mapping.
	template<class T>
	static void create(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
	{
		bool createResult = true;
		helper::vector< State<In>* > stin;
		helper::vector< State<Out>* > stout;

		createResult = sofa::core::objectmodel::VectorObjectRef::parseAll< State<In> >("input", arg, stin);
		createResult = createResult && sofa::core::objectmodel::VectorObjectRef::parseAll< State<Out> >("output", arg, stout);
	
		//If one of the parsing failed
		if(!createResult)
			return;
	
		obj = new T( stin, stout);

		if (context) 
			context->addObject(obj);

		if (arg)
			obj->parse(arg);
	}

protected:

    void getVecInCoord     (const MultiVecCoordId id,         helper::vector<      InDataVecCoord* > &v) const
    {   for (unsigned int i=0;i<fromModels.size();++i) v.push_back(id[fromModels[i]].write()); }
    void getConstVecInCoord(const ConstMultiVecCoordId id,    helper::vector<const InDataVecCoord* > &v) const
    {   for (unsigned int i=0;i<fromModels.size();++i) v.push_back(id[fromModels[i]].read());  }
    void getVecInDeriv      (const MultiVecDerivId id,         helper::vector<      InDataVecDeriv* > &v) const
    {   for (unsigned int i=0;i<fromModels.size();++i) v.push_back(id[fromModels[i]].write()); }
    void getConstVecInDeriv (const ConstMultiVecDerivId id,    helper::vector<const InDataVecDeriv* > &v) const
    {   for (unsigned int i=0;i<fromModels.size();++i) v.push_back(id[fromModels[i]].read());  }
    void getMatInDeriv      (const MultiMatrixDerivId id,      helper::vector<      InDataMatrixDeriv* > &v) const
    {   for (unsigned int i=0;i<fromModels.size();++i) v.push_back(id[fromModels[i]].write()); }
    void getConstMatInDeriv (const ConstMultiMatrixDerivId id, helper::vector<const InDataMatrixDeriv* > &v) const
    {   for (unsigned int i=0;i<fromModels.size();++i) v.push_back(id[fromModels[i]].read());  }

    void getVecOutCoord     (const MultiVecCoordId id,         helper::vector<      OutDataVecCoord* > &v) const
    {   for (unsigned int i=0;i<toModels.size();++i)  v.push_back(id[toModels[i]].write());    }
    void getConstVecOutCoord(const ConstMultiVecCoordId id,    helper::vector<const OutDataVecCoord* > &v) const
    {   for (unsigned int i=0;i<toModels.size();++i)  v.push_back(id[toModels[i]].read());     }
    void getVecOutDeriv     (const MultiVecDerivId id,         helper::vector<      OutDataVecDeriv* > &v) const
    {   for (unsigned int i=0;i<toModels.size();++i)  v.push_back(id[toModels[i]].write());    }
    void getConstVecOutDeriv(const ConstMultiVecDerivId id,    helper::vector<const OutDataVecDeriv* > &v) const
    {   for (unsigned int i=0;i<toModels.size();++i)  v.push_back(id[toModels[i]].read());     }
    void getMatOutDeriv     (const MultiMatrixDerivId id,      helper::vector<      OutDataMatrixDeriv* > &v) const
    {   for (unsigned int i=0;i<toModels.size();++i)  v.push_back(id[toModels[i]].write()); }
    void getConstMatOutDeriv(const ConstMultiMatrixDerivId id, helper::vector<const OutDataMatrixDeriv* > &v) const
    {   for (unsigned int i=0;i<toModels.size();++i)  v.push_back(id[toModels[i]].read());  }


    /// If true, display the mapping
    bool getShow() const { return this->getContext()->getShowMappings(); }
};

} // namespace core

} // namespace sofa

#endif


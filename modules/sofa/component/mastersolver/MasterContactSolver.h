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
#ifndef SOFA_COMPONENT_ODESOLVER_MASTERCONTACTSOLVER_H
#define SOFA_COMPONENT_ODESOLVER_MASTERCONTACTSOLVER_H

#include <sofa/core/componentmodel/behavior/OdeSolver.h>
#include <sofa/simulation/common/MasterSolverImpl.h>
#include <sofa/simulation/common/Node.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/core/componentmodel/behavior/BaseConstraintCorrection.h>
#include <sofa/core/componentmodel/behavior/OdeSolver.h>
#include <sofa/component/odesolver/OdeSolverImpl.h>
#include <sofa/component/linearsolver/FullMatrix.h>
#include <sofa/component/linearsolver/SparseMatrix.h>
#include <sofa/helper/set.h>
#include <sofa/helper/LCPcalc.h>

namespace sofa
{

namespace component
{

namespace odesolver
{

using namespace sofa::defaulttype;
using namespace sofa::component::linearsolver;
using namespace helper::system::thread;

class SOFA_COMPONENT_MASTERSOLVER_API LCP
{
public:
        int maxConst;
        LPtrFullMatrix<double> W;
    FullVector<double> dFree, f;
        double tol;
        int numItMax;
        unsigned int nbConst;
        bool useInitialF;
        double mu;
        int dim;
private:
        bool lok;

public:
        LCP(unsigned int maxConstraint);
        ~LCP();
        void reset(void);
        //LCP& operator=(LCP& lcp);
        inline double** getW(void) {return W.lptr();};
        inline double& getMu(void) { return mu;};
        inline double* getDfree(void) {return dFree.ptr();};
        inline int getDfreeSize(void) {return dFree.size();};
        inline double getTolerance(void) {return tol;};
        inline void setTol(double t) {tol = t;};
        inline double getMaxIter(void) {return numItMax;};
        inline double* getF(void) {return f.ptr();};
        inline bool useInitialGuess(void) {return useInitialF;};
        inline unsigned int getNbConst(void) {return nbConst;};
        inline void setNbConst(unsigned int nbC) {nbConst = nbC;};
        inline unsigned int getMaxConst(void) {return maxConst;};

        inline bool isLocked(void) {return false;};
        inline void lock(void) {lok = true;};
        inline void unlock(void) {lok = false;};
        inline void wait(void) {while(lok) ; } //infinite loop?
};



class SOFA_COMPONENT_MASTERSOLVER_API MechanicalResetContactForceVisitor : public simulation::MechanicalVisitor
{
public:
    VecId force;
        MechanicalResetContactForceVisitor()
        {
        }

        virtual Result fwdMechanicalState(simulation::Node* node, core::componentmodel::behavior::BaseMechanicalState* ms)
        {
                ctime_t t0 = begin(node, ms);
                ms->resetContactForce();
                end(node, ms, t0);
                return RESULT_CONTINUE;
        }

        virtual Result fwdMappedMechanicalState(simulation::Node* node, core::componentmodel::behavior::BaseMechanicalState* ms)
        {
                ctime_t t0 = begin(node, ms);
                ms->resetForce();
                end(node, ms, t0);
                return RESULT_CONTINUE;
        }
#ifdef SOFA_DUMP_VISITOR_INFO
    void setReadWriteVectors()
    {
    }
#endif
};

/* ACTION 2 : Apply the Contact Forces on mechanical models & Compute displacements */
class SOFA_COMPONENT_MASTERSOLVER_API MechanicalApplyContactForceVisitor : public simulation::MechanicalVisitor
{
public:
    VecId force;
        MechanicalApplyContactForceVisitor(double *f):_f(f)
        {
        }
        virtual Result fwdMechanicalState(simulation::Node* node, core::componentmodel::behavior::BaseMechanicalState* ms)
        {
          ctime_t t0 = begin(node, ms);
                ms->applyContactForce(_f);
                end(node, ms, t0);
                return RESULT_CONTINUE;
        }

        virtual Result fwdMappedMechanicalState(simulation::Node* node, core::componentmodel::behavior::BaseMechanicalState* ms)
        {
                ctime_t t0 = begin(node, ms);
                ms->applyContactForce(_f);
                end(node, ms, t0);
                return RESULT_CONTINUE;
        }
#ifdef SOFA_DUMP_VISITOR_INFO
    void setReadWriteVectors()
    {
    }
#endif

private:
      double *_f; // vector of contact forces from lcp //
                                // to be multiplied by constraint direction in mechanical models //

};

/* ACTION 3 : gets the vector of constraint values */
/* ACTION 3 : gets the vector of constraint values */
class SOFA_COMPONENT_MASTERSOLVER_API MechanicalGetConstraintValueVisitor : public simulation::MechanicalVisitor
{
  public:

        MechanicalGetConstraintValueVisitor(BaseVector *v): _v(v) // , _numContacts(numContacts)
        {
#ifdef SOFA_DUMP_VISITOR_INFO
          setReadWriteVectors();
#endif
        }

    virtual Result fwdConstraint(simulation::Node* node, core::componentmodel::behavior::BaseConstraint* c)
        {
            //sout << c->getName()<<"->getConstraintValue()"<<sendl;
                ctime_t t0 = begin(node, c);
                c->getConstraintValue(_v /*, _numContacts*/);
                end(node, c, t0);
                return RESULT_CONTINUE;
        }
#ifdef SOFA_DUMP_VISITOR_INFO
    void setReadWriteVectors()
    {
    }
#endif
private:
        BaseVector* _v; // vector for constraint values
        // unsigned int &_numContacts; // we need an offset to fill the vector _v if differents contact class are created
};


class SOFA_COMPONENT_MASTERSOLVER_API MechanicalGetContactIDVisitor : public simulation::MechanicalVisitor
{
public:
        MechanicalGetContactIDVisitor(long *id, unsigned int offset = 0)
                : _id(id),_offset(offset)
  {
#ifdef SOFA_DUMP_VISITOR_INFO
    setReadWriteVectors();
#endif
}

    virtual Result fwdConstraint(simulation::Node* node, core::componentmodel::behavior::BaseConstraint* c)
        {
                ctime_t t0 = begin(node, c);
                c->getConstraintId(_id, _offset);
                end(node, c, t0);
                return RESULT_CONTINUE;
        }

#ifdef SOFA_DUMP_VISITOR_INFO
    void setReadWriteVectors()
    {
    }
#endif
private:
        long *_id;
        unsigned int _offset;
};



class SOFA_COMPONENT_MASTERSOLVER_API MasterContactSolver : public sofa::simulation::MasterSolverImpl
{
public:
	SOFA_CLASS(MasterContactSolver, sofa::simulation::MasterSolverImpl);
		// for unbuilt lcp
		// typedef Mat<3,3,double> Mat3x3;
		typedef std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> list_cc;
		typedef std::vector<list_cc> VecListcc;

        Data<bool> displayTime;
        Data<bool> initial_guess;
		Data<bool> build_lcp;
        Data < double > tol;
        Data < int > maxIt;
        Data < double > mu;

        Data < helper::set<int> > constraintGroups;

        MasterContactSolver();
    // virtual const char* getTypeName() const { return "MasterSolver"; }

		void step (double dt);

        //virtual void propagatePositionAndVelocity(double t, VecId x, VecId v);

        virtual void init();
        //LCP* getLCP(void) {return (lcp == &lcp1) ? &lcp2 : &lcp1;};
		LCP* getLCP();
		void lockLCP(LCP* l1, LCP* l2=0); ///< Do not use the following LCP until the next call to the function

private:
        std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> constraintCorrections;
        void computeInitialGuess();
        void keepContactForcesValue();

		unsigned int _numConstraints;
        double _mu;


 		/// for built lcp ///
        void build_LCP();
        LCP lcp1, lcp2, lcp3;
        LPtrFullMatrix<double>  *_W;
        LCP *lcp;
		
		/// common built-unbuilt
        simulation::Node *context;
        FullVector<double> *_dFree, *_result;
		///

		
		/// for unbuilt lcp ///
		void build_problem_info();
		int lcp_gaussseidel_unbuilt(double *dfree, double *f);
		int nlcp_gaussseidel_unbuilt(double *dfree, double *f);
		int gaussseidel_unbuilt(double *dfree, double *f) { if (_mu == 0.0) return lcp_gaussseidel_unbuilt(dfree, f); else return nlcp_gaussseidel_unbuilt(dfree, f); }

		SparseMatrix<double> *_Wdiag;
		//std::vector<helper::LocalBlock33 *> _Wdiag;
		std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> _cclist_elem1;
		std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> _cclist_elem2;
		
		
						

        typedef struct {
                Vector3 n;
                Vector3 t;
                Vector3 s;
                Vector3 F;
                long id;

        } contactBuf;

	helper::vector<contactBuf> _PreviousContactList;
        unsigned int _numPreviousContact;
	helper::vector<long> _cont_id_list;

	// for gaussseidel_unbuilt
	helper::vector< helper::LocalBlock33 > unbuilt_W33;
	helper::vector< double > unbuilt_d;

	helper::vector< double > unbuilt_W11;
	helper::vector< double > unbuilt_invW11;
	
};

} // namespace odesolver

} // namespace component

} // namespace sofa

#endif

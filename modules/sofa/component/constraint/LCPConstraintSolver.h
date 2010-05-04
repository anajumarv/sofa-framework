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
#ifndef SOFA_COMPONENT_CONSTRAINT_LCPCONSTRAINTSOLVER_H
#define SOFA_COMPONENT_CONSTRAINT_LCPCONSTRAINTSOLVER_H

#include <sofa/core/componentmodel/behavior/ConstraintSolver.h>
#include <sofa/core/componentmodel/behavior/BaseConstraintCorrection.h>

#include <sofa/simulation/common/Node.h>
#include <sofa/simulation/common/MechanicalVisitor.h>

#include <sofa/component/linearsolver/FullMatrix.h>
#include <sofa/component/linearsolver/SparseMatrix.h>

#include <sofa/helper/set.h>
#include <sofa/helper/map.h>
#include <sofa/helper/LCPcalc.h>

namespace sofa
{

namespace component
{

namespace constraint
{

      using namespace sofa::defaulttype;
      using namespace sofa::component::linearsolver;
      using namespace helper::system::thread;


      /// Christian : WARNING: this class is already defined in sofa::helper
      class   LCP
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



      class MechanicalResetContactForceVisitor : public simulation::MechanicalVisitor
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
      class MechanicalGetConstraintValueVisitor : public simulation::MechanicalVisitor
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


class MechanicalGetConstraintInfoVisitor : public simulation::MechanicalVisitor
{
public:
    typedef core::componentmodel::behavior::BaseConstraint::PersistentID PersistentID;
    typedef core::componentmodel::behavior::BaseConstraint::ConstCoord ConstCoord;
    typedef core::componentmodel::behavior::BaseConstraint::ConstraintGroupInfo ConstraintGroupInfo;

    MechanicalGetConstraintInfoVisitor(std::vector<ConstraintGroupInfo>& groups, std::vector<PersistentID>& ids, std::vector<ConstCoord>& positions)
    : _groups(groups), _ids(ids), _positions(positions)
    {
#ifdef SOFA_DUMP_VISITOR_INFO
        setReadWriteVectors();
#endif
    }
    
    virtual Result fwdConstraint(simulation::Node* node, core::componentmodel::behavior::BaseConstraint* c)
    {
        ctime_t t0 = begin(node, c);
        c->getConstraintInfo(_groups, _ids, _positions);
        end(node, c, t0);
        return RESULT_CONTINUE;
    }
    
#ifdef SOFA_DUMP_VISITOR_INFO
    void setReadWriteVectors()
    {
    }
#endif
private:
    std::vector<ConstraintGroupInfo>& _groups;
    std::vector<PersistentID>& _ids;
    std::vector<ConstCoord>& _positions;
};


class SOFA_COMPONENT_CONSTRAINT_API LCPConstraintSolver : public sofa::core::componentmodel::behavior::ConstraintSolver
{
    typedef std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> list_cc;
    typedef std::vector<list_cc> VecListcc;
    typedef sofa::core::VecId VecId;
public:
    SOFA_CLASS(LCPConstraintSolver, sofa::core::componentmodel::behavior::ConstraintSolver);

          LCPConstraintSolver();

          void init();


          bool prepareStates(double dt, VecId);
          bool buildSystem(double dt, VecId);
          bool solveSystem(double dt, VecId);
          bool applyCorrection(double dt, VecId);   
 



          Data<bool> displayTime;
          Data<bool> initial_guess;
          Data<bool> build_lcp;
          Data<bool> multi_grid;
          Data < double > tol;
          Data < int > maxIt;
          Data < double > mu;

          Data < helper::set<int> > constraintGroups;

          Data<std::map < std::string, sofa::helper::vector<double> > > f_graph;

          LCP* getLCP();
          void lockLCP(LCP* l1, LCP* l2=0); ///< Do not use the following LCPs until the next call to this function. This is used to prevent concurent access to the LCP when using a LCPForceFeedback through an haptic thread

        private:
          std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> constraintCorrections;
          void computeInitialGuess();
          void keepContactForcesValue();

          unsigned int _numConstraints;
          double _mu;


          /// for built lcp ///
          void build_LCP();
          LCP lcp1, lcp2, lcp3; // Triple buffer for LCP.
          LPtrFullMatrix<double>  *_W;
          LCP *lcp,*last_lcp; /// use of last_lcp allows several LCPForceFeedback to be used in the same scene

          /// multi-grid approach ///
          void MultigridConstraintsMerge();
          void build_Coarse_Compliance(std::vector<int> &/*constraint_merge*/, int /*sizeCoarseSystem*/);
          LPtrFullMatrix<double>  _Wcoarse;
          std::vector< int> _contact_group;
          std::vector< int> _constraint_group;
          std::vector<int> _group_lead;
		
          /// common built-unbuilt
          simulation::Node *context;
          FullVector<double> *_dFree, *_result;
          ///
          CTime timer;
          CTime timerTotal;

          double time;
          double timeTotal;
          double timeScale;

		
          /// for unbuilt lcp ///
          void build_problem_info();
          int lcp_gaussseidel_unbuilt(double *dfree, double *f);
          int nlcp_gaussseidel_unbuilt(double *dfree, double *f);
          int gaussseidel_unbuilt(double *dfree, double *f) { if (_mu == 0.0) return lcp_gaussseidel_unbuilt(dfree, f); else return nlcp_gaussseidel_unbuilt(dfree, f); }

          SparseMatrix<double> *_Wdiag;
          //std::vector<helper::LocalBlock33 *> _Wdiag;
          std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> _cclist_elem1;
          std::vector<core::componentmodel::behavior::BaseConstraintCorrection*> _cclist_elem2;
		
    typedef core::componentmodel::behavior::BaseConstraint::PersistentID PersistentID;
    typedef core::componentmodel::behavior::BaseConstraint::ConstCoord ConstCoord;
    typedef core::componentmodel::behavior::BaseConstraint::ConstraintGroupInfo ConstraintGroupInfo;

    class ConstraintGroupBuf
    {
    public:
        std::map<PersistentID,int> persistentToConstraintIdMap;
        int nbLines; ///< how many dofs (i.e. lines in the matrix) are used by each constraint
    };

    std::map<core::componentmodel::behavior::BaseConstraint*, ConstraintGroupBuf> _previousConstraints;
    helper::vector< double > _previousForces;

    helper::vector<ConstraintGroupInfo> _constraintGroupInfo;
    helper::vector<PersistentID> _constraintIds;
    helper::vector<ConstCoord> _constraintPositions;

          // for gaussseidel_unbuilt
          helper::vector< helper::LocalBlock33 > unbuilt_W33;
          helper::vector< double > unbuilt_d;

          helper::vector< double > unbuilt_W11;
          helper::vector< double > unbuilt_invW11;
          
          bool isActive;
};

} // namespace constraint

} // namespace component

} // namespace sofa

#endif

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

#ifndef SOFA_COMPONENT_FORCEFIELD_JOINTSPRINGFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_JOINTSPRINGFORCEFIELD_H

#include <sofa/core/componentmodel/behavior/InteractionForceField.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/VisualModel.h>
#include <sofa/defaulttype/Vec.h>
#include <vector>
#include <sofa/defaulttype/Mat.h>
using namespace sofa::defaulttype;


namespace sofa
{

namespace component
{

namespace forcefield
{

template<class DataTypes>
class JointSpringForceFieldInternalData
{
public:
};

/** JointSpringForceField simulates 6D springs between Rigid DOFS
  Use kst vector to specify the directionnal stiffnesses (on each local axe)
  Use ksr vector to specify the rotational stiffnesses (on each local axe)
*/
template<class DataTypes>
class JointSpringForceField : public core::componentmodel::behavior::InteractionForceField, public core::VisualModel
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;
    typedef core::componentmodel::behavior::MechanicalState<DataTypes> MechanicalState;
    enum { N=Coord::static_size };
    typedef defaulttype::Mat<N,N,Real> Mat;
    typedef Vec<N,Real> Vec;

protected:
    class Spring
    {
    public:
        int     m1, m2;		// the two extremities of the spring: masses m1 and m2
        Vec  kst;			// spring stiffness translation on each axe
        Vec  ksr;			// spring stiffness rotation on each axe
        Real  kd;			// damping factor
        Vec  initTrans;	// rest length of the spring
        Quat  initRot;	// rest length of the spring

        Spring(int m1=0, int m2=0, Vec ks=Vec(), Real kd=0.0)
            : m1(m1), m2(m2), kd(kd), kst(ks), ksr(ks)
        {
        }

        Spring(int m1, int m2, Vec _kst, Vec _ksr, Real kd)
            : m1(m1), m2(m2), kd(kd), kst(_kst), ksr(_ksr)
        {
        }

        Spring(int m1, int m2, Real kstx, Real ksty, Real kstz, Real ksrx, Real ksry, Real ksrz, Real kd)
            : m1(m1), m2(m2), kd(kd), kst(kstx,ksty,kstz), ksr(ksrx,ksry,ksrz)
        {
        }


        inline friend std::istream& operator >> ( std::istream& in, Spring& s )
        {
            in>>s.m1>>s.m2>>s.kst>>s.ksr>>s.kd>>s.initTrans>>s.initRot;
            return in;
        }

        inline friend std::ostream& operator << ( std::ostream& out, const Spring& s )
        {
            out<<s.m1<<" "<<s.m2<<" "<<s.kst<<" "<<s.ksr<<" "<<s.kd<<" "<<s.initTrans<<" "<<s.initRot;
            return out;
        }

    };

    MechanicalState* object1;
    MechanicalState* object2;
    double m_potentialEnergy;
    DataField<Vec> kst;
    DataField<Vec> ksr;
    DataField<double> kd;
    DataField<sofa::helper::vector<Spring> > springs;
    class Loader;
    VecCoord springRef;

    JointSpringForceFieldInternalData<DataTypes> data;

    /// Accumulate the spring force and compute and store its stiffness
    void addSpringForce(double& potentialEnergy, VecDeriv& f1, const VecCoord& p1, const VecDeriv& v1, VecDeriv& f2, const VecCoord& p2, const VecDeriv& v2, int i, const Spring& spring);
    /// Apply the stiffness, i.e. accumulate df given dx
    void addSpringDForce(VecDeriv& df1, const VecDeriv& dx1, VecDeriv& df2, const VecDeriv& dx2, int i, const Spring& spring);

public:
    JointSpringForceField(MechanicalState* object1, MechanicalState* object2, Vec _kst=Vec(100.0,100.0,100.0), Vec _ksr=Vec(100.0,100.0,100.0), double _kd=5.0);
    JointSpringForceField(Vec _kst=Vec(100.0,100.0,100.0), Vec _ksr=Vec(100.0,100.0,100.0), double _kd=5.0);

    virtual void parse(core::objectmodel::BaseObjectDescription* arg);

    bool load(const char *filename);

    core::componentmodel::behavior::MechanicalState<DataTypes>* getObject1() { return object1; }
    core::componentmodel::behavior::MechanicalState<DataTypes>* getObject2() { return object2; }
    core::componentmodel::behavior::BaseMechanicalState* getMechModel1() { return object1; }
    core::componentmodel::behavior::BaseMechanicalState* getMechModel2() { return object2; }

    virtual void init();

    virtual void addForce();

    virtual void addDForce();

    virtual double getPotentialEnergy() { return m_potentialEnergy; }
    Vec getStiffnessTranslation() { return kst.getValue(); }
    Vec getStiffnessRotation() { return ksr.getValue(); }
    double getDamping() { return kd.getValue(); }
    void setStiffnessTranslation(Vec _kst) { kst.setValue(_kst); }
    void setStiffnessRotation(Vec _ksr) { ksr.setValue(_ksr); }
    void setDamping(double _kd) { kd.setValue(_kd); }

    // -- VisualModel interface
    void draw();
    void initTextures() { }
    void update() { }

    // -- Modifiers

    void clear(int reserve=0)
    {
        vector<Spring>& springs = *this->springs.beginEdit();
        springs.clear();
        if (reserve) springs.reserve(reserve);
        this->springs.endEdit();
    }

    void addSpring(int m1, int m2, Vec ks, Real kd, Vec initLentghs, Vec initAngles)
    {
        Spring s(m1,m2,ks,kd);
        s.initTrans = initLentghs;
        s.initRot = Quat::createFromRotationVector(initAngles);

        springs.beginEdit()->push_back(s);
        springs.endEdit();
    }

    void addSpring(int m1, int m2, Vec kst, Vec ksr, Real kd, Vec initLentghs, Vec initAngles)
    {
        Spring s(m1,m2,kst, ksr, kd);
        s.initTrans = initLentghs;
        s.initRot = Quat::createFromRotationVector(initAngles);

        springs.beginEdit()->push_back(s);
        springs.endEdit();
    }


    virtual std::string getTemplateName() const
    {
        return templateName(this);
    }

    static std::string templateName(const JointSpringForceField<DataTypes>* = NULL)
    {
        return DataTypes::Name();
    }

    /// Pre-construction check method called by ObjectFactory.
    template<class T>
    static bool canCreate(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
    {
        if (arg->getAttribute("object1") || arg->getAttribute("object2"))
        {
            if (dynamic_cast<MechanicalState*>(arg->findObject(arg->getAttribute("object1",".."))) == NULL)
                return false;
            if (dynamic_cast<MechanicalState*>(arg->findObject(arg->getAttribute("object2",".."))) == NULL)
                return false;
        }
        else
        {
            if (dynamic_cast<MechanicalState*>(context->getMechanicalState()) == NULL)
                return false;
        }
        return core::componentmodel::behavior::InteractionForceField::canCreate(obj, context, arg);
    }

    /// Construction method called by ObjectFactory.
    template<class T>
    static void create(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
    {
        core::componentmodel::behavior::InteractionForceField::create(obj, context, arg);
        if (arg && (arg->getAttribute("object1") || arg->getAttribute("object2")))
        {
            obj->object1 = dynamic_cast<MechanicalState*>(arg->findObject(arg->getAttribute("object1","..")));
            obj->object2 = dynamic_cast<MechanicalState*>(arg->findObject(arg->getAttribute("object2","..")));
        }
        else if (context)
        {
            obj->object1 =
                obj->object2 =
                        dynamic_cast<MechanicalState*>(context->getMechanicalState());
        }
    }
};

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif

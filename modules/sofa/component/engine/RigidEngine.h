#ifndef RIGIDENGINE_H
#define RIGIDENGINE_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/DataEngine.h>
#include <sofa/helper/vector.h>
#include <sofa/helper/Quater.h>
#include <sofa/defaulttype/RigidTypes.h>

#include <sofa/component/component.h>


namespace sofa
{

namespace component
{

namespace engine
{
/*
 * Engine which converts (vector of) Vec3 + Quaternion to give a (vector of) Rigid
 *
 */

template <class DataTypes>
class RigidEngine : public sofa::core::DataEngine
{
public:
    SOFA_CLASS(RigidEngine,sofa::core::DataEngine);

    typedef typename DataTypes::Real Real;
    typedef sofa::defaulttype::Vec<3,Real> Vec3;
    typedef sofa::helper::Quater<Real> Quat;
    typedef typename sofa::defaulttype::StdRigidTypes<3,Real>::Coord RigidVec3;

    RigidEngine();
    virtual ~RigidEngine();

    void update();
    void init();
    void reinit();

    /// Construction method called by ObjectFactory.
    template<class T>
    static void create(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
    {
        core::objectmodel::BaseObject::create(obj, context, arg);
    }

    virtual std::string getTemplateName() const
    {
        return templateName(this);
    }

    static std::string templateName(const RigidEngine<DataTypes>* = NULL)
    {
        return DataTypes::Name();
    }

    //
    Data<helper::vector<Vec3 > > f_positions;
    Data<helper::vector<Quat> > f_orientations;
    Data<helper::vector<RigidVec3> > f_rigids;
};

#if defined(WIN32) && !defined(RIGIDENGINE_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
template class SOFA_COMPONENT_ENGINE_API RigidEngine<defaulttype::Vec3dTypes>;
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_COMPONENT_ENGINE_API RigidEngine<defaulttype::Vec3fTypes>;
#endif //SOFA_DOUBLE
#endif

} // namespace engine

} // namespace component

} // namespace sofa

#endif // RIGIDENGINE_H

#include "PlaneForceField.inl"
#include "Common/Vec3Types.h"
#include "XML/DynamicNode.h"
#include "Sofa-old/Core/MechanicalObject.h"
#include "XML/ForceFieldNode.h"

namespace Sofa
{

namespace Components
{

SOFA_DECL_CLASS(PlaneForceField)

using namespace Common;

template class PlaneForceField<Vec3dTypes>;
template class PlaneForceField<Vec3fTypes>;

template<class DataTypes>
void create(PlaneForceField<DataTypes>*& obj, XML::Node<Core::ForceField>* arg)
{
	XML::createWithParent< PlaneForceField<DataTypes>, Core::MechanicalModel<DataTypes> >(obj, arg);
	if (obj!=NULL)
	{
		if (arg->getAttribute("stiffness")) obj->setStiffness((typename PlaneForceField<DataTypes>::Real)atof(arg->getAttribute("stiffness")));
		double x=0,y=0,z=0,d=0;
		if (arg->getAttribute("normal"))
			sscanf(arg->getAttribute("normal"),"%lf %lf %lf",&x,&y,&z);
		else
			z=1;
		if (arg->getAttribute("d")) d = atof(arg->getAttribute("d"));
		
		typename DataTypes::Deriv normal;
		DataTypes::set(normal,x,y,z);
		obj->setPlane(normal,(typename PlaneForceField<DataTypes>::Real)d);
	}
}

Creator< XML::ForceFieldNode::Factory, PlaneForceField<Vec3dTypes> > PlaneForceFieldVec3dClass("PlaneForceField", true);
Creator< XML::ForceFieldNode::Factory, PlaneForceField<Vec3fTypes> > PlaneForceFieldVec3fClass("PlaneForceField", true);

} // namespace Components

} // namespace Sofa

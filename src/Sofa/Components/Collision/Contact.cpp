#include "Contact.h"
#include "../Common/Factory.inl"

namespace Sofa
{

namespace Components
{

namespace Collision
{

using namespace Common;

template class Factory< std::string, Contact, std::pair<Abstract::CollisionModel*,Abstract::CollisionModel*> >;

Contact* Contact::Create(const std::string& type, Abstract::CollisionModel* model1, Abstract::CollisionModel* model2)
{
    return Factory::CreateObject(type,std::make_pair(model1,model2));
}

} // namespace Collision

} // namespace Components

} // namespace Sofa

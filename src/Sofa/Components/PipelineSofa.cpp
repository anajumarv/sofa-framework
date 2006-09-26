#include "PipelineSofa.h"
#include "Sofa/Abstract/CollisionModel.h"
#include "Graph/GNode.h"

#include "Common/ObjectFactory.h"

#include <GL/gl.h>

#define VERBOSE(a) if (getVerbose()) a; else

namespace Sofa
{

namespace Components
{

using namespace Abstract;
using namespace Common;
using namespace Collision;

void create(PipelineSofa*& obj, ObjectDescription* arg)
{
    obj = new PipelineSofa();
    obj->setVerbose(atoi(arg->getAttribute("verbose","0"))!=0);
    obj->setDraw(atoi(arg->getAttribute("draw","0"))!=0);
    obj->setDepth(atoi(arg->getAttribute("depth","6")));
}

SOFA_DECL_CLASS(PipelineSofa)

Creator<ObjectFactory, PipelineSofa> PipelineSofaClass("CollisionPipeline");

PipelineSofa::PipelineSofa()
    : verbose_(false), draw_(false), depth_(6)
{
}

typedef Graph::GNode::ctime_t ctime_t;

void PipelineSofa::doCollisionReset()
{
    Abstract::BaseContext* scene = getContext();
    Graph::GNode* node = dynamic_cast<Graph::GNode*>(scene);
    if (node && !node->getLogTime()) node=NULL; // Only use node for time logging
    ctime_t t0 = 0;
    const std::string category = "collision";

    VERBOSE(std::cout << "Reset collisions"<<std::endl);
    // clear all contacts
    if (contactManager!=NULL)
    {
        const std::vector<Contact*>& contacts = contactManager->getContacts();
        for (std::vector<Contact*>::const_iterator it = contacts.begin(); it!=contacts.end(); it++)
        {
            (*it)->removeResponse();
        }
    }
    // clear all collision groups
    if (groupManager!=NULL)
    {
        if (node) t0 = node->startTime();
        groupManager->clearGroups(scene);
        if (node) t0 = node->endTime(t0, category, groupManager, this);
    }
}

void PipelineSofa::doCollisionDetection(const std::vector<Abstract::CollisionModel*>& collisionModels)
{
    Abstract::BaseContext* scene = getContext();
    Graph::GNode* node = dynamic_cast<Graph::GNode*>(scene);
    if (node && !node->getLogTime()) node=NULL; // Only use node for time logging
    ctime_t t0 = 0;
    const std::string category = "collision";

    // clear all detection outputs
    {
        std::vector< DetectionOutput* >::iterator it = detectionOutputs.begin();
        std::vector< DetectionOutput* >::iterator itEnd = detectionOutputs.end();
        for (; it != itEnd; it++)
            delete *it;
    }
    detectionOutputs.clear();

    VERBOSE(std::cout << "Compute Bounding Trees"<<std::endl);
    // First, we compute a bounding volume for the collision model (for example bounding sphere)
    // or we have loaded a collision model that knows its other model

    std::vector<CollisionModel*> vectBoundingVolume;
    {
        if (node) t0 = node->startTime();
        const bool continuous = intersectionMethod->useContinuous();
        const double dt       = getContext()->getDt();

        std::vector<CollisionModel*>::const_iterator it = collisionModels.begin();
        std::vector<CollisionModel*>::const_iterator itEnd = collisionModels.end();
        int nActive = 0;
        for (; it != itEnd; it++)
        {
            if (!(*it)->isActive()) continue;
            if (continuous)
                (*it)->computeContinuousBoundingTree(dt, getDepth());
            else
                (*it)->computeBoundingTree(getDepth());
            vectBoundingVolume.push_back ((*it)->getFirst());
            ++nActive;
        }
        if (node) t0 = node->endTime(t0, "collision/bbox", this);
        VERBOSE(std::cout << "Computed "<<nActive<<" BBoxs"<<std::endl);
    }
    // then we start the broad phase
    if (broadPhaseDetection==NULL) return; // can't go further
    VERBOSE(std::cout << "BroadPhaseDetection "<<broadPhaseDetection->getName()<<std::endl);
    if (node) t0 = node->startTime();
    broadPhaseDetection->clearBroadPhase();
    broadPhaseDetection->addCollisionModels(vectBoundingVolume);
    if (node) t0 = node->endTime(t0, category, broadPhaseDetection, this);

    // then we start the narrow phase
    if (narrowPhaseDetection==NULL) return; // can't go further
    VERBOSE(std::cout << "NarrowPhaseDetection "<<narrowPhaseDetection->getName()<<std::endl);
    if (node) t0 = node->startTime();
    narrowPhaseDetection->clearNarrowPhase();
    std::vector<std::pair<CollisionModel*, CollisionModel*> >& vectCMPair = broadPhaseDetection->getCollisionModelPairs();
    VERBOSE(std::cout << vectCMPair.size()<<" colliding model pairs"<<std::endl);
    narrowPhaseDetection->addCollisionPairs(vectCMPair);
    if (node) t0 = node->endTime(t0, category, narrowPhaseDetection, this);

    VERBOSE(std::cout << "CollisionDetection "<<std::endl);
    // then we start the real detection between primitives
    {
        std::vector<std::pair<CollisionElementIterator, CollisionElementIterator> >& vectElemPair = narrowPhaseDetection->getCollisionElementPairs();
        std::vector<std::pair<CollisionElementIterator, CollisionElementIterator> >::iterator it4 = vectElemPair.begin();
        std::vector<std::pair<CollisionElementIterator, CollisionElementIterator> >::iterator it4End = vectElemPair.end();

        // Cache the intersector used
        ElementIntersector* intersector = NULL;
        Abstract::CollisionModel* model1 = NULL;
        Abstract::CollisionModel* model2 = NULL;

        if (node) t0 = node->startTime();
        for (; it4 != it4End; it4++)
        {
            CollisionElementIterator cm1 = it4->first;
            CollisionElementIterator cm2 = it4->second;
            if (cm1->getCollisionModel() != model1 || cm2->getCollisionModel() != model2)
            {
                model1 = cm1->getCollisionModel();
                model2 = cm2->getCollisionModel();
                intersector = intersectionMethod->findIntersector(model1, model2);
            }
            if (intersector != NULL)
            {
                DetectionOutput *detection = intersectionMethod->intersect(cm1, cm2);

                if (detection)
                    detectionOutputs.push_back(detection);
            }
        }
        if (node) t0 = node->endTime(t0, category, intersectionMethod, this);
    }
    VERBOSE(std::cout << detectionOutputs.size()<<" collisions detected"<<std::endl);
}

void PipelineSofa::doCollisionResponse()
{
    Abstract::BaseContext* scene = getContext();
    Graph::GNode* node = dynamic_cast<Graph::GNode*>(scene);
    if (node && !node->getLogTime()) node=NULL; // Only use node for time logging
    ctime_t t0 = 0;
    const std::string category = "collision";

    // then we start the creation of contacts
    if (contactManager==NULL) return; // can't go further
    VERBOSE(std::cout << "Create Contacts "<<contactManager->getName()<<std::endl);
    if (node) t0 = node->startTime();
    contactManager->createContacts(detectionOutputs);
    if (node) t0 = node->endTime(t0, category, contactManager, this);

    // finally we start the creation of collisionGroup

    const std::vector<Contact*>& contacts = contactManager->getContacts();

    // First we remove all contacts with static objects and directly add them
    std::vector<Contact*> notStaticContacts;

    for (std::vector<Contact*>::const_iterator it = contacts.begin(); it!=contacts.end(); it++)
    {
        Contact* c = *it;
        if (c->getCollisionModels().first->isStatic())
        {
            c->createResponse(c->getCollisionModels().second->getContext());
        }
        else if (c->getCollisionModels().second->isStatic())
        {
            c->createResponse(c->getCollisionModels().first->getContext());
        }
        else
            notStaticContacts.push_back(c);
    }


    if (groupManager==NULL)
    {
        VERBOSE(std::cout << "Linking all contacts to Scene"<<std::endl);
        for (std::vector<Contact*>::const_iterator it = notStaticContacts.begin(); it!=notStaticContacts.end(); it++)
        {
            (*it)->createResponse(scene);
        }
    }
    else
    {
        VERBOSE(std::cout << "Create Groups "<<groupManager->getName()<<std::endl);
        if (node) t0 = node->startTime();
        groupManager->createGroups(scene, notStaticContacts);
        if (node) t0 = node->endTime(t0, category, groupManager, this);
    }
}

void PipelineSofa::draw()
{
    if (!getDraw()) return;
    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glBegin(GL_LINES);
    std::vector< DetectionOutput* >::iterator it = detectionOutputs.begin();
    std::vector< DetectionOutput* >::iterator itEnd = detectionOutputs.end();
    for (; it != itEnd; it++)
    {
        DetectionOutput* d = *it;
        if (d->distance<0)
            glColor3f(1.0f,0.5f,0.5f);
        else
            glColor3f(0.5f,1.0f,0.5f);
        glVertex3dv(d->point[0]);
        glVertex3dv(d->point[1]);
    }
    glEnd();
    glLineWidth(1);
}
} // namespace Components

} // namespace Sofa

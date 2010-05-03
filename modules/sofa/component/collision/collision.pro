SOFA_DIR = ../../../..
TEMPLATE = lib
include($${SOFA_DIR}/sofa.cfg)
TARGET = sofacomponentcollision$$LIBSUFFIX
CONFIG += $$CONFIGLIBRARIES

CONFIG -= staticlib
CONFIG += dll

DEFINES += SOFA_BUILD_COMPONENT_COLLISION

HEADERS += initCollision.h \
           AttachBodyPerformer.h \
           AttachBodyPerformer.inl \
           BarycentricContactMapper.h \
           BarycentricContactMapper.inl \
           BarycentricPenalityContact.h \
           BarycentricPenalityContact.inl \
           BruteForceDetection.h \
           CarvingManager.h \
           ComponentMouseInteraction.h \
           ComponentMouseInteraction.inl \
           ContinuousIntersection.h \
           ContinuousTriangleIntersection.h \
           Cube.h \
           CubeModel.h \
           TopologicalChangeManager.h \
           DefaultCollisionGroupManager.h \
           DefaultContactManager.h \
           DefaultPipeline.h \
           DiscreteIntersection.h \
           DiscreteIntersection.inl \
           DistanceGridCollisionModel.h \
           FixParticlePerformer.h \
           FixParticlePerformer.inl \
           FrictionContact.h \
           FrictionContact.inl \
           InciseAlongPathPerformer.h \
           InteractionPerformer.h \
           Line.h \
           LineLocalMinDistanceFilter.h \
           LineModel.h \
           LMDNewProximityIntersection.h \
           LMDNewProximityIntersection.inl \
           LocalMinDistance.h \
	   LocalMinDistanceFilter.h \
	   LocalMinDistanceFilter.inl \
           MinProximityIntersection.h \
           MouseInteractor.h \
           MouseInteractor.inl \
           NewProximityIntersection.h \
           NewProximityIntersection.inl \
           Point.h \
	   PointLocalMinDistanceFilter.h \
           PointModel.h \
           Ray.h \
           RayContact.h \
           RayModel.h \
#           RayPickInteractor.h \
#           RayPickInteractor.inl \
           RayTriangleIntersection.h \
           RemovePrimitivePerformer.h \
           RuleBasedContactManager.h \
           SolverMerger.h \
           SpatialGridPointModel.h \
           Sphere.h \
           SphereModel.h \
           SphereTreeModel.h \
           TetrahedronModel.h \
           TreeCollisionGroupManager.h \
           Triangle.h \
	   TriangleLocalMinDistanceFilter.h \
           TriangleModel.h \
           TriangleModelInRegularGrid.h \
           RayTraceDetection.h \
           TriangleOctree.h \
           TriangleOctreeModel.h \

SOURCES += initCollision.cpp \
           AttachBodyPerformer.cpp \
           BarycentricContactMapper.cpp \
           BarycentricPenalityContact.cpp \
           BruteForceDetection.cpp \     
           ComponentMouseInteraction.cpp \
           ContinuousIntersection.cpp \
           ContinuousTriangleIntersection.cpp \
           CubeModel.cpp \
           TopologicalChangeManager.cpp \
           CarvingManager.cpp \
           DefaultCollisionGroupManager.cpp \
           DefaultContactManager.cpp \
           DefaultPipeline.cpp \
           DiscreteIntersection.cpp \
           DistanceGridCollisionModel.cpp \
           InciseAlongPathPerformer.cpp \
           LineModel.cpp \
           LMDNewProximityIntersection.cpp \
           FixParticlePerformer.cpp \
           FrictionContact.cpp \
		   InteractionPerformer.cpp \
           LineLocalMinDistanceFilter.cpp \
           LocalMinDistance.cpp \
           LocalMinDistanceFilter.cpp \
           MinProximityIntersection.cpp \
           MouseInteractor.cpp \
           NewProximityIntersection.cpp \
           PointLocalMinDistanceFilter.cpp \
           PointModel.cpp \
           RayContact.cpp \
           RayModel.cpp \
#           RayPickInteractor.cpp \  
           RayTriangleIntersection.cpp \
           RemovePrimitivePerformer.cpp \
           RuleBasedContactManager.cpp \
           SolverMerger.cpp \
           SpatialGridPointModel.cpp \
           SphereModel.cpp \
           SphereTreeModel.cpp \
           TetrahedronModel.cpp \
           TreeCollisionGroupManager.cpp \
	   TriangleLocalMinDistanceFilter.cpp \
           TriangleModel.cpp \
           TriangleModelInRegularGrid.cpp \
           RayTraceDetection.cpp \
           TriangleOctree.cpp \
           TriangleOctreeModel.cpp \
           


LIBS += $$SOFA_FRAMEWORK_LIBS
LIBS += -lsofasimulation$$LIBSUFFIX
LIBS += -lsofatree$$LIBSUFFIX
LIBS += -lsofacomponentbase$$LIBSUFFIX
LIBS += -lsofacomponentlinearsolver$$LIBSUFFIX
LIBS += -lsofacomponentodesolver$$LIBSUFFIX
LIBS += -lsofacomponentforcefield$$LIBSUFFIX
LIBS += -lsofacomponentmapping$$LIBSUFFIX
LIBS += -lsofacomponentconstraint$$LIBSUFFIX
LIBS += -lsofacomponentvisualmodel$$LIBSUFFIX
LIBS += -lsofacomponentmass$$LIBSUFFIX


LIBS += $$SOFA_EXT_LIBS 

macx : LIBS += -framework GLUT 

# Make sure there are no cross-dependencies
INCLUDEPATH -= $$SOFA_DIR/applications

include(collision-local.cfg) 

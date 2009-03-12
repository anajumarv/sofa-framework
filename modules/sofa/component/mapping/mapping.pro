SOFA_DIR = ../../../..
TEMPLATE = lib
include($${SOFA_DIR}/sofa.cfg)
TARGET = sofacomponentmapping$$LIBSUFFIX
CONFIG += $$CONFIGLIBRARIES

CONFIG -= staticlib
CONFIG += dll

DEFINES += SOFA_BUILD_COMPONENT_MAPPING

HEADERS +=  initMapping.h \
           ArticulatedSystemMapping.h \
           ArticulatedSystemMapping.inl \
           BarycentricMapping.h \
           BarycentricMapping.inl \
           BeamLinearMapping.h \
           BeamLinearMapping.inl \
           CenterOfMassMapping.h \
           CenterOfMassMapping.inl \
		   CenterPointMechanicalMapping.h \
		   CenterPointMechanicalMapping.inl \
           CurveMapping.h \
           CurveMapping.inl \
           IdentityMapping.h \
           IdentityMapping.inl \
           ImplicitSurfaceMapping.h \
           ImplicitSurfaceMapping.inl \
           LaparoscopicRigidMapping.h \
           LaparoscopicRigidMapping.inl \
           LineSetSkinningMapping.h \
           LineSetSkinningMapping.inl \
           Mesh2PointMechanicalMapping.h \
           Mesh2PointMechanicalMapping.inl \
           RigidMapping.h \
           RigidMapping.inl \
           RigidRigidMapping.h \
           RigidRigidMapping.inl \
	   SimpleTesselatedTetraMechanicalMapping.h \
	   SimpleTesselatedTetraMechanicalMapping.inl \
           SkinningMapping.h \
           SkinningMapping.inl \
           SPHFluidSurfaceMapping.h \
           SPHFluidSurfaceMapping.inl \
           SubsetMapping.h \
           SubsetMapping.inl \
           TubularMapping.h \
           TubularMapping.inl \
           VoidMapping.h

SOURCES += initMapping.cpp \
           ArticulatedSystemMapping.cpp \
           BarycentricMapping.cpp \
           BeamLinearMapping.cpp \
           CenterOfMassMapping.cpp \
		   CenterPointMechanicalMapping.cpp \
           CurveMapping.cpp \
           IdentityMapping.cpp \
           ImplicitSurfaceMapping.cpp \
           LaparoscopicRigidMapping.cpp \
           RigidMapping.cpp \
           RigidRigidMapping.cpp \
           LineSetSkinningMapping.cpp \
           Mesh2PointMechanicalMapping.cpp \
	   SimpleTesselatedTetraMechanicalMapping.cpp \
           SkinningMapping.cpp \
           SPHFluidSurfaceMapping.cpp \
           SubsetMapping.cpp \
           TubularMapping.cpp \
           VoidMapping.cpp 
           


LIBS += $$SOFA_FRAMEWORK_LIBS
LIBS += -lsofasimulation$$LIBSUFFIX
LIBS += -lsofatree$$LIBSUFFIX
LIBS += -lsofacomponentbase$$LIBSUFFIX
LIBS += -lsofacomponentforcefield$$LIBSUFFIX

LIBS += $$SOFA_EXT_LIBS 

macx : LIBS += -framework GLUT 

# Make sure there are no cross-dependencies
INCLUDEPATH -= $$SOFA_DIR/applications

include(mapping-local.cfg) 

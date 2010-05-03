SOFA_DIR = ../../../..
TEMPLATE = lib
TARGET = sofacomponentmastersolver

include($${SOFA_DIR}/sofa.cfg)

CONFIG += $$CONFIGLIBRARIES

CONFIG -= staticlib
CONFIG += dll

DEFINES += SOFA_BUILD_COMPONENT_MASTERSOLVER

HEADERS += initMasterSolver.h \
           DefaultMasterSolver.h \
           MultiStepMasterSolver.h \
           MultiTagMasterSolver.h \
           MasterContactSolver.h 

SOURCES += initMasterSolver.cpp \
           DefaultMasterSolver.cpp \
           MultiStepMasterSolver.cpp \
           MultiTagMasterSolver.cpp \
           MasterContactSolver.cpp



LIBS += $$SOFA_FRAMEWORK_LIBS
LIBS += -lsofasimulation$$LIBSUFFIX
LIBS += -lsofacomponentbase$$LIBSUFFIX
LIBS += -lsofacomponentconstraint$$LIBSUFFIX
LIBS += -lsofacomponentlinearsolver$$LIBSUFFIX
LIBS += $$SOFA_EXT_LIBS 

macx : LIBS += -framework GLUT 

# Make sure there are no cross-dependencies
INCLUDEPATH -= $$SOFA_DIR/applications

include(component-local.cfg) 

# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./modules/sofa/simulation/bgl
# Target is a library:  sofabgl$$LIBSUFFIX

HEADERS += bgl.h \
           bfs_adapter.h \
           dfs_adapter.h \
           dfv_adapter.h \
           BglNode.h \
           BglSimulation.h \
           BglGraphManager.h \
           BglGraphManager.inl \
           GetObjectsVisitor.h 

SOURCES += BglNode.cpp \   
           BglSimulation.cpp \
           BglGraphManager.cpp  \
           GetObjectsVisitor.cpp

SOFA_DIR =../../../..
TEMPLATE = lib
TARGET = sofabgl

include($${SOFA_DIR}/sofa.cfg)

CONFIG += $$CONFIGLIBRARIES

CONFIG -= staticlib
CONFIG += dll

DEFINES += SOFA_BUILD_SIMULATION_BGL

LIBS += $$SOFA_FRAMEWORK_LIBS
LIBS += -lsofasimulation$$LIBSUFFIX
LIBS += $$SOFA_EXT_LIBS

# Make sure there are no cross-dependencies
INCLUDEPATH -= $$SOFA_DIR/applications

SOFA_DIR = ../../../..
TEMPLATE = lib
TARGET = sofacomponentengine

include($${SOFA_DIR}/sofa.cfg)

CONFIG += $$CONFIGLIBRARIES

CONFIG -= staticlib
CONFIG += dll

DEFINES += SOFA_BUILD_COMPONENT_ENGINE

HEADERS += initEngine.h \
           BoxROI.h \
           BoxROI.inl \
           ExtrudeSurface.h \
           ExtrudeSurface.inl \
           GroupFilterYoungModulus.h \
           GroupFilterYoungModulus.inl \
           MergePoints.h \
           MergePoints.inl \
           MergeSets.h \
           MergeSets.inl \
	       MeshBarycentricMapperEngine.h \
	       MeshBarycentricMapperEngine.inl \
	       TransformPosition.h \
	       TransformPosition.inl \
	       TransformEngine.h \
	       TransformEngine.inl \
           PlaneROI.h \
           PlaneROI.inl \
           PointsFromIndices.h \
           PointsFromIndices.inl \
           ValuesFromIndices.h \
           ValuesFromIndices.inl \
           IndicesFromValues.h \
           IndicesFromValues.inl \
           JoinPoints.h \
           JoinPoints.inl \
           MapIndices.h \
           MapIndices.inl \
	       PrimitivesInSphereROI.h \
	       PrimitivesInSphereROI.inl \
           RandomPointDistributionInSurface.h \
           RandomPointDistributionInSurface.inl \
           Spiral.h \
           Spiral.inl \
           TrianglesInBoxROI.h \
           TrianglesInBoxROI.inl \
           TrianglesInSphereROI.h \
           TrianglesInSphereROI.inl \
           TrianglesInPlaneROI.h \
           TrianglesInPlaneROI.inl \
           Vertex2Frame.h \
           Vertex2Frame.inl \
           TextureInterpolation.h \
           TextureInterpolation.inl

SOURCES += initEngine.cpp \
           BoxROI.cpp \
           ExtrudeSurface.cpp \
           GroupFilterYoungModulus.cpp \
           MergePoints.cpp \
           MergeSets.cpp \
	   MeshBarycentricMapperEngine.cpp \
           PlaneROI.cpp \
	  	   PrimitivesInSphereROI.cpp \
	  	   TransformPosition.cpp \
           TransformEngine.cpp \
           PointsFromIndices.cpp \
           ValuesFromIndices.cpp \
           IndicesFromValues.cpp \
           JoinPoints.cpp \
           MapIndices.cpp \
           RandomPointDistributionInSurface.cpp \
           Spiral.cpp \
           TrianglesInBoxROI.cpp \
           TrianglesInSphereROI.cpp \
           TrianglesInPlaneROI.cpp \
           Vertex2Frame.cpp \
           TextureInterpolation.cpp
           
           

LIBS += $$SOFA_FRAMEWORK_LIBS
LIBS += -lsofacomponentbase$$LIBSUFFIX
LIBS += -lsofacomponentcollision$$LIBSUFFIX
LIBS += -lsofasimulation$$LIBSUFFIX
LIBS += $$SOFA_EXT_LIBS

macx : LIBS += -framework GLUT 

# Make sure there are no cross-dependencies
INCLUDEPATH -= $$SOFA_DIR/applications

include(engine-local.cfg) 

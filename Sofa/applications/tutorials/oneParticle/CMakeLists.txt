include(${SOFA_CMAKE_DIR}/preProject.cmake)

set(SOURCE_FILES

    oneParticle.cpp

    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

AddLinkerDependencies(SofaSimulationTree SofaComponentMain SofaGuiMain)

if(SOFA-LIB_SIMULATION_GRAPH_DAG)
    AddLinkerDependencies(SofaSimulationGraph)
endif()


include(${SOFA_CMAKE_DIR}/postProject.cmake)


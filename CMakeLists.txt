cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(cy)


option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


add_executable(cylinder cylinder.cc ${sources} ${headers})
target_link_libraries(cylinder ${Geant4_LIBRARIES})


set(REFLECT_SCRIPTS
  init_vis.mac
  gui.mac
  alpha.mac
  beta.mac
  Bi214.mac
  chamber.mac
  Co60.mac
  Cs137.mac
  gamma.mac
  muon.mac
  muon1.mac
  Na22.mac
  Pb214.mac
  Po218.mac
  Rn222.mac
  test.mac
  vis.mac
  )


foreach(_script ${REFLECT_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


install(TARGETS cylinder DESTINATION bin)

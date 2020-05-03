PROJECT(CPU)

FIND_PACKAGE(TBB CONFIG REQUIRED)

LINK_LIBRARIES(TBB::tbb)

ADD_COMPILE_DEFINITIONS(
	PROJECT_DIR="${PROJECT_SOURCE_DIR}/"
)

IF(WIN32)
	ADD_COMPILE_OPTIONS(
		/wd4244  # int to float may lose precision Yes, yes, I know, it is on purpose
		/Qvec-report:1
	)
ELSEIF(UNIX)
	ADD_COMPILE_OPTIONS(-Wno-unused-result) # Why do we need the return value from fscanf() or system()?
ENDIF()

ADD_EXECUTABLE(${PROJECT_NAME}_Demo ${CMAKE_SOURCE_DIR}/example/Demo.cpp)
ADD_EXECUTABLE(${PROJECT_NAME}_AttackExperiment ${CMAKE_SOURCE_DIR}/example/AttackExperiment.cpp)
ADD_EXECUTABLE(${PROJECT_NAME}_TwitterExperiment ${CMAKE_SOURCE_DIR}/example/TwitterExperiment.cpp)
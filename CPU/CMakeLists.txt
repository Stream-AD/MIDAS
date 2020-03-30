PROJECT(CPU)

ADD_COMPILE_DEFINITIONS(
	PROJECT_DIR="${PROJECT_SOURCE_DIR}/"
)

IF(WIN32)
	ADD_COMPILE_OPTIONS(
		/wd4244  # int to float may lose precision Yes, yes, I know, it is on purpose
		/Qvec-report:1  # Just to make the vectorization magic verbose, removing this to let the compiler be quiet
	)
ELSEIF(UNIX)
	ADD_COMPILE_OPTIONS(-Wno-unused-result) # Why do we need the return value from fscanf() or system()?
ENDIF()

ADD_EXECUTABLE(${PROJECT_NAME}_Demo ${CMAKE_SOURCE_DIR}/example/Demo.cpp)
ADD_EXECUTABLE(${PROJECT_NAME}_Experiment ${CMAKE_SOURCE_DIR}/example/Experiment.cpp)
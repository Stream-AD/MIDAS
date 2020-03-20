PROJECT(CPU)

ADD_COMPILE_DEFINITIONS(
	PROJECT_DIR="${PROJECT_SOURCE_DIR}/"
)

ADD_COMPILE_OPTIONS(
	/wd4244 # int to float may lose precision Yes, yes, I know, it is on purpose
)

ADD_EXECUTABLE(${PROJECT_NAME}_Demo ${CMAKE_SOURCE_DIR}/example/Demo.cpp)
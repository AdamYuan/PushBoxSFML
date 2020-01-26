project(PushBox)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(dep)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

file(GLOB DEP_FILES
	dep/imgui/*.*
	dep/sqlite/*.*
	)

add_executable(PushBox 
	${DEP_FILES}
	src/Config.hpp
	src/DataBase.hpp
	src/Error.hpp
	src/Level.hpp
	src/LevelSwitch.hpp
	src/LevelSaveThread.hpp
	src/Logic.hpp
	src/Renderer.hpp
	src/Resource.hpp
	src/main.cpp
	)
target_link_libraries(PushBox
	${CMAKE_DL_LIBS}
	${OPENGL_opengl_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
	sfml-system sfml-graphics sfml-window)

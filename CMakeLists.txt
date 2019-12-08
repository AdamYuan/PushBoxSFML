project(PushBox)

set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(dep)

file(GLOB SOURCE_FILES
	src/*.*
	dep/imgui/*.*
	dep/sqlite/*.*
	)

add_executable(PushBox ${SOURCE_FILES})
target_link_libraries(PushBox ${CMAKE_DL_LIBS} pthread GL sfml-system sfml-graphics sfml-window)

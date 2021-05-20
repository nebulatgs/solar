cmake_minimum_required(VERSION 3.10)

project(solar)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(solar 
			   src/solar.cpp
			   src/game.cpp
			   src/partsys.cpp
			   src/shader.cpp
			   src/bcontroller.cpp
			   src/particle.cpp
			   )

# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set_target_properties(solar PROPERTIES COMPILE_FLAGS "-std=c++17 -O3 -fexceptions -s USE_PTHREADS=1")
set_target_properties(solar PROPERTIES LINK_FLAGS "
						-std=c++17 
						-O3
						-flto 
						-s ASSERTIONS=1
						
						
						-s USE_GLFW=3 
						-s MAX_WEBGL_VERSION=2 
						-s MIN_WEBGL_VERSION=2 
						-s WASM=1
						-s TOTAL_MEMORY=256MB 
						
						-s ENVIRONMENT=web,worker
						-s DISABLE_EXCEPTION_CATCHING=0
						-s USE_PTHREADS=1
						-s PTHREAD_POOL_SIZE=8
						--bind
						--preload-file ../shaders")
# add_subdirectory("${PROJECT_SOURCE_DIR}/../glfw" "${PROJECT_SOURCE_DIR}/../glfw/build")
# add_subdirectory("${PROJECT_SOURCE_DIR}/lib" "${PROJECT_SOURCE_DIR}/lib/build")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glm" "${PROJECT_SOURCE_DIR}/lib/glm/build")

# include_directories("${PROJECT_SOURCE_DIR}/../glfw/include")
# include_directories("${PROJECT_SOURCE_DIR}/lib/include")
include_directories("${PROJECT_SOURCE_DIR}/lib/glm")

# target_link_libraries(solar glfw ${GLFW_LIBRARIES})
# target_link_libraries(solar glfw)
# target_link_libraries(solar glad)
target_link_libraries(solar glm)

add_custom_command(TARGET solar
  POST_BUILD
  COMMAND /usr/bin/cp solar.* ..
  )

# find_package(GLUT REQUIRED)
# include_directories(${GLUT_INCLUDE_DIR})
# target_link_libraries(solar GL)
# target_link_libraries(solar glut)
# find_package(GLEW REQUIRED)
# target_link_libraries(solar GLEW::GLEW)

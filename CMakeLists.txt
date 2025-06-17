cmake_minimum_required(VERSION 3.16)
project(Drawing_Spells_Simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/vcpkg")

# Add your source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Add your includes directory (optional, your own headers)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
)

# Add EnTT include path manually from vcpkg_installed
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include
        ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib
)

find_package(raylib CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
# Copy runtime DLLs from vcpkg bin folder
install(FILES "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/raylib.dll" DESTINATION .)
install(FILES "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/glfw3.dll" DESTINATION .)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/spells" DESTINATION .)

#cmake_minimum_required(VERSION 3.16)
#project(Drawing_Spells_Simulator)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

## Add your source files
#file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

#add_executable(${PROJECT_NAME} ${SOURCES})

# Add your includes directory (optional, your own headers)
#target_include_directories(${PROJECT_NAME}
#    PRIVATE
#        ${CMAKE_SOURCE_DIR}/includes
#)

# Add EnTT include path manually from vcpkg_installed
#target_include_directories(${PROJECT_NAME}
#    PRIVATE
#        ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include
#        ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/lib
#)

#find_package(raylib CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/raylib.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/glfw3.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
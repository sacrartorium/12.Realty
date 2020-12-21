cmake_minimum_required(VERSION 3.5)

set (APPLICATION_NAME realty)

project(${APPLICATION_NAME} VERSION $ENV{TRAVIS_BUILD_NUMBER})

configure_file(src/version.hpp.in src/version.hpp)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true src/*.hpp)

set(SOURCES ${SOURCES})

add_executable(rclst ${SOURCES} src/rclst.cpp)
target_link_libraries(rclst dlib openblas)
set_target_properties(rclst PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(rclst
    PRIVATE "${CMAKE_BINARY_DIR}"
)

add_executable(rclss ${SOURCES} src/rclss.cpp)
target_link_libraries(rclss dlib openblas)
set_target_properties(rclss PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(rclss
    PRIVATE "${CMAKE_BINARY_DIR}"
)

FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
    message(STATUS "Doxygen is found.")

    set(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR}/src)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doc)
    set(DOXYGEN_RECURSIVE NO)
    set(DOXYGEN_PROJECT_NAME ${CMAKE_PROJECT_NAME})
    set(DOXYGEN_PROJECT_NUMBER $ENV{TRAVIS_BUILD_NUMBER})
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_HAVE_DOT YES)

    configure_file(doxygen.conf.in doxygen.conf)
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf)
else ()
    message(STATUS "Doxygen is not found.")
endif()

if (MSVC)
    target_compile_options(rclst PRIVATE
        /W4
    )
    target_compile_options(rclss PRIVATE
        /W4
    )
else ()
    target_compile_options(rclst PRIVATE
        -Wall -Wextra -pedantic -O0 -g3
    )
    target_compile_options(rclss PRIVATE
        -Wall -Wextra -pedantic -O0 -g3
    )
endif()

install(TARGETS rclst RUNTIME DESTINATION bin)
install(TARGETS rclss RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT sacrartorium@example.com)

include(CPack)

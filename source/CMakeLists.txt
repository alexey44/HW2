cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

find_package(GTest REQUIRED)

SET(TARGET_H
    IpFilter.h
   )

SET(TARGET_SRC
    IpFilter.cpp
    )

add_executable(ip_filter ip_filter.cpp ${TARGET_H} ${TARGET_SRC})
add_executable(gtest_ip_filter ip_filter_gtest.cxx ${TARGET_H} ${TARGET_SRC})

set_target_properties(ip_filter 
                      gtest_ip_filter
                      PROPERTIES
                      CXX_STANDARD 11
                      CXX_STANDARD_REQUIRED ON
)

target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(gtest_ip_filter GTest::GTest GTest::Main)
#////////////////////////////////////////////////
target_compile_options(ip_filter PRIVATE
    -Wall -Wextra -pedantic -Werror
)
target_compile_options(gtest_ip_filter PRIVATE
    -Wall -Wextra -pedantic -Werror
)
#////////////////////////////////////////////////

install(TARGETS ip_filter RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
message(${PROJECT_VERSION_PATCH})

enable_testing()
add_test(gtest_ip_filter gtest_ip_filter)

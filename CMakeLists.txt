cmake_minimum_required(VERSION 3.25)
project(get_geo_ip C)

set(CMAKE_C_STANDARD 17)

# Set Homebrew paths
set(JSON_C_ROOT /opt/homebrew/opt/json-c)
set(CURL_ROOT /opt/homebrew/opt/curl)

# Create static library for Rust FFI
add_library(get_geo_ip_lib STATIC
        library.c
        utils/json_parse.c
        utils/validate_ip.c
        utils/resolve_hostname.c
        utils/resolve_hostname.h
)


# Include directories for static library
target_include_directories(get_geo_ip_lib PRIVATE
        ${JSON_C_ROOT}/include
        ${CURL_ROOT}/include
)

# Link directories and libraries for static library
target_link_directories(get_geo_ip_lib PRIVATE
        ${JSON_C_ROOT}/lib
        ${CURL_ROOT}/lib
)

target_link_libraries(get_geo_ip_lib json-c curl)

# Optional: Add executable for testing
add_executable(get_geo_ip main.c)
target_include_directories(get_geo_ip PRIVATE
        ${JSON_C_ROOT}/include
        ${CURL_ROOT}/include
)
target_link_directories(get_geo_ip PRIVATE
        ${JSON_C_ROOT}/lib
        ${CURL_ROOT}/lib
)
target_link_libraries(get_geo_ip get_geo_ip_lib json-c curl)
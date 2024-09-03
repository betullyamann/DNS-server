cmake_minimum_required(VERSION 3.5.0)
project(dns_server VERSION 0.1.0 LANGUAGES C CXX)

include_directories("${CMAKE_SOURCE_DIR}/inc")

# List of source files
set(SOURCES 
    "src/main.cpp"
    "src/deserialize.cpp "
    "src/DNSReadBuffer.cpp "
    "src/DNSResource.cpp "
)

# Add executable target using globbing to find all source files in src directory
# file(GLOB SOURCES "src/*.cpp")

add_executable(DNS_server ${SOURCES})


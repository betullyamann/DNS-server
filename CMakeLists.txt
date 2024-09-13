cmake_minimum_required(VERSION 3.5.0)
project(dns_server VERSION 0.1.0 LANGUAGES C CXX)

include_directories("${CMAKE_SOURCE_DIR}/inc", "${CMAKE_SOURCE_DIR}/test")

# List of source files
set(SOURCES 
    "src/deserialize.cpp"
    "src/DNSReadBuffer.cpp"
    "src/DNSResource.cpp"
    "src/serialize.cpp"
    "src/DNSWriteBuffer.cpp"
)

set(TESTS 
    "test/test_deserialization.cpp"
    "test/test_serialization.cpp"
)

# Add executable target using globbing to find all source files in src directory
# file(GLOB SOURCES "src/*.cpp")

add_executable(DNS_server "test/main.cpp" ${SOURCES} ${TESTS})


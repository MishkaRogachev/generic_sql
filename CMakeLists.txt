# Project
project(generic_sql)

if (WIN32)
    add_definitions(-DUNITS_DLL)
endif ()

# Target
add_library(${PROJECT_NAME} SHARED "")

# Include
HEADER_DIRECTORIES(INCLUDES "include")
HEADER_DIRECTORIES(INTERNAL_INCLUDES "src")
target_include_directories(${PROJECT_NAME}
    PUBLIC ${INCLUDES}
    PRIVATE ${INTERNAL_INCLUDES}
)

# Sources
file(GLOB_RECURSE SOURCES "include/*" "src/*")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

# Libraries
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)

# Tests
if(WITH_TESTS)
    add_subdirectory(tests)
endif(WITH_TESTS)

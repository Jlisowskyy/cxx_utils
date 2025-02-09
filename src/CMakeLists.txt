# ------------------------------
# Find all source files
# ------------------------------

file(GLOB_RECURSE LIB_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Ensure main.cpp removed from lib sources
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# ------------------------------
# Define static library
# ------------------------------

add_library(${LIB_NAME} STATIC
        ${LIB_SOURCES}
)

# ------------------------------
# Add includes
# ------------------------------

target_include_directories(${LIB_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

# ------------------------------
# Define test exec
# ------------------------------

add_executable(test_exec
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

target_link_libraries(test_exec PRIVATE
        ${LIB_NAME}
)

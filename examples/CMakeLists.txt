message(STATUS "Build ${PROJECT_NAME} examples enabled")

set(TARGET_NAME_HEAD ${PROJECT_NAME}_example)

#

set(TARGET_NAME ${TARGET_NAME_HEAD}_1)
add_executable(${TARGET_NAME} console)
target_link_libraries(${TARGET_NAME} ${PROJECT_NAME})

file(GLOB root_source "*.cpp")
file(GLOB_RECURSE source "*.cpp")

add_executable(qbe ${root_source} ${source})

target_link_libraries(qbe SDL2 GL)

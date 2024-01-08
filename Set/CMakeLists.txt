set(lib_name HidMixer-Set)
set(src_dir ${CMAKE_CURRENT_LIST_DIR}/src)

add_library(${lib_name} INTERFACE)

target_sources(${lib_name} INTERFACE
    ${src_dir}/HidMixer/Set.cpp
)

target_include_directories(${lib_name} INTERFACE
    ${src_dir}
)

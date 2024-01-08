set(lib_name HidMixer-HidReporter)
set(src_dir ${CMAKE_CURRENT_LIST_DIR}/src)

add_library(${lib_name} INTERFACE)

target_include_directories(${lib_name} INTERFACE
    ${src_dir}
)

target_link_libraries(${lib_name} INTERFACE 
    tinyusb_device
)

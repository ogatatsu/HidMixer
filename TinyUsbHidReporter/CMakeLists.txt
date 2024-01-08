set(lib_name HidMixer-TinyUsbHidReporter)
set(src_dir ${CMAKE_CURRENT_LIST_DIR}/src)

add_library(${lib_name} INTERFACE)

target_sources(${lib_name} INTERFACE
    ${src_dir}/HidMixer/TinyUsbHidReporterDetail/TinyUsbHidReporter.cpp
)

target_include_directories(${lib_name} INTERFACE
    ${src_dir}
)

target_link_libraries(${lib_name} INTERFACE 
    HidMixer-HidReporter
    FreeRTOS-Kernel-Core
    tinyusb_device
)

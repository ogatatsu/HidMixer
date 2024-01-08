set(lib_name HidMixer-HidEngine)
set(src_dir ${CMAKE_CURRENT_LIST_DIR}/src)

add_library(${lib_name} INTERFACE)

target_sources(${lib_name} INTERFACE
    ${src_dir}/HidMixer/HidEngineDetail/ActionImpl.cpp
    ${src_dir}/HidMixer/HidEngineDetail/ActionTapper.cpp
    ${src_dir}/HidMixer/HidEngineDetail/HidCore.cpp
    ${src_dir}/HidMixer/HidEngineDetail/HidEngine.cpp
    ${src_dir}/HidMixer/HidEngineDetail/HidEngineTask.cpp
    ${src_dir}/HidMixer/HidEngineDetail/TimerMixin.cpp
    ${src_dir}/HidMixer/HidEngineDetail/types.cpp
)

target_include_directories(${lib_name} INTERFACE 
    ${src_dir}
)

target_link_libraries(${lib_name} INTERFACE 
    HidMixer-Set
    HidMixer-HidReporter
    etl
    gsl-lite
    FreeRTOS-Kernel-Core
    consthash
)

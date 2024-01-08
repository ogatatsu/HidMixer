
cmake_minimum_required(VERSION 3.1.4)
project(HidMixer)

add_subdirectory(deps/consthash)
add_subdirectory(deps/etl)
add_subdirectory(deps/gsl-lite)
add_subdirectory(HidEngine)
add_subdirectory(UsbAction)
add_subdirectory(HidReporter)
add_subdirectory(TinyUsbHidReporter)
add_subdirectory(Set)

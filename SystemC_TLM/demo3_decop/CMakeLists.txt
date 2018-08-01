ADD_EXECUTABLE(tlm_demo3_decop
sc_main.cpp
../demo2/memory.h
../demo2/memory.cpp
../demo2/processor.h
../demo2/processor.cpp
processor0.h
processor0.cpp
processor1.h
processor1.cpp
../demo3_sync/bus.h
)
target_link_libraries( tlm_demo3_decop
${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)

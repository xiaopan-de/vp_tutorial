ADD_EXECUTABLE(tlm_demo3_sync
sc_main.cpp
../tlm_demo2/memory.h
../tlm_demo2/memory.cpp
../tlm_demo2/processor.h
../tlm_demo2/processor.cpp
processor0.h
processor0.cpp
processor1.h
processor1.cpp
bus.h
)
target_link_libraries( tlm_demo3_sync
${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)

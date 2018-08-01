ADD_EXECUTABLE(tlm_demo2
sc_main.cpp
memory.h
processor.h
memory.cpp
processor.cpp
)
target_link_libraries( tlm_demo2
${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)

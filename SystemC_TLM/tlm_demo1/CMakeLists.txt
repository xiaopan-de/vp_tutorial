ADD_EXECUTABLE(tlm_demo1
sc_main.cpp
initiator.h
target.h
)
target_link_libraries( tlm_demo1
${SYSTEMC_LIBRARIES}
${SYSTEMCAMS_LIBRARIES}
)

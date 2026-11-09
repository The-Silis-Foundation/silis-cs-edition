# CMake generated Testfile for 
# Source directory: /home/eatheswar/silisfoundation/silis/third-party/OpenSTA/dcalc/test
# Build directory: /home/eatheswar/silisfoundation/silis/core/synthengine/build/OpenSTA_build/dcalc/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(tcl.dcalc.prima_report "bash" "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/test/regression.sh" "/home/eatheswar/silisfoundation/silis/core/synthengine/build/OpenSTA_build/sta" "dcalc_prima_report")
set_tests_properties(tcl.dcalc.prima_report PROPERTIES  LABELS "tcl;module_dcalc" WORKING_DIRECTORY "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/dcalc/test" _BACKTRACE_TRIPLES "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/CMakeLists.txt;708;add_test;/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/dcalc/test/CMakeLists.txt;1;sta_module_tests;/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/dcalc/test/CMakeLists.txt;0;")
subdirs("cpp")

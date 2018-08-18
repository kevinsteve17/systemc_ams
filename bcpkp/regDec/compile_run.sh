#!/bin/sh
echo 'Compiling *.c *cpp files'
rm -rf voltage.o
export SYSTEMC_HOME=/usr/local/systemc-2.3.2/
export LD_LIBRARY_PATH=/usr/local/systemc-ams-2.1/lib-linux64/:$SYSTEMC_HOME/lib-linux64/
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 -I/usr/local/systemc-ams-2.1/include -L/usr/local/systemc-ams-2.1/lib-linux64 tb_regdec.cpp instRegDec.cpp -lsystemc -lsystemc-ams -lm -o voltage.o
echo 'Simulation Started'
./voltage.o
echo 'Simulation Ended'
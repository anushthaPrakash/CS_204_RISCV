/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor


*/


/* main.cpp 
   Purpose of this file: The file handles the input and output, and
   invokes the simulator
*/


// #include <bits/stdc++.h>
#include<bits/stdc++.h>
#include "myRISCVSim.h"
using namespace std;

int main()
{
  #ifndef ONLINE_JUDGE
    freopen("output_log.mem", "w", stdout);
  #endif
  // reset the processor
  reset_proc();
  // load the program memory
  load_program_memory();
  // run the simulator
  run_riscvsim();
  return 0;
}
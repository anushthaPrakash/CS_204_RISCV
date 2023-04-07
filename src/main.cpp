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
// #include "myRISCVSim.cpp"
#include "StalledPipeline.cpp"
// #include "SingleCycle.cpp"
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


// Some derivation in data dependencies RAW 
// increase one cycle number 
// when in pipeline 3 stalls instruction is encountered first 3 stalls are shown in pipeline the 2 the 1 which is not the case 
// first 1 stall come then 2 then 3
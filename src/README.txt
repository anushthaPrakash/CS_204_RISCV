================================================
Functional Simulator for subset of ARM Processor
================================================

README

Table of contents
1. Directory Structure
2. Getting Started: Installation and Running 
3. Output Format
4. Contributers

Phase 1
-----------

Directory Structure:
--------------------
CS112-Project
  |
  |- bin
      |
      |- myARMSim
  |- doc
      |
      |- design-doc-phase1.docx
  |- include
      |
      |- myARMSim.h
  |- pics
      |
      |- 1.jpeg
      |- 2.jpeg
      |- logo.png
  |- src
      |- main.cpp
      |- Makefile
      |- myARMSim.cpp
      |- D_Memory.mem
      |- data_out.mem
      |- gui.py
      |- inp.py
      |- input.mem
      |- output_log.mem
      |- README.txt
      |- register.mem
      |- test_case.mem
  |- arraysum.mem
  |- bubblesort.mem
  |- fibonacci.mem
  |- Project-statement.txt
  |- README.md


Getting Started: Installation and running
-------------------------------------------
  Prerequisites
  - pip (>21.0.3)
  - python (>3.7)

  Libraries Used
    Back-end - Python3 and G++
      - os:  for getting and adding path to certain file locations.
      - sys:  for reading and editing files with ease.
    Front-end - Python3
      - PyQT5:  for the Graphic User Interface.
      - qdarkstyle:  for dark theme
  
    Running the GUI version
    --------------------------
      - Your computer should have Python3, G++ and Makefile installed.You can check if they are installed or not by using the following commands
            
            python3 --version
            g++ --version 
            make --version
      - Download the zip file of the repository
      - cd into the CS_204_RISCV then to src
            cd CS_204_RISCV/src
      - Now run the following command in terminal
            make 
      - The above commands will open the GUI. You have to press ASSEMBLE, which will compile and run the cpp files.
      - Wait for the message "Ready to run" and then press the RUN button.
      - This will print the output log, update the register values and data memory in the GUI.
    
    Running the Basic version without GUI
    ---------------------------------------
      - Type the following command after cd into the src folder
            g++ main.cpp myRISCVSim.cpp -I ../include
      - This will run the program and output the output log in output_log.mem, data memory in D_memory.mem and register values in register.mem

    Feeding Input to the program
    -------------------------------
       - The src Folder contains a test_case.mem file in which the machine code you want to run will go in the following format (containing the instruction code as well as the pc).The exit code will be 0xEF000011
            0x0 0x00500513
            0x4 0x008000EF
            0x8 0x0440006F
       - The machine code in this format for the 3 test cases i.e. bubblesort (bubblesort.mem), sum of array element (arraysum.mem) and fibonacci (fibonacci.mem) are provided in the root directory. You can copy-paste the code from there to test_case.mem file in src folder for running them. 
       - If you want to run any code written in assembly, Go to venus simulator (online), copy-paste the dump in the input.py file in src folder, then run the inp.py file. It will convert the dump into the required assembly format and write it in the test_case.mem file. You can run the code to see the output.

Output Format
-----------------
   Check the generated folder for details of compilation. It contains:
       output_log.mem : contains general stats about the different stages like fetch,decode,ALU,memory access,register writeback and details of changes in temporary registers for each cycle
       data_out.mem: details of instruction memory
       register.mem: details of registers
       D_Memory.mem: details of data memory


Phase 2
----------

Directory Structure:
--------------------
CS112-Project
  |
  |- bin
      |
      |- myARMSim
  |- doc
      |
      |- design-doc-phase1.docx
      |- design-doc-phase2.docx
  |- include
      |
      |- myARMSim.h
  |- pics
      |
      |- 1.jpeg
      |- 2.jpeg
      |- logo.png
  |- src
      |- main.cpp
      |- CH_Cycle.mem
      |- CountData.mem
      |- D_Memory.mem
      |- data_out.mem
      |- DH_Cycle.mem
      |- SingleCycle.cpp
      |- StalledPipeline.cpp
      |- main.cpp
      |- Makefile
      |- myARMSim.cpp
      |- D_Memory.mem
      |- data_out.mem
      |- gui.py
      |- inp.py
      |- input.mem
      |- output_log.mem
      |- README.txt
      |- register.mem
      |- test_case.mem
  |- arraysum.mem
  |- bubblesort.mem
  |- fibonacci.mem
  |- Project-statement.txt
  |- README.md

File Contents
----------------
  - SingleCycle.cpp has single cycle execution.
  - StalledPipeline.cpp has pipeline execution with Stalls
  - myRISCVSim.cpp has pipeline with Branch Prediction and data forwarding

Execution
-----------
  - In case of running without GUI, in main.cpp just uncomment the header file corresponding to the one you want to run and comment rest.
  - Now see the output_log.mem
Contributers
----------------
  - Anushtha Prakash         2021CSB1071
  - Arpit Kumar Gautam       2021CSB1073
  - Aryaman Gupta            2021CSB1074
  - Ashima Goyal             2021CSB1075
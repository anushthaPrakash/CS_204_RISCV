<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
    <br />
<p align="center">
    <img src="pics/logo_new.png" alt="Logo" width="580" height="200">

  <h3 align="center">RISC-V Simulator</h3>

  <p align="center">
    A C++ and Python implementation of RISC-V Simulation.
    <br />
    <a href="https://github.com/anushthaPrakash/CS_204_RISCV/blob/main/src/README.txt"><strong>Explore the docs »</strong></a>
    
  </p>
</p>
<h1  style="color:blue;text-align:center" ><strong>CS204 Project:RISC-V Assembler</strong></h1>

<p>The aim of this project is to simulate the machine level execution of RISC-V as well as the execution of RISC-V 32-bit instructions using a high level 
language. The Project also aims to give final updates to the user regarding each step of the execution of the program. It also returns the final status of 
the memory and registers as output for the user to analyse the working of their programs thoroughly. The Project currently allows the user to use 26 
different instructions and can be extended to allow the use of any number of instructions as long as the instructions are supported by 32-bit RISC-V ISA. The 
program executes each instruction using five stages as described in the RISC-V architecture. The instruction memory size is upto 1000 instructions.</p>

<br/>
<h2>Table of Contents</h2>
<ul>
  <li><a href="#getting-started">Getting Started: Installation and Running</a></li>
  <li><a href="#screenshoots">Screenshots</a></li>
  <li><a href="#contributors">Contributors</a></li>
</ul>
<br>
<h2 id="getting-started">Getting Started: Installation and running</h2>

### Prerequisites
- `pip` (>21.0.3)
- `python` (>3.7)

### Libraries Used

#### Back-end - Python3 and G++
- `os: ` for getting and adding path to certain file locations.
- `sys: ` for reading and editing files with ease.
#### Front-end - Python3
- `PyQT5: ` for the Graphic User Interface.
- `qdarkstyle: ` for dark theme

<br>
<h3>Running the GUI version</h3>
<ul>
  <li>Your computer should have Python3, G++ and Makefile installed.You can check if they are installed or not by using the following commands</li>
  <pre><code>
    python3 --version
    g++ --version 
    make --version
 </code></pre>
  <li>Download the zip file of the repository</li>
  <li>cd into the CS_204_RISCV then to src</li>
  <pre><code>
   cd CS_204_RISCV/src
 </code></pre>
  <li>Now run the following command in terminal </li>
  <pre><code>
    make
  </code></pre>
</ul>
<ul>
  <li>The above commands will open the GUI. You have to press ASSEMBLE, which will compile and run the cpp files. </li>
  <li>Wait for the message "Ready to run" and then press the RUN button. </li>

  <li>This will print the output log, update the register values and data memory in the GUI.</li>
</ul>
<h3>Running the Basic version without GUI</h3>
<ul>
  <li>Type the following command after cd into the src folder</li>
  <pre><code>
    g++ main.cpp myRISCVSim.cpp -I ../include
  </code></pre>
  <li>This will run the program and output the output log in output_log.mem, data memory in D_memory.mem and register values in register.mem</li>
</ul>
<h3>Feeding Input to the program</h3>
<ul>
  <li>The src Folder contains a test_case.mem file in which the machine code you want to run will go in the following format (containing the instruction code as well as the pc).The exit code will be 0xEF000011 </li>
  <pre><code>
    0x0 0x00500513
    0x4 0x008000EF
    0x8 0x0440006F
  </code></pre>
  <li>The machine code in this format for the 3 test cases i.e. bubblesort (bubblesort.mem), sum of array element (arraysum.mem) and fibonacci (fibonacci.mem) are provided in the root directory. You can copy-paste the code from there to test_case.mem file in src folder for running them.</li>
  <li>If you want to run any code written in assembly, Go to venus simulator (online), copy-paste the dump in the input.py file in src folder, then run the inp.py file. It will convert the dump into the required assembly format and write it in the test_case.mem file. You can run the code to see the output.</li>
</ul>
<br>

<h3>Output Format</h3>

Check the generated folder for details of compilation. It contains:

- `output_log.mem :` contains general stats about the different stages like fetch,decode,ALU,memory access,register writeback and details of changes in temporary registers for each cycle
- `data_out.mem:` details of instruction memory
- `register.mem:` details of registers
- `D_Memory.mem:` details of data memory

<h2 id="screenshoots">ScreenShots</h2>

<h3>Linux View</h3>
<p align="center">
  <img src="pics/1.jpeg" alt="Logo" width="1080" height="500">
</p>
<p align="center">
  <img src="pics/2.jpeg" alt="Logo" width="1080" height="500">
</p>

<h3>Windows View</h3>
<p align="center">
  <img src="pics/3.png" alt="Logo" width="1080" height="500">
</p>
<p align="center">
  <img src="pics/4.png" alt="Logo" width="1080" height="500">
</p>

<h2 id="contributors">Contributors</h2>

<div align="center">
  <strong>
    <a href="https://github.com/anushthaPrakash">Anushtha Prakash</a> &emsp;
    <a href="https://github.com/ARinger22">Arpit Kumar Gautam</a> &emsp;
    <a href="https://github.com/AGlade7">Aryaman Gupta</a> &emsp;
    <a href="https://github.com/ashima-09">Ashima Goyal</a> &emsp;
  </strong>
</div>

</body>
</html>

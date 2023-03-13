================================================
Functional Simulator for subset of ARM Processor
================================================

README

Table of contents
1. Directory Structure
2. How to build
3. How to execute



Directory Structure:
--------------------
CS112-Project
  |
  |- bin
      |
      |- myARMSim
  |- doc
      |
      |- design-doc.docx
  |- include
      |
      |- myARMSim.h
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


  |- test
      |- simple_add.mem
      |- fib.mem
      |- array_add.mem

How to build
------------
For building:
	$cd src
	$make

For cleaning the project:
	$cd src
	$make clean


How to execute
--------------
./myARMSim test/simple_add.mem


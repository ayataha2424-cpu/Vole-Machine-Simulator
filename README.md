Vole Machine Simulator CS213 Assignment 2 Part 2

Overview

An object oriented C++ simulator and GUI application for the Vole Machine Architecture This simulator executes Vole machine instruction sets manages 16 Registers R0 RF 256 bytes of Memory 0x00 0xFF program counter PC instruction register IR and displays output written to the memory mapped screen output address 0xFF

Team Members and Task Distribution

Person 1 Ziad Ahmed Osman ID 20251166

Code CPU Core Fetch Decode Execute Cycle PC Management HALT C000 and Main GUI Controls Load Run Step Halt Reset

Report and Video Introduction Vole Machine Architecture and Vole Programs

Person 2 Habiba Essam Abo El Fotouh ID 20250187

Code ALU Core Add Integer Add Float Bitwise AND OR XOR Rotate Right and Memory Visualizer Grid

Report and Video Presentation Video Report Compilation and Formatting

Person 3 Nada Said Ahmed ID 20251453

Code Memory Management 256 cell Array File ProgramLoader Comment Stripping and Registers UI Display

Report and Video Initial Design Classes and UML Diagram

Person 4 Aya Taha Esmail ID 20251069

Code Core Registers and Instructions Modules Decode and Validate Terminal Menu displayMenu Screen 0xFF Output Widget and GUI Core Integration

Report and Video AI Generated Designs Comparison Final Design and README Documentation

Features and Architecture

1 Object Oriented Design C++ Clean separation of concerns with header h and implementation cpp files using Include Guards
2 Registers and Decoding Manages sixteen 8 bit registers and decodes 16 bit hex instructions
3 Memory and File Loader Supports loading programs from text files ignoring comments and mapping 0xFF address to output screen
4 Execution Control Supports full auto run mode and step by step debugging
5 Interactive GUI Bonus Built with Qt wxWidgets showcasing real time Memory Grid Registers panel and Output Screen
6 20 Test Programs Pre tested assembly programs including Password Checker Array Sum Floating point ops etc

How to Build and Run

Terminal Version
1 Open the solution in Visual Studio 2022
2 Build the project Ctrl Shift B
3 Run the executable F5
4 Follow the on screen menu displayMenu to load instructions and step through execution

GUI Version
1 Ensure Qt wxWidgets environment is configured
2 Build and launch the GUI application
3 Click Load to select a program file then use Run or Step to execute

Repository and GitHub Workflow

Main Branch main

Feature Branches feature registers and instructions feature alu feature memory feature gui

Version Control Rules Independent feature branches pushed before final system integration

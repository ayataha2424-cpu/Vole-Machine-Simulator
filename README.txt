==========================================================
 Vole Machine Simulator  (Terminal + Qt GUI)
 CS213 - Object Oriented Programming - Assignment 2
 Faculty of Computers and Artificial Intelligence, Cairo University
 Academic Year 26/27
 Supervised by: Dr Mohammad El-Ramly
==========================================================

TEAM
----
Team Leader: Aya Taha Esmail - Phone: 011 20642713

Name                          ID         Section   Email
Aya Taha Esmail (TL)          20251069   S17,18    20251069@stud.fci-cu.edu.eg
Habiba Essam Abo El Fotoh     20250187   S15,16    20250187@stud.fci-cu.edu.eg
Nada Said Ahmed               20251453   S17,18    20251453@stud.fci-cu.edu.eg
Ziad Ahmed Osman              20251166   S17,18    20251166@stud.fci-cu.edu.eg

PROJECT STRUCTURE
-----------------
Core (terminal version, written without AI):
  ALU.h/.cpp, CPU.h/.cpp, Instructions.h/.cpp, Memory.h/.cpp,
  Registers.h/.cpp, ProgramLoader.h/.cpp,
  A2-[Section]-[IDs].cpp (main file with the menu)

GUI (bonus, Qt 6 Widgets, written with AI assistance):
  gui/CMakeLists.txt, gui/main.cpp, gui/mainwindow.h, gui/mainwindow.cpp
  The GUI reuses the core files from the parent folder. It does not
  copy or modify them.

REQUIREMENTS
------------
- Windows 10/11
- Qt 6.12.0 (or any Qt 6.x) with the "MinGW 64-bit" kit
- Qt Creator and CMake (both come with the Qt installer)

HOW TO INSTALL QT
-----------------
1. Download the Qt Online Installer from https://www.qt.io
2. Log in with a free Qt account.
3. In "Select Components" choose:
   - Qt > Qt 6.x > MinGW 64-bit
   - Qt > Developer and Designer Tools > Qt Creator, CMake, MinGW
4. Finish the installation.

HOW TO BUILD AND RUN THE GUI
----------------------------
1. Open Qt Creator.
2. File > Open File or Project...
3. Select gui/CMakeLists.txt
4. Choose the kit "Desktop Qt 6.x.x MinGW 64-bit" and click
   "Configure Project".
5. Press Ctrl+B to build, then Ctrl+R to run.
(A black console window opens next to the GUI. This is normal; the
 program loader prints its error messages there.)

HOW TO BUILD AND RUN THE TERMINAL VERSION
-----------------------------------------
Open a terminal in the project folder (the one containing the .cpp files):
   g++ -std=c++17 *.cpp -o vole.exe
   .\vole.exe

HOW TO USE THE GUI
------------------
1. Load Program: choose a .txt file (one instruction of 4 hex digits
   per line; text after ';' is ignored) and enter an EVEN start
   address in hex (e.g. 00).
2. Step: executes one instruction.
3. Run: executes until HALT (C000), with a 10,000-step limit.
4. Reset: resets PC, IR, registers and screen.
5. Overwrite: edit any register or memory cell using 2 hex digits.
6. Screen box: shows everything written to address FF.
7. Changed cells/registers are highlighted, and the current PC cell
   is marked.
Programs with an invalid instruction are rejected and an error
message is shown.

SAMPLE PROGRAM
--------------
2005
2107
5201
32FF
C000
Expected screen output: 0C

NOTES
-----
- Address FF is the screen output.
- The program start address must be even.
- run() stops after 10,000 steps to avoid infinite loops (e.g. B000).
- The program loader rejects the whole file if any instruction is invalid.
- A program may fill memory up to the last cell (start address + size <= 256).

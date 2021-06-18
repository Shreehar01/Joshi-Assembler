#include "stdafx.h"
#include "Emulator.h"

/**/
/*
Emulator::InsertMemory()
NAME
	Emulator::InsertMemory - Stores the translated code in the
							 specified memory location.
SYNOPSIS
	bool Emulator::InsertMemory(int a_location, int a_contents);
	a_location -> The location where the translated code is to be stored.
	a_contents -> The translated code that is to be stored.
DESCRIPTION
	Stores the translated code in the specified memory location.
RETURNS
	Returns true if the a_contents was successfully stored in the location a_location.
	Returns false otherwise.
AUTHOR
	Shreehar Joshi
DATE
	2:14 PM 05/10/2021
*/
/**/
bool Emulator::InsertMemory(int a_location, int a_contents) {
	// If the translated code is to be stored at a location greater
	// than the memory size, return false.
	if (a_location >= MEMSZ) 
		return false;
	
	// Add the translated code to the specified location.
	m_memory[a_location] = a_contents;		
	return true;
}
/*bool Emulator::InsertMemory(int a_location, int a_contents);*/


/**/
/*
Emulator::RunProgram()
NAME
	Emulator::RunProgram - Runs the emulator by processing the translated code in the memory.
SYNOPSIS
	bool Emulator::RunProgram();
DESCRIPTION
	Goes through each memory location and executes the instruction found.
RETURNS
	Returns true if the code was succesfully emulated. 
	Returns false if error was encountered.
AUTHOR
	Shreehar Joshi
DATE
	2:20 PM 05/10/2021
*/
/**/
bool Emulator::RunProgram() {
	int instruction = 0;	// Translated code in specified location of memory.
	int opCode = 0;			// Operation code of the translation.
	int regNum = 0;			// Register number in the translation.
	int address = 0;		// Address in the translation.
	int location = 100;		// The location from which the execution of code begins.
	
	//Loop through each of the locations in the memory of VC5000.
	while (true) {
		// If there is no code, then skip the location. 
		if (m_memory[location] == 0) 
			continue;

		// If there is a code in the location, then execute it. 
		instruction = m_memory[location];
		opCode = instruction / 10000000;
		regNum = (instruction / 1000000) % 10;
		address = instruction % 100000;

		// Check if the register number is valid. Return false if its invalid.
		if (regNum < 0 || regNum > 9) {
			cout << "The register number is invalid." << endl;
			return false;
		}

		// Check the operation code and execute accordingly.
		switch (opCode) {
			case 0:
				// Assembler instruction in the memory in betweem machine lagnuage instruction. Skip it.
				continue;
			case 1:
				// Add the value in the given address to the register. 
				m_reg[regNum] += m_memory[address];
				break;

			case 2:
				// Substract the value in the given address from the value in the register.
				m_reg[regNum] -= m_memory[address];
				break;

			case 3:
				// Multiply the value in the given address with the value in the register.
				m_reg[regNum] *= m_memory[address];
				break;

			case 4:
				// Divide the value in the register with the value in the given address.
				m_reg[regNum] /= m_memory[address];
				break;

			case 5:
				// Load the value in the address into the register.
				m_reg[regNum] = m_memory[address];
				break;

			case 6:
				// Store the value in the register into the address.
				m_memory[address] = m_reg[regNum];
				break;

			case 7:
				// Ask input from the user and store it in the given address.
				cout << "?";
				int userInput;

				// Check if the input is an integer less than 1000000.
				if (cin >> userInput && userInput < MEMSZ) {
					m_memory[address] = userInput;
					break;
				}
				cout << "Input Error. Either the input is too large or it is of wrong type." << endl;
				return false;

			case 8:
				// Display the value present in the given address.
				cout << m_memory[address] << endl;
				break;

			case 9:
				// Change the location to the given address-1. Why address-1? Because the location is incremented by 1.
				location = address - 1;
				break;

			case 10:
				// If the value in the register is negative, proceed to address-1.
				if (m_reg[regNum] < 0)
					location = address - 1;
				break;

			case 11:
				// If the value in the register is 0, proceed to address-1.
				if (m_reg[regNum] == 0)
					location = address - 1;
				break;

			case 12:
				// If the value in the register is positive, proceed to address-1.
				if (m_reg[regNum] > 0)
					location = address - 1;
				break;

			case 13:
				// Terminate the emulation by returning true that indicates success.
				return true;

			default:
				// Terminate the emulation with an error.
				cout << "Unidentified Operation Code." << endl;
				return false;

		}

		location++;
	}
}
/*bool Emulator::RunProgram();*/
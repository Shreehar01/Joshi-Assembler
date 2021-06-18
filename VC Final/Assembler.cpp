//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"


/**/
/*
Assembler::Assembler()
NAME
    Assembler::Assembler - It is the constructor of the assembler class. 
SYNOPSIS
    Assembler::Assembler(int argc, char *argv[]);
        argc	->	The total number of command-line arguments. 
        argv	->	An array consisting of strings of program and file name. 
DESCRIPTION
    It is the constructor for the assembler class. 
    argc and argv are passed to the file access constructor.
RETURNS
    No return value as this is a constructor.
AUTHOR
    Shreehar Joshi
DATE
    5:20 PM 05/12/2021
*/
/**/
Assembler::Assembler(int argc, char* argv[]): m_facc(argc, argv) {};
/*Assembler::Assembler(int argc, char *argv[]);*/


// Destructor currently does nothing.  You might need to add something as you develope this project.
Assembler::~Assembler() {};


/**/
/*
Assembler::PassI()
NAME
    Assembler::PassI - Helps in filling the symbol table.
SYNOPSIS
    Assembler::PassI(); 
DESCRIPTION
    It establishes the location of the labels. 
RETURNS
    void.
AUTHOR
    Shreehar Joshi
DATE
    5:41 PM 05/12/2021
*/
/**/
void Assembler::PassI( ) {
    int loc = 0;        // Tracks the location of the instructions to be generated.
    // Successively process each line of source code.
    while(true) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {
            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.  Currently this is only comments.
        if( st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr ) continue;
	    
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.IsLabel( ) )
            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
/*Assembler::PassI();*/


/**/
/*
Assembler::PassII()
NAME
    Assembler::PassII - Translates the code and records errors.
SYNOPSIS
    Assembler::PassII();
DESCRIPTION
    Translates the code in the each line and stores them in the emulator. Also records and displayes errors. 
RETURNS
    void.
AUTHOR
    Shreehar Joshi
DATE
    6:01 PM 05/12/2021
*/
/**/
void Assembler::PassII() {
    // Do not proceed until the user hits enter.
    cout << "Press Enter to continue..." << endl;
    cin.ignore();

    m_facc.Rewind();    // Clean all file flags and go back to the beginning of the file.

    // Title and the Headers.
    cout << "Translation of Program:" << endl << endl;    // Title of the output
    cout << left << setw(10) << "LOCATION" << left << setw(15) << "CONTENTS" << left << setw(25) << "ORIGINAL STATEMENT" << endl;   // Headers of the output.

    int loc = 0;           // Tracks the location of the instructions to be generated.
    int errorCount;       // Records the number of errors found in a single line of translated code.
    string error;          // Records the error message.

    // Successively process each line of source code.
    while (true) {
        errorCount = 0;
        error = "";
        Errors::InitErrorReporting();            // A code has 3 parts (Op code, Register Number, and Address). Reseting all the error parts to false. 

        // Read the next line from the source file.
        string line;                          // Records the next line of instruction.
        if (!m_facc.GetNextLine(line)) {
            // If there are no more lines, we are missing an end statement.
            error = "No End statement found.";
            errorCount++;                 // Increment the value of the number of errors in the line. 
            Errors::RecordError(error);
            Errors::DisplayErrors(errorCount);
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // Pass II will determine if the end is the last statement.
        if (st == Instruction::ST_End) {
            if (m_facc.GetNextLine(line)) {
                error = "End statement not present or End is not the last statement.";
                errorCount++;
                Errors::RecordError(error);
                Errors::DisplayErrors(errorCount);
                return;
            } else {                        //Otherwise, end is the last statement. Break;
                cout << right << setw(30) << m_inst.GetInstruction() << endl;
                break;
            }
        }

        // If the operation code is invalid. Then record error in all parts of the code.
        if (st == Instruction::ST_Invalid) {
            error = "Invalid operation code.";
            errorCount++;
            Errors::RecordError(error);
            Errors::ErrorParts(1);
            Errors::ErrorParts(2);
            Errors::ErrorParts(3);
        }

        // Labels can only be on machine language, assembler language, or instruction with illegal operation code.
        // instructions.  So, skip other instruction types.  Currently this is only comments.
        if (st == Instruction::ST_Comment) {
            cout << setw(25) << " " << setw(30) << left << m_inst.GetInstruction() << endl;
            continue;
        }

        cout << left << setw(10) << loc;
        int value = 0;
        // If it is Assembler Instruction, then process and record the output value and errors from it.
        if (st == Instruction::ST_AssemblerInstr)
            if (ProcessAssemblerInstr(errorCount, value, loc)) continue;    // If true was returned, then continue with the loop.

        // If it is Machine Instruction, then process and record the output value and errors from it.
        if (st == Instruction::ST_MachineLanguage)
            ProcessMachineInstr(errorCount, value);

        // Convert the output value into string so that 9 digit translated code can be appropriately displayed.
        string output = to_string(value);
        int number_of_zeros = 9 - output.length();
        output = std::string(number_of_zeros, '0').append(output);    // Prepending the string with appropriate number of '0's.

        // Manipulate the output string. Replace the values with '?' in the parts where the errors were recorded.
        if (Errors::IsErrorParts(1))
            output.replace(0, 2, "??");
        if (Errors::IsErrorParts(2))
            output.replace(2, 1, "?");
        if (Errors::IsErrorParts(3))
            output.replace(3, 6, "??????");

        // Record the translated code to the memory only if no errors were recorded.
        if (Errors::IsEmpty())
            m_emul.InsertMemory(loc, value);
        
        cout << left << setw(15) << output << left << setw(25) << m_inst.GetInstruction() << endl; // Display the translation and the original instruction.

        // If the location of the next instruction is greater than size of the memory of VC 5000, then record the error, display it and return.
        if (m_inst.LocationNextInstruction(loc) > 999999) {
            error = "Insufficient Memory for Translation";
            Errors::RecordError(error);
            errorCount++;
            Errors::DisplayErrors(errorCount);
            return;
        }

        loc = m_inst.LocationNextInstruction(loc);   // Locate the next instruction.

        // If there are any errors found in the line, then display them. 
        if (errorCount)
            Errors::DisplayErrors(errorCount);
    }
    cout << endl << "-------------------------------------------------------" << endl << endl;
}
/*Assembler::PassII();*/


/**/
/*
Assembler::IsValidSymbol()
NAME
    Assembler::IsValidSymbol - Translates the assembler code and records errors.
SYNOPSIS
    void Assembler::IsValidSymbol(const string& a_symbol, int& a_errorCount);
        a_symbol     -> a symbol which is to be check for validity.
        a_errorCount -> record the number of errors encounterd in single line of code.
DESCRIPTION
    Checks if the symbol a_symbol is valid and record error and increments the value of a_errorCount
    if symbol is not valid.
RETURNS
    void.
AUTHOR
    Shreehar Joshi
DATE
    6:32 PM 05/12/2021
*/
/**/
void Assembler::IsValidSymbol(const string& a_symbol, int& a_errorCount) {
    string error = "";              // Records the error message.

    // Record error if the length of the symbol is not in between 1 and 20.
    if (a_symbol.length() < 1 || a_symbol.length() > 20) {
        error = "The length of symbol must be between 1 and 20.";
        a_errorCount++;
        Errors::RecordError(error);
    }

    // Record error if the first character of the symbol is not an alphabet.
    if (!isalpha(a_symbol[0])) {
        error = "The first character of the symbol must be letter.";
        a_errorCount++;
        Errors::RecordError(error);
    }

    // Record error if the characters (except the first characters) of the symbol are neither alphabet nor digit.  
    for (size_t i = 1; i < a_symbol.length(); i++) {
        if (!isalpha(a_symbol[i]) && !isdigit(a_symbol[i])) {
            error = "The symbols can only contain alphabet or digit.";
            a_errorCount++;
            break;   // If error found, then no need to check reamining characters.
        }
    }
}
/*void Assembler::IsValidSymbol(const string& a_symbol, int& a_errorCount);*/


/**/
/*
Assembler::ProcessMachineInstr()
NAME
    Assembler::ProcessMachineInstr - Process the machine language instruction and record errors.
SYNOPSIS
    void Assembler::ProcessMachineInstr(int& a_errorCount, int& a_value);
        a_errorCount ->  records the number of errors found in a single line of code.
        a_value       -> the translated code.
DESCRIPTION
    Processes the machine language instruction and generates the translated code a_value that is to
    be stored in the memory and displayed. It also records if any error is found and increments the value of a_errorCount.
RETURNS
    void.
AUTHOR
    Shreehar Joshi
DATE
    7:05 PM 05/12/2021
*/
/**/
void Assembler::ProcessMachineInstr(int& a_errorCount, int& a_value) {
    string error;             // Stores the error message.
    int temp_loc = 0;         // Stores the location of the label. If the value become -999, then we 
                              // can say that the label was already defined. 

    // First lets begin by checking the label.  
    if (m_inst.IsLabel()) {
        IsValidSymbol(m_inst.GetLabel(), a_errorCount);  // Check if the label is valid.
        // Record the error if the label was already defined.
        if (m_symtab.LookupSymbol(m_inst.GetLabel(), temp_loc) && temp_loc == -999) {
            error = "Multiply defined symbol.";
            a_errorCount++;
            Errors::RecordError(error);
        }
    }


    // Now, lets check the machine language instructions where register is required.
    if (m_inst.GetNumOpCode() != 7 && m_inst.GetNumOpCode() != 8 && m_inst.GetNumOpCode() != 10 && m_inst.GetNumOpCode() != 13) {
        // Record error if register is missing.
        if (!m_inst.IsNumericOperand1()) {
            error = "Missing register value.";
            a_errorCount++;
            Errors::RecordError(error);
            Errors::ErrorParts(2);
        }
        // Record error if the operand 2 is non-numeric.
        if (m_inst.IsNumericOperand2()) {
            error = "Error in operand 2 type.";
            a_errorCount++;
            Errors::RecordError(error);
            Errors::ErrorParts(2);
        }
        // Record error if operand 2 is missing.
        if (m_inst.IsNumericOperand1() && m_inst.GetOperand2().length() == 0) {
            error = "Missing operand 2.";
            a_errorCount++;
            Errors::RecordError(error);
            Errors::ErrorParts(3);
        }
        // Record error if there are extra operands.
        if (m_inst.IsEndString()) {
            error = "Extra operand found.";
            a_errorCount++;
            Errors::RecordError(error);
        }
        // Now lets check if the operand 2 is valid.
        if (!m_inst.IsNumericOperand2() && m_inst.GetOperand2().length() != 0)
            IsValidSymbol(m_inst.GetOperand2(), a_errorCount);

        a_value = (m_inst.GetNumOpCode() * 10 + m_inst.GetOperand1Value()) * 1000000;  // Process the value. 


        // If register value is given and operand 2 is also given but the operand 2 is not defined in the symbol table.
        if (m_inst.GetOperand2().length() != 0 && !m_symtab.LookupSymbol(m_inst.GetOperand2(), temp_loc)) {
            error = "The symbol is not defined.";
            a_errorCount++;
            Errors::RecordError(error);
            Errors::ErrorParts(3);
        } else if (m_inst.GetOperand2().length() == 0 && !m_inst.IsNumericOperand1() && !m_symtab.LookupSymbol(m_inst.GetOperand1(), temp_loc)) {   
            // If register value was missing and the operand 2 was provided as operand 1, record error 
            // if the operand 1(symbol) is not defined in the symbol table.
            error = "The symbol is not defined.";
            a_errorCount++;
            Errors::RecordError(error);
            Errors::ErrorParts(3);         
        } else {
            // Add the address to the translation value.
            a_value += temp_loc;
        }


    } else {             //  Now, lets process the instructions where register value is not required.

     // Lets process the "halt" instruction.
     // Record error if the there are extra operands.
        if (m_inst.GetNumOpCode() == 13) {
            if (m_inst.GetOperand1().length() != 0) {
                error = "Extra operand found.";
                a_errorCount++;
                Errors::RecordError(error);
            }
            a_value = (m_inst.GetNumOpCode() * 10 + 9) * 1000000;
        }

        // Lets process the "branch", "read", and "write" instruction.
        // There are lot of possible ways in which these instruction can be provided. 
        // Some, all or none of the operands(register value, symbol name, and extra operand) might be present. 
        // While we could have simply ignored the register value provided by the user, but we will include them in the translated code.
        if (m_inst.GetNumOpCode() != 13) {
            // Translate the code when the register value, and symbol name are present. Record error if the the symbol was not present in the symbol table.
            if (m_inst.IsNumericOperand1() && m_inst.GetOperand2().length() != 0) {
                a_value = (m_inst.GetNumOpCode() * 10 + m_inst.GetOperand1Value()) * 1000000;
                IsValidSymbol(m_inst.GetOperand2(), a_errorCount);
                if (!m_symtab.LookupSymbol(m_inst.GetOperand2(), temp_loc)) {
                    error = "The symbol is not defined.";
                    a_errorCount++;
                    Errors::RecordError(error);
                    Errors::ErrorParts(3);
                }
                // Record error if extra operand was provided.
                if (m_inst.IsEndString()) {
                    error = "Extra operand found.";
                    a_errorCount++;
                    Errors::RecordError(error);
                }
            }
            // Translate the code when the register value is present but symbol name is missing. Record error if symbol name is missing.
            if (m_inst.IsNumericOperand1() && m_inst.GetOperand2().length() == 0) {
                error = "Missing Operand.";
                a_errorCount++;
                Errors::RecordError(error);
                a_value = (m_inst.GetNumOpCode() * 10 + m_inst.GetOperand1Value()) * 1000000;
            }
            // Translate the code when register value is not present and only symbol name is present.
            // Record error if symbol name was not defined.
            if (!m_inst.IsNumericOperand1() && m_inst.GetOperand2().length() == 0) {
                a_value = (m_inst.GetNumOpCode() * 10 + 9) * 1000000;
                IsValidSymbol(m_inst.GetOperand1(), a_errorCount);
                if (!m_symtab.LookupSymbol(m_inst.GetOperand1(), temp_loc)) {
                    error = "The symbol is not defined.";
                    a_errorCount++;
                    Errors::RecordError(error);
                    Errors::ErrorParts(3);
                }
            }
            // Translate the code when register value is not present but Symbol Name and Extra Operand is present. 
            if (!m_inst.IsNumericOperand1() && m_inst.GetOperand2().length() != 0) {
                error = "Extra operand found.";
                a_errorCount++;
                Errors::RecordError(error);
                a_value = (m_inst.GetNumOpCode() * 10 + 9) * 1000000;
                IsValidSymbol(m_inst.GetOperand1(), a_errorCount);
                if (!m_symtab.LookupSymbol(m_inst.GetOperand1(), temp_loc)) {
                    error = "The symbol is not defined.";
                    a_errorCount++;
                    Errors::RecordError(error);
                    Errors::ErrorParts(3);
                }
            }
            // Translate the code when no operands are provided. Record error accordingly.
            if (m_inst.GetOperand1().length() == 0) {
                error = "Missing operand.";
                a_errorCount++;
                Errors::RecordError(error);
                Errors::ErrorParts(3);
                a_value = (m_inst.GetNumOpCode() * 10 + 9) * 1000000;
            }


        }
        a_value += temp_loc;
    }
}
/*void Assembler::ProcessMachineInstr(int& a_errorCount, int& a_value);*/


/**/
/*
Assembler::ProcessAssemblerInstr()
NAME
    Assembler::ProcessAssemblerInstr - Process the assembler instruction and record errors.
SYNOPSIS
    void Assembler::ProcessAssemblerInstr(int& a_errorCount, int& a_value, int& a_loc);
        a_errorCount  ->  records the number of errors found in a single line of code.
        a_value       -> the value of the translated code.
        a_loc         -> location of the current line of code. 
DESCRIPTION
    Processes the assembler instruction and generates the value of translation that is to
    be stored in the memory and displayed. It also records if any error is found and increments the value of a_errorCount.
    If the instruction is "org" or "ds" then the location is updated with the location of the next instruction.
RETURNS
    Returns true if we can "continue" with the loop. This happens if the instruction code is "ds" or "org"  as 
    we dont need to display the translation value.
    Returns false if we cannot "continue" with the loop (cannot skip other instructions in Pass 2).
AUTHOR
    Shreehar Joshi
DATE
    7:34 PM 05/12/2021
*/
/**/
bool Assembler::ProcessAssemblerInstr(int& a_errorCount, int& a_value, int& a_loc) {
    string error;             // Stores the error message.
    int temp_loc = 0;         // Stores the location of the label. If the value become -999, then we 
                              // can say that the label was already defined. 

    // Lets deal with the errors and translation value (if needed) first.
    // If the operation code was "org".
    if (m_inst.GetNumOpCode() == 3) {
        // If the operand is not numeric, then record error.
        if (!m_inst.IsNumericOperand1()) {
            error = "Operand must be numeric. Origin set to 100 by default.";
            a_errorCount++;
            Errors::RecordError(error);
        }
        // If there is additional operand, then record error. 
        if (m_inst.GetOperand2().length() != 0) {
            error = "Extra operand found.";
            a_errorCount++;
            Errors::RecordError(error);
        }
    } else {
        // If the operation code was not "org". This means either "dc" or "ds".
        // Record error if no label was provided. "dc" and "ds" must have a label.
        if (m_inst.GetLabel().length() == 0) {
            error = "Symbol name missing.";
            a_errorCount++;
            Errors::RecordError(error);
        }
        else {    // If the label exists.
            IsValidSymbol(m_inst.GetLabel(), a_errorCount);           // Check if the label is valid.   
            // Record error if the label was already defined.
            if (m_symtab.LookupSymbol(m_inst.GetLabel(), temp_loc) && temp_loc == -999) {
                error = "Multiply defined symbol.";
                a_errorCount++;
                Errors::RecordError(error);
            }
        }

        // Now moving onto the operand part of the instruction.
        // If the operand is not numeric, then record error.
        if (!m_inst.IsNumericOperand1()) {
            error = "Operand must be numeric.";
            a_errorCount++;
            Errors::RecordError(error);
            Errors::ErrorParts(3);
        }
        else {   // If the operand is numeric.
            a_value = m_inst.GetOperand1Value();
            // If the operation code is "dc" and constant is too large, then record error. 
            // No need to check the operand value for "ds" as if this is too large then it will 
            // eventually recorded as error once we get the next instruction towards the end of the loop.
            if (m_inst.GetNumOpCode() == 1 && a_value > 999999) {
                error = "The constant is too large.";
                a_errorCount++;
                a_value = 0;
                Errors::RecordError(error);
                Errors::ErrorParts(3);
            }
        }

        // Now, lets check if extra operands were provided.
        if (m_inst.GetOperand2().length() != 0) {
            error = "Extra operand found.";
            a_errorCount++;
            Errors::RecordError(error);
        }
    }
    
    // Now lets check if we can "continue" with the loop in Pass 2.
    // We can do so if the instruction code was either "ds" or "org".
    if (m_inst.GetNumOpCode() == 2 || m_inst.GetNumOpCode() == 3) {
        cout << left << setw(15) << "" << left << m_inst.GetInstruction() << endl;
        // If the next instruction is out of bound of the memory of VC 5000, display the errors and exit the program.
        if (m_inst.LocationNextInstruction(a_loc) > 999999) {
            error = "Insufficient Memory for Translation. Program has terminated.";
            Errors::RecordError(error);
            a_errorCount++;
            Errors::DisplayErrors(a_errorCount);
            exit(1);
        }
        a_loc = m_inst.LocationNextInstruction(a_loc);  // Store the next instruction.
        // If there are errors, then display them.
        if (a_errorCount)
            Errors::DisplayErrors(a_errorCount);
        // Send the message to continue through the loop without executing any further part of the code.
        return true;
    }
    
    // If we need to execute other codes in the Pass2 within the loop, return false as the message.
    // The other codes are the codes for displaying output(translated code and instruction) and errors, if any.
    return false;
}
/*void Assembler::ProcessAssemblerInstr(int& a_errorCount, int& a_value, int& a_loc);*/


/**/
/*
Assembler::RunProgramInEmulator()
NAME
    Assembler::RunProgramInEmulator - Runs the emulator.
SYNOPSIS
    void Assembler::RunProgramInEmulator();
DESCRIPTION
    Runs the emulator if there are no errors. 
RETURNS
    void.
AUTHOR
    Shreehar Joshi
DATE
    8:20 PM 05/12/2021
*/
/**/
void Assembler::RunProgramInEmulator() {
    cout << "Press Enter to continue..." << endl;
    cin.ignore();

    // Check if there are errors. Run the emulator only if there are no errors.
    if (!Errors::IsEmpty()) {
        cout << "Emulator can only proceed if there are no errors present." << endl;
    } else {
        cout << "Results from Emulating Program:" << endl << endl;
        if (!m_emul.RunProgram()) cout << "Error encountered. Emulator couldn't proceed." << endl;
    }

    cout << endl << "End of Emulation" << endl;
}
/*void Assembler::RunProgramInEmulator();*/
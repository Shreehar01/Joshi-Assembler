#include "stdafx.h"
#include "Instruction.h"

/**/
/*
Instruction::ResetValues()
NAME
    Instruction::ResetValues - Reinitializes the member variables.
SYNOPSIS
    void Instruction::ResetValues();
DESCRIPTION
    Reinitializes the member variables to default values.
Returns
    void
AUTHOR
    Shreehar Joshi
DATE
    03:47 PM 04/18/2021
*/
/**/
void Instruction::ResetValues() {
    m_Label = "";
    m_OpCode = "";
    m_Operand1 = "";
    m_Operand2 = "";
    m_instruction = "";
    m_IsNumericOperand1 = false;
    m_IsNumericOperand2 = false;
    m_Operand1Value = 0;
    m_type = ST_Invalid;
    m_endString = "";
}
/*void Instruction::ResetValues();*/


/**/
/*
Instruction::GetNumOpCode()
NAME
    Instruction::GetNumOpCode - Find the numeric operation code.
SYNOPSIS
    int Instruction::GetNumOpCode(const vector<string>& a_instructionSet, const string& a_value);
        a_instructionSet -> a vector that consists of the list of instructions (eg. add, sub, etc).
        a_value          -> a string whose numeric operation code is to be found.   
DESCRIPTION
    Finds the numeric operation code of a_value by looking at the instruction set a_instructionSet. 
Returns
    Returns the numeric operation code of the instruction provided. Returns -1 if the instruction
    doesnt exist in the set of instructions provided.
AUTHOR
    Shreehar Joshi
DATE
    03:47 PM 04/18/2021
*/
/**/
int Instruction::GetNumOpCode(const vector<string>& a_instructionSet, const string& a_value) {
    auto iterator = find(a_instructionSet.begin(), a_instructionSet.end(), a_value);

    // If the operation code value was not found in the instruction set.
    if (iterator == a_instructionSet.end())
        return -1;

    // Return the numeric code for the operation.
    int index = iterator - a_instructionSet.begin();
    return index;
}
/*int Instruction::GetNumOpCode(const vector<string>& a_instructionSet, const string& a_value);*/



/**/
/*
Instruction::IsNumeric()
NAME
    Instruction::IsNumeric - Checks if the operand is numeric.
SYNOPSIS
    bool Instruction::IsNumeric(const string& a_value)
DESCRIPTION
    Checks if the operand a_value is numeric.
Returns
    Returns true if the value provided is numeric. Returns false otherwise.
AUTHOR
    Shreehar Joshi
DATE
    4:21 PM 04/18/2021
*/
/**/
bool Instruction::IsNumeric(const string& a_value) {
    
    // If the value is empty, return false.
    if (a_value.length() == 0) 
        return false;
    
    // Loop through each characters of the value.
    for (char const& c : a_value) {
        // If even one character is not a digit, then return false.
        if (std::isdigit(c) == 0) 
            return false;
    }

    return true;
}
/*bool Instruction::IsNumeric(const string& a_value)*/


/**/
/*
Instruction::InstructionType Instruction::ParseInstruction()
NAME
    Instruction::InstructionType Instruction::ParseInstruction - Getting the type of instruction.
SYNOPSIS
    Instruction::InstructionType Instruction::ParseInstruction(string a_line);
        a_line -> a string that consists of a line in the file to be parsed.
DESCRIPTION
    The function extracts different components of instruction from the given line. It also
    determined the type of instruction.
Returns
    Returns the instruction type (comment, end, invalid, assembler, and machine instruction)
    in the line a_line.
AUTHOR
    Shreehar Joshi
DATE
    7:29 AM 05/26/2021
*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string a_line) {
    
    ResetValues(); // Initialize all the member variables.
    m_instruction = a_line;   // Make a copy of the original instruction.
    

    // Remove all the characters from the the first semicolon in the line.
    // Code copied from the website (Help on the VC5000 Assembler Project). 
    size_t isemi1 = a_line.find(';');
    if (isemi1 != string::npos)
        a_line.erase(isemi1);
    

    // Get rid of any commas from the line.
    // Code copied from the website (Help on the VC5000 Assembler Project).
    replace(a_line.begin(), a_line.end(), ',', ' ');


    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    // Code copied from the website (Help on the VC5000 Assembler Project).
    istringstream ins(a_line);
    if (a_line[0] == ' ' || a_line[0] == '\t') {
        m_Label = "";
        ins >> m_OpCode >> m_Operand1 >> m_Operand2 >> m_endString;
    } else {
        ins >> m_Label >> m_OpCode >> m_Operand1 >> m_Operand2 >> m_endString;
    }


    // Convert all the op codes to lowercase.
    transform(m_OpCode.begin(), m_OpCode.end(), m_OpCode.begin(), ::tolower);
    

    // Determining the type of the instruction.
    if (a_line.length() == 0 || all_of(a_line.begin(), a_line.end(), isspace)) {
        
        // If the line is empty or has just the blank spaces, then the instruction type is comment.
        m_type = ST_Comment;

    } else if (m_OpCode == "end") {
        
        // If the opcode has "end" as the value, then the instruction is end.
        m_type = ST_End;

    } else if (GetNumOpCode(m_assemblerInstruction, m_OpCode) != -1) {
        
        // If the opcode string was found in the assembler instruction, then the instruction type is assembler instruction.
        m_type = ST_AssemblerInstr;
        m_NumOpCode = GetNumOpCode(m_assemblerInstruction, m_OpCode) + 1;

    } else if (GetNumOpCode(m_machineInstruction, m_OpCode) != -1) {
        
        // If the opcode string was found in the machine instruction, then the instruction type is machine instruction.
        m_type = ST_MachineLanguage;
        m_NumOpCode = GetNumOpCode(m_machineInstruction, m_OpCode) + 1;

    }
    

    // Determine if operand 1 and 2 are numeric.
    if (IsNumeric(m_Operand1)) {
        m_IsNumericOperand1 = true;
        m_Operand1Value =  stoi(m_Operand1);   // Store the numeric value of the operand 1.
    }
    if (IsNumeric(m_Operand2)) 
        m_IsNumericOperand2 = true;

    return m_type;
}
/*Instruction::InstructionType Instruction::ParseInstruction(string a_line);*/


/**/
/*
Instruction::LocationNextInstruction(int a_loc)
NAME
    Instruction::LocationNextInstruction - Find the location of the next instruction.
SYNOPSIS
    int Instruction::LocationNextInstruction(int a_loc);
        a_loc -> the location of the current instruction.
DESCRIPTION
    It finds the location of the next instruction.
Returns
    It returns the location of the next instruction.
AUTHOR
    Shreehar Joshi
DATE
    11:23 AM 05/26/2021
*/
/**/
int Instruction::LocationNextInstruction(int a_loc) {
    
    // If the opcode is "org", then check if there is an integer provided. If it is provided,
    // then the next instruction will be set to the integer. Else, set it to 100 by default.
    if (m_OpCode == "org") {
        if (IsNumeric(m_Operand1))
            return a_loc + stoi(m_Operand1);
        else
            return 100;
    }

    // If the opcode is "ds", then check if there is an integer provided. If it is provided,
    // then the location of next instruction will be determined by 
    // incrementing the current location with the value of the integer. Else, return the location of next instruction by
    // incrementing the current location by 1.
    if(m_OpCode == "ds") {
        if (IsNumeric(m_Operand1))
            return a_loc + stoi(m_Operand1);
        else
            return a_loc + 1;
    }

    return a_loc + 1;
}
/*int Instruction::LocationNextInstruction(int a_loc);*/


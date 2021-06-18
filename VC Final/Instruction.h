//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

// The elements of an instruction.
class Instruction {

public:

    // Constructor of the Instruction class. Initializes all the member variables.
    Instruction() { ResetValues(); }
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?
    enum InstructionType {
        ST_MachineLanguage, 	   // A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End,                   		// End instruction.
        ST_Invalid                  // Instruction with Illegal Operation Code     
    };

    // Parse the Instruction.
    InstructionType ParseInstruction(string a_line);

    // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc);

    // To access the original instruction.
    inline string& GetInstruction() {
        return m_instruction;
    }

    // To access the label.
    inline string& GetLabel( ) {
        return m_Label;
    };

    // To access the operation code.
    inline int& GetNumOpCode() {
        return m_NumOpCode;
    }

    // To access the operand 1. 
    inline string& GetOperand1() { 
        return m_Operand1;
    }

    // To access the operand 2.
    inline string& GetOperand2() { 
        return m_Operand2; 
    }

    // To determine if a label is blank.
    inline bool IsLabel( ) {
        return ! m_Label.empty();
    };

    // To determine if there is an end string.
    bool IsEndString() {
        return ! m_endString.empty();
    }

    // To determine if the operand 1 is numeric.
    inline bool IsNumericOperand1() { 
        return m_IsNumericOperand1; 
    }

    // To determine if the operand 2 is numeric.
    inline bool IsNumericOperand2() { 
        return m_IsNumericOperand2;
    }

    // To access the numeric value of operand 1 if it is numeric.  
    inline int& GetOperand1Value() {
        return m_Operand1Value;
    }
 
private:


    // The elements of a instruction
    string m_Label;            // The label.
    string m_OpCode;           // The symbolic op code.
    string m_Operand1;         // The first operand. 
    string m_Operand2;         // The second operand.
    string m_endString;        // The extra string.

    string m_instruction;      // The original instruction.

    // Derived values.
    int m_NumOpCode;           // The numerical value of the op code for machine language equivalents.
    InstructionType m_type;    // The type of instruction.

    bool m_IsNumericOperand1;  // == true if the operand 1 is numeric.
    bool m_IsNumericOperand2;  // == true if the operand 2 is present and it is numeric.
    int m_Operand1Value;       // The value of the operand 1 if it is numeric.

    // Containers
    vector<string> m_assemblerInstruction = { "dc", "ds", "org", "end"};
    vector<string> m_machineInstruction = { "add", "sub", "mult", "div", "load", "store", "read", "write", "b", "bm", "bz", "bp", "halt" };

    // Functions
    // Reinitialize all the member variables.
    void ResetValues();

    // To determine if the given operation code belongs assembler or machine language instruction.
    // Returns the value of operation code of the instruction.
    int GetNumOpCode(const vector<string>& a_instructionSet, const string& a_value);

    // To determine if the value of the operand is numeric.
    bool IsNumeric(const string& a_value);
    
};
#endif


//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H


#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII( );

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab; // Symbol table object
    Instruction m_inst;	    // Instruction object
    Emulator m_emul;        // Emulator object

    // Checks if the symbol is valid and record errors accordingly.
    void IsValidSymbol(const string& a_symbol, int& a_errorCount);

    // Process the assembler instruction. Record translated values and errors.
    bool ProcessAssemblerInstr(int& error_count, int& value, int& loc);

    // Process the machine language instruction. Record translated values and errors.
    void ProcessMachineInstr(int& error_count, int& value);
};
#endif

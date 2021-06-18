//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"


/**/
/*
SymbolTable::AddSymbol()
NAME
    SymbolTable::AddSymbol - adds a new symbol to the symbol table.
SYNOPSIS
    void SymbolTable::AddSymbol(const string& a_symbol, int a_loc);
        a_symbol -> The name of the symbol to be added.
        a_loc    ->  The location where the symbol is to be added.
DESCRIPTION
    This function will place the symbol a_symbol in the location a_loc
    in the symbol table.
RETURNS
    void
AUTHOR
    Shreehar Joshi
DATE
    7:21 PM 05/04/2021
*/
/**/
void SymbolTable::AddSymbol(const string& a_symbol, int a_loc) {
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (st != m_symbolTable.end()) {
        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record the location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*void SymbolTable::AddSymbol(const string& a_symbol, int a_loc);*/


/**/
/*
SymbolTable::DisplaySymbolTable()
NAME
    SymbolTable::DisplaySymbolTable - Displays the symbol table on the screen.
SYNOPSIS
    void SymbolTable::DisplaySymbolTable();
DESCRIPTION
    Displays the symbol table on the screen. 
RETURNS
    void
AUTHOR
    Shreehar Joshi
DATE
    7:29 PM 05/04/2021
*/
/**/
void SymbolTable::DisplaySymbolTable() {
    int symbolNo = 0;  // The number of the symbol in the symbol table.
    cout << "Output from test program:" << endl << endl;
    cout << "Symbol Table: " << endl << endl;
    cout << setw(10) << left << "Symbol#" << setw(10) << left << "Symbol" << setw(10) << left << "Location" << endl;
    
    // Display the symbols and their location.
    map<string, int>::iterator it;
    for (it = m_symbolTable.begin(); it != m_symbolTable.end(); it++) 
        cout << setw(10) << left << symbolNo++ << setw(10) << left << it->first << setw(10) << left << it->second << endl;
    
    cout << endl;
    cout << "-------------------------------------------------------" << endl << endl;
}
/*void SymbolTable::DisplaySymbolTable();*/


/**/
/*
SymbolTable::LookupSymbol()
NAME
    SymbolTable::LookupSymbol - Checks if the symbol exists in the symbol table.
SYNOPSIS
    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);
        a_symbol  ->  The name of the symbol to be looked for.
        a_loc     ->  The location that will be changed if the symbol is found.
DESCRIPTION
    Checks if the symbol a_symbol exists in the symbol table. If the symbol exists
    the value of a_loc is changed to the location of the symbol.
RETURNS
    Returns true if the symbol is found. Returns false otherwise.
AUTHOR
    Shreehar Joshi
DATE
    7:41 PM 05/04/2021
*/
/**/
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {

    // If the symbol exists in the symbol table, then return true.
    if (m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
        a_loc = m_symbolTable[a_symbol];
        return true;
    }

    // If the symbol doesnt exist in the symbol table, then return false.
    return false;
}
/*bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);*/

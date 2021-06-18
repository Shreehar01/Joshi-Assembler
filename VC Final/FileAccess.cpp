//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"


/**/
/*
FileAccess::FileAccess()
NAME
    FileAccess::FileAccess - Helps in opening a file from which the code is to be read.
SYNOPSIS
    FileAccess::FileAccess(int argc, char *argv[]);
    argc	->	The total number of command-line arguments.
    argv	->	An array consisting of names of program and file to be opened.
DESCRIPTION
    This function is a constructor for FileAccess class which is used to 
    open the file to read the code. It exits with error if the number of command-line
    argument is not appropriate if the file to be read from doesnt exist.
Returns
    No return value because this is a constructor.
AUTHOR
    Shreehar Joshi
DATE
    9:01 AM 04/16/2021
*/
/**/
FileAccess::FileAccess( int argc, char *argv[] ) {
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
/*FileAccess::FileAccess(int argc, char *argv[]);*/


/**/
/*
FileAccess::GetNextLine()
NAME
    FileAccess::GetNextLine - Get the next line from the file.
SYNOPSIS
    bool FileAccess::GetNextLine(string &a_line);
    a_line -> Consists of the set of characters in the next line.
DESCRIPTION
    If the next lines exists in the file, this function reads the line 
    and stores it in a_line. 
Returns
    Returns true if next line exists and returns false otherwise.
AUTHOR
    Shreehar Joshi
DATE
    9:14 AM 04/16/2021
*/
/**/
bool FileAccess::GetNextLine(string &a_line ) {
    // If there is no more data, return false.
    if( m_sfile.eof()) 
        return false;
    
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}
/*bool FileAccess::GetNextLine(string &a_line);*/


/**/
/*
FileAccess::Rewind()
NAME
    FileAccess::Rewind - Goes back to the beginning of file.
SYNOPSIS
    void FileAccess::Rewind();
DESCRIPTION
    Cleans all the file flafs and goes back to the beginning
    of the file.
Returns
    void
AUTHOR
    Shreehar Joshi
DATE
    9:21 AM 04/16/2021
*/
/**/
void FileAccess::Rewind( ) {
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
/*void FileAccess::Rewind();*/

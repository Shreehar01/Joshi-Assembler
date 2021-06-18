//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include "stdafx.h"

class Errors {

public:
    
    // Initializes error reports.
    static void InitErrorReporting();

    // Records an error message.
    static void RecordError(const string& a_emsg );

    // Displays the collected error message in the most recent translated line.
    static void DisplayErrors(const int& a_count);

    // Records which part (operation code, register number, or address) of the translated code has error.
    inline static void ErrorParts(const int& a_part) {
        m_errorPart[a_part - 1] = true;
    }

    // Checks if specific part of the code consists error.
    inline static bool IsErrorParts(const int& a_part) {
        return m_errorPart[a_part - 1];
    }

    // Checks if there are errors or not.
    static bool IsEmpty();

private:

    static vector<string> m_ErrorMsgs;  // This must be declared in the .cpp file.  Why?
    static bool m_errorPart[3];         // Records true if a part of the translated code contains error.
};
#endif
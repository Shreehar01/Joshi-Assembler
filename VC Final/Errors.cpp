#include "stdafx.h"
#include "Errors.h"


vector<string> Errors::m_ErrorMsgs = {};  // Consists of all the error messages.
bool Errors::m_errorPart[3] = { false, false, false };    // Records the part of the translated code that consists of error. true if the part has error. 
														  // false otherwise.

/**/
/*
Errors::InitErrorReporting()
NAME
	Errors::InitErrorReporting - Resets all the error parts to false.
SYNOPSIS
	void Errors::InitErrorReporting();
DESCRIPTION
	A code consist of three parts - operation code, register number, and address.
	This function resets all the variables that are used for indicating if there is error in any part of
	the code to false.
Returns
	void
AUTHOR
	Shreehar Joshi
DATE
	5:37 PM 05/05/2021
*/
/**/
void Errors::InitErrorReporting() {
	m_errorPart[0] = false;
	m_errorPart[1] = false;
	m_errorPart[2] = false;
}
/*Errors::InitErrorReporting();*/


/**/
/*
Errors::RecordError()
NAME
	Errors::RecordError - Records the error message.
SYNOPSIS
	Errors::RecordError(const string& a_eMsg);
	a_eMsg -> The string that contains the error message.
DESCRIPTION
	Stores the error message a_eMsg to the end of the vector
	consisting of the list of error messages.
Returns
	void
AUTHOR
	Shreehar Joshi
DATE
	5:48 PM 05/05/2021
*/
/**/
void Errors::RecordError(const string& a_eMsg) {
	m_ErrorMsgs.push_back(a_eMsg);
}
/*Errors::RecordError(const string& a_eMsg);*/


/**/
/*
Errors::DisplayErrors()
NAME
	Errors::DisplayErrors - Prints the lists of errors in the most recent line of code.
SYNOPSIS
	void Errors::DisplayErrors(const int& a_count);
	a_count -> The number which indicates the last n number of errors to be displayed.
DESCRIPTION
	Prints out all the last a_count number of error messages stored in the vector that consists of errors.
Returns
	void
AUTHOR
	Shreehar Joshi
DATE
	6:13 PM 05/05/2021
*/
/**/
void Errors::DisplayErrors(const int& a_count) {
	
	// Display the last a_count errors in the vector (errors in the recently translated line of code).  
	for (int i = m_ErrorMsgs.size() - a_count; i < m_ErrorMsgs.size(); i++ ) 
		cout << m_ErrorMsgs[i] << " ";
	
	cout << endl;
}
/*void Errors::DisplayErrors(const int& a_count);*/


/**/
/*
Errors::IsEmpty()
NAME
	Errors::IsEmpty - Checks if there are any errors recorded.
SYNOPSIS
	bool Errors::IsEmpty();
DESCRIPTION
	Checks if the list of errors is empty or not.
Returns
	Returns true if the vector containing errors messages is empty.
	Returns false otherwise.
AUTHOR
	Shreehar Joshi
DATE
	6:21 PM 05/05/2021
*/
/**/
bool Errors::IsEmpty() {
	
	// Return true if there are no errors.
	if (m_ErrorMsgs.empty())
		return true; 
	
	// Return false if there are errors.
	return false;
}
/*bool Errors::IsEmpty();*/




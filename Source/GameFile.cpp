#include <windows.h>
#include "..\Header\Gamelib.h"



/*********************************************************


CONSTRUCTION / DESTRUCTION


***********************************************************/




GAME_FILE::GAME_FILE()
	{
	m_hFile			= NULL;
	m_FileIsOpen	= FALSE;
	m_Mode			= 0;
	}



GAME_FILE::~GAME_FILE()
	{
	Close();
	}




/****************************************************************


OPEN / CLOSE ROUTINES


****************************************************************/



BOOL GAME_FILE::OpenForReading(const char * strfile)
	{

	/////////////////////////////////////
	//								   //
	// Used to open a file for reading //
	//								   //
	/////////////////////////////////////


	// If a file is already open then close it

	if (m_FileIsOpen)
		Close();


	// Open the file

	m_hFile = CreateFile(strfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)	
		{
		OutputDebugString("ERROR :: OPENING GAME FILE\r\n");

		m_hFile = NULL;
	
		return FALSE;
		}


	m_FileIsOpen = TRUE;
	m_Mode = GAME_FILE::MODE_READ;

	return TRUE;
	}




BOOL GAME_FILE::OpenForWriting(const char * strfile)
	{

	/////////////////////////////////////
	//								   //
	// Used to open a file for writing //
	//								   //
	/////////////////////////////////////


	// If a file is already open then close it

	if (m_FileIsOpen)
		Close();


	// Open the file

	m_hFile = CreateFile(strfile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)	
		{
		OutputDebugString("ERROR :: OPENING GAME FILE\r\n");

		m_hFile = NULL;
	
		return FALSE;
		}


	m_FileIsOpen = TRUE;
	m_Mode = GAME_FILE::MODE_WRITE;

	return TRUE;
	}




void GAME_FILE::Close()
	{

	////////////////////////////
	//						  //
	// Used to close the file //
	//						  //
	////////////////////////////


	if (m_FileIsOpen)
		{
		CloseHandle(m_hFile);

		m_hFile = NULL;
		m_FileIsOpen = FALSE;
		m_Mode = 0;
		}
	}





BOOL GAME_FILE::WriteRaw(void * buffer, int size)
	{

	///////////////////////////////////////////
	//										 //
	// Used to write a raw value to the file //
	//										 //
	///////////////////////////////////////////


	DWORD numwritten;


	// Prompt error if in read mode

	if (m_Mode == GAME_FILE::MODE_READ)
		{
		OutputDebugString("ERROR :: WRITING TO A READ ONLY FILE\r\n");
		return FALSE;
		}


	// Write to the file 

	return WriteFile(m_hFile, buffer, size, &numwritten, NULL);
	}




BOOL GAME_FILE::ReadRaw(void * buffer, int size, DWORD * numread)
	{

	////////////////////////////////////////////
	//										  //
	// Used to read a raw value from the file //
	//										  //
	////////////////////////////////////////////


	DWORD tempnumread;


	// Prompt error if in write mode

	if (m_Mode == GAME_FILE::MODE_WRITE)
		{
		OutputDebugString("ERROR :: READING FROM A WRITE ONLY FILE\r\n");
		return FALSE;
		}


	// Read from the file

	return ReadFile(m_hFile, buffer, (DWORD)size, (numread != NULL) ? numread : &tempnumread, NULL);
	}




BOOL GAME_FILE::WriteLine(STRING * line)
	{

	//////////////////////////////////////////////////////////////
	//															//
	// Used to write a text line to the file.  The line ends in //
	// carriage return and line read							//
	//															//
	//////////////////////////////////////////////////////////////
	

	DWORD numwritten;
	STRING linewrite = line;

	
	// Prompt error if in read mode

	if (m_Mode == GAME_FILE::MODE_READ)
		{
		OutputDebugString("ERROR :: WRITING TO A READ ONLY FILE\r\n");
		return FALSE;
		}


	linewrite += "\r\n";

	return WriteFile(m_hFile, (char *)linewrite, strlen(linewrite), &numwritten, NULL);
	}




STRING GAME_FILE::ReadLine(BOOL skipblanks, BOOL skipcomments)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to read a text line to the file.  The line ends in //
	// carriage return and line read						   //
	//														   //
	/////////////////////////////////////////////////////////////


	DWORD startpos, currpos, endpos, filesize, numread;
	char lastcharread, charread, firstchar = ' ';
	STRING retstring;



	// Prompt error if in write mode

	if (m_Mode == GAME_FILE::MODE_WRITE)
		{
		OutputDebugString("ERROR :: READING FROM A WRITE ONLY FILE\r\n");
		return retstring;
		}


	// Store the current file position

	startpos = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	filesize = GetFileSize(m_hFile, NULL);


	lastcharread = '\0';


	// While the done flag is not set

	currpos = startpos;
	endpos = currpos;


	while(1)
		{

		// If at the end of the file, set the done flag

		if (currpos >= filesize)
			{
			break;
			}


		// Read the next character from the file

		ReadFile(m_hFile, &charread, 1, &numread, NULL);


		// If this character is not \r or \n then set the endpos to this position

		if (charread != 13 && charread != 10)
			endpos = currpos;


		// If the character read is alpha/numberic and the first char so far is a space
		// then set the first char to the char read

		if (charread >= 31 && charread <= 254 && firstchar == ' ')
			firstchar = charread;


		// If the last character read is \r and this character is \n then ...

		if (lastcharread == 13 && charread == 10)
			{
			// If skip blanks is set and the first char is blank (and all others) or 
			// skip comments is set and the first char is '/' (comment character) then
			// just set a new start position and continue

			if ((skipblanks == TRUE && firstchar == ' ') || (skipcomments == TRUE && firstchar == '/'))
				{
				currpos++;
				startpos = currpos;
				endpos = startpos;
				firstchar = ' ';

				continue;
				}


			// Otherwise, just break from the loop

			break;
			}

		lastcharread = charread;
		currpos++;
		}



	// If the endpos is greater than the startpos

	if (currpos > startpos)
		{
		char * temp;


		// Allocate memory for the read string

		temp = (char*) new char[endpos - startpos + 2];


		// Read the string into the buffer

		SetFilePointer(m_hFile, startpos, NULL, FILE_BEGIN);
		ReadFile(m_hFile, temp, endpos - startpos + 1, &numread, NULL);
				

		// Shift the file pointer up two spaces

		if (SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT) + 2 < filesize)
			SetFilePointer(m_hFile, 2, NULL, FILE_CURRENT);


		// Set the return string
	
		temp[endpos - startpos + 1] = '\0';
		retstring = temp;


		// Deallocate the temporary buffer

		delete[] temp;
		}


	// Return the string

	return retstring;
	}




BOOL GAME_FILE::WriteParams(STRING_ARRAY params)
	{

	//////////////////////////////////////////////////////////////////
	//																//
	// Used to write parameters of a string array to a text file by //
	// separating each string with a comma							//
	//																//
	//////////////////////////////////////////////////////////////////
	

	DWORD numwritten;
	char comma = ',';
	char endline[] = { 13, 10 };


	// Prompt error if in read mode

	if (m_Mode == GAME_FILE::MODE_READ)
		{
		OutputDebugString("ERROR :: WRITING TO A READ ONLY FILE\r\n");
		return FALSE;
		}


	// Write each string from the array

	for (int i = 0; i < params.Count(); i++)
		{
		WriteFile(m_hFile, (char *)params[i], strlen(params[i]), &numwritten, NULL);		

		if (i < params.Count() - 1)
			WriteFile(m_hFile, &comma, 1, &numwritten, NULL);

		WriteFile(m_hFile, endline, 2, &numwritten, NULL);
		}


	return TRUE;
	}
		



STRING_ARRAY GAME_FILE::ReadParams(BOOL trimarray)
	{

	///////////////////////////////////////////////////////////////////////
	//																	 //
	// Used to read parameters of a line by reading a line, splitting it //
	// at the commas, trimming the individual strings, and returning     //
	// them in a string array											 //
	//																	 //
	///////////////////////////////////////////////////////////////////////


	STRING_ARRAY splitarray;
	STRING_ARRAY retarray;
	STRING line;


	// Prompt error if in write mode

	if (m_Mode == GAME_FILE::MODE_WRITE)
		{
		OutputDebugString("ERROR :: READING FROM A WRITE ONLY FILE\r\n");
		return retarray;
		}


	// Read the line

	line = ReadLine(TRUE);
	

	// Split the line at the commas

	splitarray = line.Split(',');

	
	// If trimarray is false, then return the untrimmed array

	if (!trimarray)
		return splitarray;


	// For each string in the split array, trim it and add it to the return array

	for (int i = 0; i < splitarray.Count(); i++)
		{
		retarray.Add(splitarray[i].Trim());
		}


	return retarray;
	}
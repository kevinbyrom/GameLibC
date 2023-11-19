#include <windows.h>
#include <stdio.h>
#include "..\Header\Gamelib.h"




/**************************************************


CONSTRUCTION / DESTRUCTION


**************************************************/



STRING_ARRAY::STRING_ARRAY()
	{
	Strings		= NULL;
	StrCount	= 0;
	ArraySize	= 0;
	}




STRING_ARRAY::STRING_ARRAY(const STRING_ARRAY & s)
	{
	Strings		= NULL;
	StrCount	= 0;
	ArraySize	= 0;

	if (s.StrCount > 0 && AllocateArray(s.StrCount))
		{
		for (int i = 0; i < s.StrCount; i++)
			{
			Strings[i] = new STRING(s.Strings[i]);

			StrCount = s.StrCount;
			}
		}
	}




STRING_ARRAY::~STRING_ARRAY()
	{
	DeallocateStrings();
	DeallocateArray();
	}




/*********************************************************


ARRAY ROUTINES


**********************************************************/



BOOL STRING_ARRAY::AllocateArray(int size)
	{

	////////////////////////////////////////////////////////
	//													  //
	// Used to allocate the array to fit a specified size //
	// if it does not already hold enough room			  //
	//													  //
	////////////////////////////////////////////////////////


	STRING ** temp = NULL;
	int tempsize;
	int templength;
	BOOL firstalloc = TRUE;


	// Return true if the array size already fits

	if (ArraySize >= size)
		return TRUE;


	// Create a backup of the array

	if (ArraySize > 0)
		{
		temp = new STRING*[ArraySize];
		tempsize = ArraySize;
		templength = StrCount;
		memcpy(temp, Strings, sizeof(STRING *) * ArraySize);
		}


	try
		{
		// Delete the array if already allocated;

		if (Strings != NULL)
			{
			DeallocateArray();
			firstalloc = FALSE;
			}

		
		// If this is the first time allocating, just allocate enough, otherwise
		// create a new array which is 150% the size needed to prevent future reallocations

		if (firstalloc)
			ArraySize = size;
		else
			ArraySize = size + int(size / 2);

		Strings = new STRING*[ArraySize];
	
		
		// Set all the string pointers to null

		for (int i = 0; i < ArraySize; i++)
			Strings[i] = NULL;


		// Copy the original into the new array

		if (temp != NULL)
			{
			memcpy(Strings, temp, sizeof(STRING *) * tempsize);
			StrCount = templength;
			}
		}
	catch(...)
		{

		// Return false if an exception occured

		Strings		= NULL;
		ArraySize	= 0;
		StrCount	= 0;

		return FALSE;
		}


	// Remove the temporary array

	if (temp != NULL)
		delete[] temp;


	#ifdef _DEBUG
	//OutputDebugString("ALLOCATED STRING ARRAY\r\n");
	#endif


	return TRUE;
	}




void STRING_ARRAY::DeallocateArray()
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to deallocate the memory used for the array //
	//													//
	//////////////////////////////////////////////////////


	if (Strings != NULL)
		delete[] Strings;

	Strings		= NULL;
	ArraySize	= 0;
	StrCount	= 0;

	#ifdef _DEBUG
	//OutputDebugString("DEALLOCATED STRING_ARRAY\r\n");
	#endif
	}




void STRING_ARRAY::DeallocateStrings()
	{

	//////////////////////////////////////////////
	//											//
	// Used to deallocate all allocated strings //
	//											//
	//////////////////////////////////////////////


	for (int i = 0; i < StrCount; i++)
		{
		if (Strings[i] != NULL)
			delete Strings[i];

		Strings[i] = NULL;
		}
	}




void STRING_ARRAY::DeallocateString(int index)
	{

	////////////////////////////////////////////
	//										  //
	// Used to deallocate an allocated string //
	//										  //
	////////////////////////////////////////////

	delete Strings[index];
	Strings[index] = NULL;
	}





void STRING_ARRAY::Add(STRING & s)
	{

	//////////////////////////////////////////////////
	//												//
	// Used to add a new string to the string array //
	//												//
	//////////////////////////////////////////////////


	AddStringToArray(&s);
	}




void STRING_ARRAY::Add(STRING * s)
	{

	//////////////////////////////////////////////////
	//												//
	// Used to add a new string to the string array //
	//												//
	//////////////////////////////////////////////////


	AddStringToArray(s);
	}




void STRING_ARRAY::Add(const char * s)
	{

	//////////////////////////////////////////////////
	//												//
	// Used to add a new string to the string array //
	//												//
	//////////////////////////////////////////////////


	STRING temp(s);


	AddStringToArray(&temp);
	}




void STRING_ARRAY::AddStringToArray(STRING * pstr)
	{

	//////////////////////////////////////////////////////////////////////
	//																	//
	// Used to allocate enough room in the string array and add the new //
	// string to the end of it											//
	//																	//
	//////////////////////////////////////////////////////////////////////


	if (AllocateArray(StrCount + 1))
		{
		Strings[StrCount] = new STRING(pstr);
		StrCount += 1;
		}
	}




void STRING_ARRAY::RemoveAt(int index)
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to remove a string at the specified position //
	//													 //
	///////////////////////////////////////////////////////


	STRING ** temp = NULL;


	// If index is out of range for the array, just return

	if (index < 0 || index >= StrCount)
		return;


	// If the string count is 1, then just deallocate the array and return

	if (StrCount == 1)
		{
		DeallocateStrings();
		DeallocateArray();
		return;
		}


	// Deallocate the string

	DeallocateString(index);


	// First, store the array in a temp array

	temp = new STRING*[StrCount];
	memcpy(temp, Strings, sizeof(STRING *) * StrCount);


	// Next, if the new string count will be less then half the array size
	// then just deallocate and reallocate the array to a smaller size

	if (StrCount - 1 < (ArraySize / 2))
		{
		DeallocateArray();
		AllocateArray(StrCount - 1);
		}


	// Copy the strings which are before the removed item

	if (index != 0)
		memcpy(Strings, temp, index);

	
	// Copy the strings which are after the removed item

	if (index != StrCount - 1)
		memcpy(Strings[index], temp[index + 1], StrCount - index - 1);


	// Set the new string count
 
	StrCount -= 1;
	}




/*************************************************************


ASSIGNMENT ROUTINES


*************************************************************/




STRING_ARRAY & STRING_ARRAY::operator = (const STRING_ARRAY & s)
	{
	DeallocateStrings();
	StrCount	= 0;

	if (s.StrCount > 0 && AllocateArray(s.StrCount))
		{
		for (int i = 0; i < s.StrCount; i++)
			{
			Strings[i] = new STRING(s.Strings[i]);

			StrCount = s.StrCount;
			}
		}	

	return *this;
	}




/*************************************************************


INDEXER ROUTINES


**************************************************************/



STRING STRING_ARRAY::operator [] (const int index) const
	{

	// If the index is out of range, just return an empty string

	if (index < 0 || index >= StrCount)
		{
		STRING temp;
		return temp;
		}


	return *Strings[index];
	}





/**************************************************


CONSTRUCTION / DESTRUCTION


***************************************************/




STRING::STRING()
	{
	Buffer		= NULL;
	BufferSize	= 0;
	StrLength	= 0;
	}



STRING::STRING(const STRING * s)
	{
	Buffer		= NULL;
	BufferSize	= 0;
	StrLength	= 0;

	if (s->StrLength == 0)
		return;

	if (AllocateBuffer(s->StrLength + 1))
		{
		SetBufferString(s->Buffer);
		}
	}



STRING::STRING(const STRING & s)
	{
	Buffer		= NULL;
	BufferSize	= 0;
	StrLength	= 0;

	if (s.StrLength == 0)
		return;

	if (AllocateBuffer(s.StrLength + 1))
		{
		SetBufferString(s.Buffer);
		}
	}



STRING::STRING(const char * s)
	{
	Buffer		= NULL;
	BufferSize	= 0;
	StrLength	= 0;

	if (s == NULL)
		return;

	if (AllocateBuffer(strlen(s) + 1))
		{
		SetBufferString(s);
		}
	}




STRING::~STRING()
	{
	if (Buffer != NULL)
		DeallocateBuffer();
	}



/***************************************************


BUFFER ROUTINES


*****************************************************/



BOOL STRING::AllocateBuffer(int size)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to allocate the buffer to fit a specified size //
	// if it does not already hold enough room			   //
	//													   //
	/////////////////////////////////////////////////////////


	char * temp = NULL;
	int tempsize;
	int templength;
	BOOL firstalloc = TRUE;


	// Return true if the buffer size already fits

	if (BufferSize >= size)
		return TRUE;


	// Create a backup of the buffer

	if (BufferSize > 0)
		{
		temp = new char[BufferSize];
		tempsize = BufferSize;
		templength = StrLength;
		memcpy(temp, Buffer, sizeof(char) * BufferSize);
		}


	try
		{
		// Delete the buffer if already allocated;

		if (Buffer != NULL)
			{
			DeallocateBuffer();
			firstalloc = FALSE;
			}

		
		// If this is the first time allocating, just allocate enough, otherwise
		// create a new buffer which is 150% the size needed to prevent future reallocations

		if (firstalloc)
			BufferSize = size;
		else
			BufferSize = size + int(size / 2);

		Buffer = new char[BufferSize];
	
		
		// Copy the original into the new buffer

		if (temp != NULL)
			{
			memcpy(Buffer, temp, sizeof(char) * tempsize);
			StrLength = templength;
			}
		}
	catch(...)
		{

		// Return false if an exception occured

		Buffer = NULL;
		BufferSize = 0;
		StrLength = 0;

		return FALSE;
		}


	// Remove the temporary buffer

	if (temp != NULL)
		delete[] temp;


	#ifdef _DEBUG
	//OutputDebugString("ALLOCATED BUFFER\r\n");
	#endif


	return TRUE;
	}




void STRING::DeallocateBuffer()
	{

	///////////////////////////////////////////////////////
	//													 //
	// Used to deallocate the memory used for the buffer //
	//													 //
	///////////////////////////////////////////////////////


	if (Buffer != NULL)
		delete[] Buffer;

	Buffer		= NULL;
	BufferSize	= 0;
	StrLength	= 0;

	#ifdef _DEBUG
	//OutputDebugString("DEALLOCATED BUFFER\r\n");
	#endif
	}




void STRING::SetBufferString(const char * str)
	{

	///////////////////////////////////////////////////////////////////
	//																 //
	// Used to set the buffer characters to that of the given string //
	//																 //
	///////////////////////////////////////////////////////////////////


	// If the string is invalid, just return 

	if (str == NULL)
		return;


	// Copy the characters from the string to the buffer

	StrLength = strlen(str);

	memcpy(Buffer, str, StrLength);
	Buffer[StrLength] = '\0';
	}




void STRING::SetBufferString(const char c)
	{

	/////////////////////////////////////////////////////////////////////
	//																   //
	// Used to set the buffer character to that of the given character //
	//																   //
	/////////////////////////////////////////////////////////////////////


	// Copy the character to the buffer

	StrLength = 1;

	Buffer[0] = c;
	Buffer[1] = '\0';
	}




void STRING::ConcatBufferString(const char * str)
	{

	/////////////////////////////////////////////////////////////////////////
	//																	   //
	// Used to concatenate a string to the end of the string in the buffer //
	//																	   //
	/////////////////////////////////////////////////////////////////////////


	// Add the string to the end of the buffer string

	memcpy(&Buffer[StrLength], str, sizeof(char) * strlen(str));
	
	StrLength = StrLength + strlen(str);

	Buffer[StrLength] = '\0';
	}




void STRING::ConcatBufferString(const char c)
	{

	////////////////////////////////////////////////////////////////////////////
	//																		  //
	// Used to concatenate a character to the end of the string in the buffer //
	//																	      //
	////////////////////////////////////////////////////////////////////////////


	// Add character string to the end of the buffer string

	Buffer[StrLength] = c;
	
	StrLength = StrLength + 1;

	Buffer[StrLength] = '\0';
	}




/**************************************


SELECTION ROUTINES


***************************************/




STRING_ARRAY STRING::Split(char delimeter)
	{

	///////////////////////////////////////////////////////////////////////////
	//																		 //
	// Used to split a string whereever the character specified as delimiter //
	// is found																 //
	//																		 //
	///////////////////////////////////////////////////////////////////////////


	STRING_ARRAY temp;
	int startpos = 0;


	// If the string is empty, return an empty array

	if (StrLength == 0)
		return temp;


	// Search each character until delimeter is found and add that portion 
	// to the string array

	for (int i = 0; i < StrLength; i++)
		{
		if (Buffer[i] == delimeter)
			{
			if (startpos != i)
				temp.Add(Mid(startpos, i - startpos));

			startpos = i + 1;
			}
		}


	temp.Add(Mid(startpos, StrLength - startpos));

	return temp;
	}




STRING STRING::Mid(int start, int count)
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to return a string which contains a portion //
	// of this string									//
	//													//
	//////////////////////////////////////////////////////


	STRING temp;
	

	// If the start and count are within the limits...

	if (StrLength >= start + count)
		{

		// Allocate room for the string

		temp.AllocateBuffer(count + 1);


		// Copy the characters

		memcpy(temp.Buffer, &Buffer[start], sizeof(char) * count);


		// Set the string length and finalize the string

		temp.StrLength = count;
		temp.Buffer[count] = '\0';
		}


	return temp;
	}





/*******************************************


CONVERSION ROUTINES


********************************************/





STRING STRING::ToUpper()
	{
	STRING temp(*this);

	for (int i = 0; i < StrLength; i++)
		temp.Buffer[i] = (char)toupper(temp.Buffer[i]);

	return temp;
	}




STRING STRING::ToLower()
	{
	STRING temp(*this);

	for (int i = 0; i < StrLength; i++)
		temp.Buffer[i] = (char)tolower(temp.Buffer[i]);

	return temp;
	}





/**************************************


PAD ROUTINES


***************************************/





STRING STRING::PadLeft(char padchar, int num)
	{
	STRING temp;
	char * pad = NULL;

	if (num > 0)
		{
		
		// Create a character array the size of num

		pad = new char[num + 1];


		// Fill the array with the character set in padchar

		for (int i = 0; i < num; i++)
			pad[i] = padchar;


		// Finalize the string

		pad[num] = '\0';


		temp = pad;
		temp += *this;

		delete[] pad;
		}


	return temp;
	}




STRING STRING::PadRight(char padchar, int num)
	{
	STRING temp;
	char * pad = NULL;


	if (num > 0)
		{
		
		// Create a character array the size of num

		pad = new char[num + 1];


		// Fill the array with the character set in padchar

		for (int i = 0; i < num; i++)
			pad[i] = padchar;


		// Finalize the string

		pad[num] = '\0';


		temp = *this;
		temp += pad;

		delete[] pad;
		}


	return temp;
	}





/**************************************


TRIM ROUTINES


***************************************/




STRING STRING::TrimLeft()
	{

	/////////////////////////////////////////////////////////////////
	//															   //
	// Used to return a string with all whitespaces on the left of //
	// this string removed										   //
	//															   //
	/////////////////////////////////////////////////////////////////


	STRING temp;


	// Starting from the first character, check each character until 
	// a non whitespace character is found

	for (int i = 0; i < StrLength; i++)
		{
		if (Buffer[i] != ' ')
			{
			temp = Mid(i, StrLength - i);
			break;
			}
		}

	return temp;
	}




STRING STRING::TrimRight()
	{

	//////////////////////////////////////////////////////////////////
	//															    //
	// Used to return a string with all whitespaces on the right of //
	// this string removed										    //
	//															    //
	//////////////////////////////////////////////////////////////////


	STRING temp;


	// Starting from the last character, check each character until 
	// a non whitespace character is found

	for (int i = StrLength - 1; i >= 0; i--)
		{
		if (Buffer[i] != ' ')
			{
			temp = Mid(0, i + 1);
			break;
			}
		}

	return temp;
	}




STRING STRING::Trim()
	{

	///////////////////////////////////////////////////////////////////
	//															     //
	// Used to return a string with all whitespaces on the right and //
	// left of this string removed									 //
	//															     //
	///////////////////////////////////////////////////////////////////


	STRING temp;

	temp = TrimLeft();
	temp = temp.TrimRight();

	return temp;
	}





/*************************************


ASSIGNMENT OPERATORS


***************************************/





STRING & STRING::operator = (const STRING * x)
	{
	if (AllocateBuffer(x->StrLength + 1))
		{
		SetBufferString(x->Buffer);
		}

	return *this;
	}




STRING & STRING::operator = (const STRING & x)
	{
	if (AllocateBuffer(x.StrLength + 1))
		{
		SetBufferString(x.Buffer);
		}

	return *this;
	}




STRING & STRING::operator = (char x)
	{
	if (AllocateBuffer(2))
		{
		SetBufferString(x);
		}

	return *this;
	}




STRING & STRING::operator = (const char * x)
	{
	if (AllocateBuffer(strlen(x) + 1))
		{
		SetBufferString(x);
		}

	return *this;
	}




STRING & STRING::operator = (int x)
	{
	char temp[256];

	sprintf(temp, "%d", x);

	if (AllocateBuffer(strlen(temp) + 1))
		{
		SetBufferString(temp);
		}

	return *this;
	}




STRING & STRING::operator = (unsigned int x)
	{
	char temp[256];

	sprintf(temp, "%u", x);

	if (AllocateBuffer(strlen(temp) + 1))
		{
		SetBufferString(temp);
		}

	return *this;
	}




STRING & STRING::operator = (float x)
	{
	char temp[256];

	sprintf(temp, "%.2f", x);

	if (AllocateBuffer(strlen(temp) + 1))
		{
		SetBufferString(temp);
		}

	return *this;
	}





/*********************************************


Comparison operators


**********************************************/




BOOL STRING::operator == (const STRING * x)
	{
	return !strncmp(Buffer, x->Buffer, x->StrLength);
	}




BOOL STRING::operator != (const STRING * x)
	{
	return strncmp(Buffer, x->Buffer, x->StrLength);
	}




BOOL STRING::operator == (const STRING & x)
	{
	return !strncmp(Buffer, x.Buffer, x.StrLength);
	}




BOOL STRING::operator != (const STRING & x)
	{
	return strncmp(Buffer, x.Buffer, x.StrLength);
	}




BOOL STRING::operator == (const char * x)
	{
	return !strncmp(Buffer, x, strlen(x));
	}



BOOL STRING::operator != (const char * x)
	{
	return strncmp(Buffer, x, strlen(x));
	}




/**********************************************


Concatenate operators


***********************************************/




STRING & STRING::operator += (const STRING * x)
	{
	if (AllocateBuffer(StrLength + x->StrLength + 1))
		{
		ConcatBufferString(x->Buffer);
		}

	return *this;
	}




STRING & STRING::operator += (const STRING & x)
	{
	if (AllocateBuffer(StrLength + x.StrLength + 1))
		{
		ConcatBufferString(x.Buffer);
		}

	return *this;
	}




STRING & STRING::operator += (const char x)
	{
	if (AllocateBuffer(StrLength + 2))
		{
		ConcatBufferString(x);
		}

	return *this;
	}




STRING & STRING::operator += (const char * x)
	{
	if (AllocateBuffer(StrLength + strlen(x) + 1))
		{
		ConcatBufferString(x);
		}

	return *this;
	}




STRING & STRING::operator += (int x)
	{
	char temp[256];

	sprintf(temp, "%d", x);

	if (AllocateBuffer(StrLength + strlen(temp) + 1))
		{
		ConcatBufferString(temp);
		}

	return *this;
	}




STRING & STRING::operator += (unsigned int x)
	{
	char temp[256];

	sprintf(temp, "%u", x);

	if (AllocateBuffer(StrLength + strlen(temp) + 1))
		{
		ConcatBufferString(temp);
		}

	return *this;
	}




STRING & STRING::operator += (float x)
	{
	char temp[256];

	sprintf(temp, "%.02f", x);

	if (AllocateBuffer(StrLength + strlen(temp) + 1))
		{
		ConcatBufferString(temp);
		}

	return *this;
	}





STRING STRING::operator + (const STRING * x) const
	{
	STRING temp(*this);

	temp += x;

	return temp;
	}




STRING STRING::operator + (const STRING & x) const
	{
	STRING temp(*this);

	temp += x;

	return temp;
	}




STRING STRING::operator + (const char x) const
	{
	STRING temp(*this);

	temp += x;

	return &temp;
	}




STRING STRING::operator + (const char * x) const
	{
	STRING temp(*this);

	temp += x;

	return temp;
	}




STRING STRING::operator + (int x) const	
	{
	STRING temp(*this);

	temp += x;

	return temp;
	}




STRING STRING::operator + (unsigned int x) const
	{
	STRING temp(*this);

	temp += x;

	return temp;
	}




STRING STRING::operator + (float x) const
	{
	STRING temp(*this);

	temp += x;

	return temp;
	}




/************************************************************


  INDEXER OPERATORS


**************************************************************/



char STRING::operator [] (const int x) const
	{
	if (x < 0 || x >= StrLength)
		return '\0';

	return Buffer[x];
	}





/***********************************************************


TYPECAST CONVERSIONS


*************************************************************/




STRING::operator char *()
	{
	return Buffer;
	}



STRING::operator const char *()
	{
	return Buffer;
	}
	



STRING::operator int()
	{
	if (StrLength)
		return atoi(Buffer);
	else
		return 0;
	}




STRING::operator float()
	{
	if (StrLength)
		return (float)atof(Buffer);
	else
		return 0.0f;
	}
#include <windows.h>
#include "..\Header\Gamelib.h"



int INPUT::NumJoysticks;
STRING INPUT::JoyName[MAX_JOYSTICKS];
GUID INPUT::JoyGUID[MAX_JOYSTICKS];


 

INPUT::INPUT()
	{
	Initialized	= FALSE;

	DInput		= NULL;
	DIKeyboard	= NULL;
	DIMouse		= NULL;

	KeyboardInstalled	= FALSE;
	MouseInstalled		= FALSE;

	for (int i = 0; i < MAX_JOYSTICKS; i++)
		{
		DIJoystick[i]			= NULL;
		JoystickInstalled[i]	= FALSE;
		}
	}




INPUT::~INPUT()
	{
	DeInit();
	}




int INPUT::Init(HINSTANCE instance, HWND hwnd)
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to initialize the direct input manager //
	//											   //
	/////////////////////////////////////////////////


	if (FAILED(DirectInputCreate(instance, DIRECTINPUT_VERSION, &DInput, NULL)))
		return FALSE;

	HWnd = hwnd;
	
	Initialized = TRUE;

	return TRUE;
	}




void INPUT::DeInit()
	{

	///////////////////////////////////////////////////
	//												 //
	// Used to deinitialize the direct input manager //
	//												 //
	///////////////////////////////////////////////////


	if (DIKeyboard != NULL)
		{
		DIKeyboard->Unacquire();
		DIKeyboard->Release();
		DIKeyboard = NULL;
		}


	if (DIMouse != NULL)
		{
		DIMouse->Unacquire();
		DIMouse->Release();
		DIMouse = NULL;
		}


	for (int i = 0; i < MAX_JOYSTICKS; i++)
		{
		if (DIJoystick[i] != NULL)
			{
			DIJoystick[i]->Unacquire();
			DIJoystick[i]->Release();
			DIJoystick[i] = NULL;
			}
		}


	if (DInput != NULL)
		{
		DInput->Release();
		DInput = NULL;
		}
	}


int INPUT::InstallKeyboard()
	{

	//////////////////////////////////
	//								//
	// Used to install the keyboard //
	//								//
	//////////////////////////////////


	// Return false if not initialized or has been installed

	if (!Initialized || KeyboardInstalled)
		return FALSE;


	// Create the keyboard device 

	if (FAILED(DInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL)))
		return FALSE;

	OutputDebugString("KEYBOARD CREATED\r\n");


	// Set the cooperation level 

	if (FAILED(DIKeyboard->SetCooperativeLevel(HWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return FALSE;

	OutputDebugString("KEYBOARD COOP LEVEL SET\r\n");


	// Set the keyboard data format //

	if (FAILED(DIKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return FALSE;

	OutputDebugString("KEYBOARD DATA FORMAT SET\r\n");


	// Acquire the keyboard //

	if (FAILED(DIKeyboard->Acquire()))
		return FALSE;


	KeyboardInstalled = TRUE;


	OutputDebugString("KEYBOARD INSTALLED\r\n");


	return TRUE;
	}





int INPUT::InstallMouse()
	{

	///////////////////////////////
	//							 //
	// Used to install the mouse //
	//							 //
	///////////////////////////////


	// Return false if not initialized or has been installed

	if (!Initialized || MouseInstalled)
		return FALSE;


	// Create the mouse device 

	if (FAILED(DInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL)))
		return FALSE;


	// Set the cooperation level 

	if (FAILED(DIMouse->SetCooperativeLevel(HWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return FALSE;


	// Set the mouse data format 

	if (FAILED(DIMouse->SetDataFormat(&c_dfDIMouse)))
		return FALSE;


	// Acquire the mouse 

	if (FAILED(DIMouse->Acquire()))
		return FALSE;


	MouseInstalled = TRUE;


	return TRUE;
	}




int INPUT::InstallJoysticks()
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to install the joysticks by setting up the //
	// joystick structures							   //
	//												   //
	/////////////////////////////////////////////////////


	EnumJoysticks();

	memset(JoystickInstalled, 0, sizeof(BOOL) * MAX_JOYSTICKS);

	for (int i = 0; i < NumJoysticks; i++)
		{
		LPDIRECTINPUTDEVICE lpdi;

		if (FAILED(DInput->CreateDevice(JoyGUID[i], &lpdi, NULL)))
			return FALSE;

		if (FAILED(lpdi->QueryInterface(IID_IDirectInputDevice2, (void**)&DIJoystick[i])))
			return FALSE;

		if (FAILED(DIJoystick[i]->SetDataFormat(&c_dfDIJoystick)))
			return FALSE;

		SetDefaultJoystickProperties(i);

		if (FAILED(DIJoystick[i]->Acquire()))
			return FALSE;
		

		JoystickInstalled[i] = TRUE;
		}

	return TRUE;
	}



void INPUT::Poll()
	{

	//////////////////////////////////////
	//									//
	// Used to update the input devices //
	//									//
	//////////////////////////////////////


	// Save the last input states

	memcpy(LastKeyboard, Keyboard, sizeof(unsigned char) * 256);
	memcpy(&LastMouse, &Mouse, sizeof(MOUSE));
	memcpy(LastJoystick, Joystick, sizeof(JOYSTICK) *  MAX_JOYSTICKS);


	// Get the keyboard state //

	if (KeyboardInstalled && DIKeyboard->GetDeviceState(sizeof(char) * 256, (LPVOID)Keyboard) == DIERR_INPUTLOST)
		{
		}



	// Get the mouse state //

	if (MouseInstalled && DIMouse->GetDeviceState(sizeof(MOUSE), (LPVOID)&Mouse) == DIERR_INPUTLOST)
		{
		}



	// Get the joystick state //

	for (int i = 0; i < MAX_JOYSTICKS; i++)
		{
		if (JoystickInstalled[i])
			{
			DIJoystick[i]->Poll();

			if (DIJoystick[i]->GetDeviceState(sizeof(JOYSTICK), (LPVOID)&Joystick[i]) == DIERR_INPUTLOST)
				{
				}
			}
		}
	}




void INPUT::EnumJoysticks()
	{

	////////////////////////////////////////////////////////
	//													  //
	// Used to enumerate all the available joystick GUIDs //
	//													  //
	////////////////////////////////////////////////////////


	INPUT::NumJoysticks = 0;


	DInput->EnumDevices(DIDEVTYPE_JOYSTICK, INPUT::EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	}




BOOL CALLBACK INPUT::EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID guid_ptr)
	{

	/////////////////////////////////////////////////////////////////
	//															   //
	// Callback function used to store the joystick guids returned //
	// from enum devices										   //
	//															   //
	/////////////////////////////////////////////////////////////////


	INPUT::JoyName[INPUT::NumJoysticks] = lpddi->tszProductName;
	INPUT::JoyGUID[INPUT::NumJoysticks] = lpddi->guidInstance;
	INPUT::NumJoysticks++;

	return (DIENUM_CONTINUE);
	}



void INPUT::SetDefaultJoystickProperties(int joynum)
	{

	/////////////////////////////////////////////////////////////
	//														   //
	// Used to set the default joystick properties (x/y range) //
	//														   //
	/////////////////////////////////////////////////////////////


	DIPROPRANGE range;

	// Set the x offset

	range.lMin = -100;
	range.lMax = 100;
	
	range.diph.dwSize = sizeof(DIPROPRANGE);
	range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	range.diph.dwObj = DIJOFS_X;
	range.diph.dwHow = DIPH_BYOFFSET;

	DIJoystick[joynum]->SetProperty(DIPROP_RANGE, &range.diph);


	// Set the y offset

	range.lMin = -100;
	range.lMax = 100;
	
	range.diph.dwSize = sizeof(DIPROPRANGE);
	range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	range.diph.dwObj = DIJOFS_Y;
	range.diph.dwHow = DIPH_BYOFFSET;

	DIJoystick[joynum]->SetProperty(DIPROP_RANGE, &range.diph);
	}
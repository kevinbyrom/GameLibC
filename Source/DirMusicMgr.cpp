#include <windows.h>
#include "..\Header\Gamelib.h"



DIRECT_MUSIC_MGR DMusicMgr;



DIRECT_MUSIC_MGR::DIRECT_MUSIC_MGR()
	{
	DMPerf		= NULL;
	DMLoader	= NULL;
	Initialized	= FALSE;
	}




DIRECT_MUSIC_MGR::~DIRECT_MUSIC_MGR()
	{
	DeInit();
	}




int DIRECT_MUSIC_MGR::Init(HWND hwnd, LPDIRECTSOUND dsound)
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to initialize the direct music manager //
	//											   //
	/////////////////////////////////////////////////



	// Attempt to initalize COM 

	if (FAILED(CoInitialize(NULL)))
		return FALSE;


	// Attempt to create the DirectMusicPerformance object

	if (FAILED(CoCreateInstance(CLSID_DirectMusicPerformance, 
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicPerformance,
								(void**)&DMPerf)))
		{
		return FALSE;
		}


	// Attempt to initialize it

	if (FAILED(DMPerf->Init(NULL, dsound, hwnd)))
		return FALSE;


	// Attempt to create the DirectMusicLoader object

	if (FAILED(CoCreateInstance(CLSID_DirectMusicLoader, 
								NULL,
								CLSCTX_INPROC,
								IID_IDirectMusicLoader,
								(void**)&DMLoader)))
		{
		return FALSE;
		}


	Initialized = TRUE;

	return TRUE;
	}




void DIRECT_MUSIC_MGR::DeInit()
	{

	///////////////////////////////////////////////////
	//												 //
	// Used to deinitialize the direct music manager //
	//												 //
	///////////////////////////////////////////////////


	if (DMLoader != NULL)
		{
		DMLoader->Release();
		DMLoader = NULL;
		}


	if (DMPerf != NULL)
		{
		DMPerf->Release();
		DMPerf = NULL;
		}
	}




int DIRECT_MUSIC_MGR::LoadMidi(char * filename, MIDI_CLIP * clip)
	{

	//////////////////////////////
	//							//
	// Used to load a midi file //
	//							//
	//////////////////////////////


	/*DMUS_OBJECTSEC objdesc;
	HRESULT hr;
	IDirectMusicSegment * segment = NULL;
	int index;
	char szDir[512];	
	char wszDir[512];
	

	// Get the working path */

	return TRUE;
	}
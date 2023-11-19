#include <windows.h>
#include "..\Header\Gamelib.h"




SCREEN::SCREEN()
	{
	Initialized	= FALSE;

	DDraw		= NULL;
	Primary		= NULL;
	Back		= NULL;
	Palette		= NULL;
	Clipper		= NULL;
	}





SCREEN::~SCREEN()
	{
	DeInit();
	}





int SCREEN::Init(HWND hwnd, int width, int height, int bpp, int fullscreen)
	{

	///////////////////////////////////
	//								 //
	// Used to initialize the screen //
	//								 //
	///////////////////////////////////


	LPDIRECTDRAW lptempdd;
	PALETTEENTRY GrayPal[256];


	HWnd		= hwnd;
	Width		= width;
	Height		= height;
	BPP			= bpp;
	FullScreen	= fullscreen;


	// Create the direct draw object //

	if (FAILED(DirectDrawCreate(NULL, &lptempdd, NULL)))
		return FALSE;


	// Get the latest interface //

	if (FAILED(lptempdd->QueryInterface(IID_IDirectDraw4, (LPVOID *)&DDraw)))
		{
		lptempdd->Release();
		return FALSE;
		}


	// Set the cooperation level //

	lptempdd->Release();

	if (FullScreen)
		{
		if (FAILED(DDraw->SetCooperativeLevel(HWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE)))
			return FALSE;
		}
	else
		{
		if (FAILED(DDraw->SetCooperativeLevel(HWnd, DDSCL_NORMAL)))
			return FALSE;
		}


	// Set the display mode //

	if (FullScreen)
		if (FAILED(DDraw->SetDisplayMode(Width, Height, BPP, 0, 0)))
			return FALSE;



	// Now create the primary surface //

	memset(&SurfaceDesc, 0, sizeof(SurfaceDesc));
	SurfaceDesc.dwSize				= sizeof(SurfaceDesc);

	if (FullScreen)
		{
		SurfaceDesc.dwFlags				= DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		SurfaceDesc.dwBackBufferCount	= 1;
		SurfaceDesc.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
		}
	else
		{
		SurfaceDesc.dwFlags				= DDSD_CAPS;
		SurfaceDesc.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE;
		}


	if (FAILED(DDraw->CreateSurface(&SurfaceDesc, &Primary, NULL)))
		return FALSE;



	// Set the pixel format for the screen

	DDSURFACEDESC2 pddsd;

	memset(&pddsd, 0, sizeof(pddsd));
	pddsd.dwSize = sizeof(pddsd);

	Primary->GetSurfaceDesc(&pddsd);

	PixelFormat.Create(&pddsd.ddpfPixelFormat);



	// Create the main palette//

	if (PixelFormat.GetBitCount() < 16)
		{
		for (int pal = 0; pal < 256; pal++)
			{
			GrayPal[pal].peRed		= pal;
			GrayPal[pal].peGreen	= pal;
			GrayPal[pal].peBlue		= pal;
			GrayPal[pal].peFlags	= PC_NOCOLLAPSE;
			}


		// Create the palette 

		if (FAILED(DDraw->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE, GrayPal, &Palette, NULL)))
			return FALSE;
		

		// Set the palette to the primary surface //

		if (FAILED(Primary->SetPalette(Palette)))
			return FALSE;
		}


	// Create the back surface //

	if (FullScreen)
		{
		SurfaceDesc.ddsCaps.dwCaps	= DDSCAPS_BACKBUFFER;

		if (FAILED(Primary->GetAttachedSurface(&SurfaceDesc.ddsCaps, &Back)))
			return FALSE;
		}
	else
		{
		SurfaceDesc.dwSize			= sizeof(SurfaceDesc);
		SurfaceDesc.dwFlags			= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		SurfaceDesc.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN;
		SurfaceDesc.dwHeight		= height;
		SurfaceDesc.dwWidth			= width;

		if (FAILED(DDraw->CreateSurface(&SurfaceDesc, &Back, NULL)))
			return FALSE;
		}


	BackSurf.Attach(this, Back, width, height);


	// Create a clipper //

	if (!FullScreen)
		{
		/*if (FAILED(DDraw->CreateClipper(0, &Clipper, NULL)))
			return FALSE;

		if (FAILED(Clipper->SetHWnd(0, HWnd)))
			return FALSE;

		if (FAILED(Primary->SetClipper(Clipper)))
			return FALSE;*/
		}


	Initialized = TRUE;


	return TRUE;
	}





void SCREEN::DeInit()
	{

	//////////////////////////////////////
	//									//
	// Used to de-initialize the screen //
	//									//
	//////////////////////////////////////


	if (DDraw && FullScreen)
		DDraw->RestoreDisplayMode();
		
	if (Primary)
		Primary->Release();

	if (DDraw)
		DDraw->Release();

	Primary	= NULL;
	DDraw	= NULL;
	}





int SCREEN::TotalVidMem()
	{

	//////////////////////////////////////////////////
	//									            //
	// Used to get the total amount of video memory //
	//									            //
	//////////////////////////////////////////////////


	DDCAPS ddcaps;


	// Init the caps struct 

	memset(&ddcaps, 0, sizeof(ddcaps));
	ddcaps.dwSize = sizeof(ddcaps);


	// Get the caps 

	DDraw->GetCaps(&ddcaps, NULL);


	// Return the total vid mem

	return ddcaps.dwVidMemTotal;
	}





int SCREEN::AvailableVidMem()
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to get the amount of available video memory //
	//													//
	//////////////////////////////////////////////////////


	DDCAPS ddcaps;


	// Init the caps struct 

	memset(&ddcaps, 0, sizeof(ddcaps));
	ddcaps.dwSize = sizeof(ddcaps);


	// Get the caps 

	DDraw->GetCaps(&ddcaps, NULL);


	// Return the available vid mem

	return ddcaps.dwVidMemFree;
	}






int SCREEN::Flip(BOOL vsync)
	{

	///////////////////////////////
	//							 //
	// Used to flip the surfaces //
	//							 //
	///////////////////////////////


	int retval;


	// Check the flip status of the back surface //

	retval = Back->GetFlipStatus(DDGFS_ISFLIPDONE);



	// Return false if the back surface is still drawing //

	if (retval == DDERR_WASSTILLDRAWING)
		return 0;


	
	// Wait for VBlank //

	if (vsync)
		DDraw->WaitForVerticalBlank(DDWAITVB_BLOCKEND, NULL);



	// Perform flip //

	if (FullScreen)
		retval = Primary->Flip(NULL, DDFLIP_WAIT);
	else
		{
		POINT point;
		RECT rect;
		
		point.x = 0;
		point.y = 0;

		ClientToScreen(HWnd, &point);

		rect.left	= 0;
		rect.top	= 0;
		rect.right	= Width - 1;
		rect.bottom = Height - 1;

		Primary->BltFast(point.x, point.y, Back, &rect, 0);
		}


	// If surface is lost, try to restore //

	if (retval == DDERR_SURFACELOST)
		{
		retval = DDraw->RestoreAllSurfaces();

		if (retval != DD_OK)
			return -1;
		}


	return 1;
	}





LPDIRECTDRAWSURFACE4 SCREEN::CreateOffSurface(int width, int height, int memtype)
	{

	/////////////////////////////////////////
	//									   //
	// Used to create an offscreen surface //
	//									   //
	/////////////////////////////////////////


	HRESULT retval;
	LPDIRECTDRAWSURFACE4 lpSurface;


	if (DDraw == NULL)
		return NULL;


	ZeroMemory(&SurfaceDesc, sizeof(SurfaceDesc));
	
	SurfaceDesc.dwSize			= sizeof(SurfaceDesc);
	SurfaceDesc.dwFlags			= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; 
	SurfaceDesc.dwWidth			= width; 
	SurfaceDesc.dwHeight		= height; 	


	switch(memtype)
		{
		case MEM_VIDEO:
			SurfaceDesc.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; 
			break;

		case MEM_SYSTEM:
		default:
			SurfaceDesc.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY; 
			break;
		}

	

	// Create the surface //

	retval = DDraw->CreateSurface(&SurfaceDesc, &lpSurface, NULL); 
	
	if(FAILED(retval))  
		return NULL; 
 

    // Return the surface pointer //

	return lpSurface;
	}





void  SCREEN::SetPalette(PALETTEENTRY * pal, int start, int entries)
	{

	/////////////////////////////////////////
	//									   //
	// Used to set the new palette entries //
	//									   //
	/////////////////////////////////////////


	if (Palette != NULL)
		Palette->SetEntries(0, start, entries, pal);
	}
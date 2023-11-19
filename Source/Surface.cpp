#include <windows.h>
#include "..\Header\Gamelib.h"



SURFACE::SURFACE()
	{
	Initialized = FALSE;
	Attached	= FALSE;
	Surface		= NULL;
	Width		= 0;
	Height		= 0;
	}




SURFACE::~SURFACE()
	{
	Destroy();
	}




BOOL SURFACE::Create(SCREEN * pScreen, int width, int height, int memtype)
	{

	//////////////////////////////
	//							//
	// Used to create a surface //
	//							//
	//////////////////////////////


	// Destroy the surface if already loaded 

	if (Surface != NULL)
		Destroy();

	

	// Have the screen create the surface 

	Surface = pScreen->CreateOffSurface(width, height, memtype);		



	// Return if failed 

	if (Surface == NULL)
		return FALSE;



	// Set the surface properties 

	Width		= width;
	Height		= height;

	PixelFormat = pScreen->GetPixelFormat();

	ResetClip();
	SetClip(0, 0, Width, Height);

	
	SetColorKey(0);


	Initialized = TRUE;


	return TRUE;
	}




BOOL SURFACE::CreateFromBitmap(SCREEN * pScreen, const char * filename, int memtype)
	{

	////////////////////////////////////////////
	//										  //
	// Used to load a bitmap into the surface //
	//										  //
	////////////////////////////////////////////


	HANDLE			 hFile;
	BITMAPFILEHEADER bmpfileheader;
	BITMAPINFOHEADER bmpinfoheader;
	PALETTEENTRY	 bmppalette[256];
	DWORD			 numread;
	unsigned char *	 bits, * buffer;
	int	index;
	int tempcolor;



	// Attempt to open the bitmap file //

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;



	// Read the bitmap file header //

	ReadFile(hFile, &bmpfileheader, sizeof(BITMAPFILEHEADER), &numread, NULL);



	// If the file is not a bitmap then return //

	if (bmpfileheader.bfType != BITMAP_ID)
		{
		CloseHandle(hFile);

		return FALSE;
		}



	// Read the bitmap info header //

	ReadFile(hFile, &bmpinfoheader, sizeof(BITMAPINFOHEADER), &numread, NULL);



	// If reading an 8 bit bitmap, then we need a palette //

	if (bmpinfoheader.biBitCount == 8)
		{
		ReadFile(hFile, &bmppalette, MAX_COLORS_PALETTE * sizeof(PALETTEENTRY), &numread, NULL);


		for (index = 0; index < MAX_COLORS_PALETTE; index++)
			{
			tempcolor			= bmppalette[index].peRed;
			Pal[index].peRed	= bmppalette[index].peBlue;
			Pal[index].peBlue	= tempcolor;
			Pal[index].peGreen	= bmppalette[index].peGreen;

			Pal[index].peFlags	= PC_NOCOLLAPSE;
			}
		}


	
	// Set the size of the image //

	bmpinfoheader.biSizeImage = (bmpinfoheader.biWidth * bmpinfoheader.biHeight) * (bmpinfoheader.biBitCount / 8);



	// Seek the end of the file //

	//SetFilePointer(hFile, -(int)(bmpinfoheader.biSizeImage), NULL, FILE_END);



	if (bmpinfoheader.biBitCount==8 ||
		bmpinfoheader.biBitCount==16 ||
		bmpinfoheader.biBitCount==24 ||
		bmpinfoheader.biBitCount == 32)
		{
		// Allocate a new char array for the bitmap //

		if (!(buffer = (unsigned char *)malloc(bmpinfoheader.biSizeImage)))
			{
			CloseHandle(hFile);

			return FALSE;
			}


		// Read the actual bit data //

		ReadFile(hFile, buffer, bmpinfoheader.biSizeImage, &numread, NULL);
		}
	else
		{
		return FALSE;
		}



	// Close the file when done //

	CloseHandle(hFile);



	// Create a new char array for bitmap bits after being flipped 

    bits = (unsigned char *)malloc(bmpinfoheader.biSizeImage);

	for (int i = 0; i < bmpinfoheader.biHeight; i++)
		memcpy(&bits[i * bmpinfoheader.biWidth], &buffer[(bmpinfoheader.biHeight - i - 1) * bmpinfoheader.biWidth], sizeof(unsigned char) * bmpinfoheader.biWidth);



	// Create the surface //

	if (Create(pScreen, bmpinfoheader.biWidth, bmpinfoheader.biHeight, memtype))
		CopyBits(bits);
			


	// Free the bit buffer //

	free(buffer);
	free(bits);


	return TRUE;
	}




void SURFACE::Destroy()
	{

	/////////////////////////////////
	//							   //
	// Used to destroy the surface //
	//							   //
	/////////////////////////////////


	if (Surface != NULL && Attached == FALSE)
		Surface->Release();

	Surface = NULL;
	Width	= 0;
	Height	= 0;

	Initialized = FALSE;
	}




void SURFACE::Attach(SCREEN * screen, LPDIRECTDRAWSURFACE4 surface, int width, int height)
	{

	////////////////////////////////////////////////////////////////
	//															  //
	// Used to attach a direct draw surface to this surface class //
	//															  //
	////////////////////////////////////////////////////////////////


	Surface		= surface;
	Width		= width;
	Height		= height;

	PixelFormat = screen->GetPixelFormat();
	ResetClip();

	Attached	= TRUE;

	SetClip(0, 0, Width, Height);

	Initialized = TRUE;
	}




void SURFACE::CopyBits(unsigned char * bits)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to copy bits from an array and set the surface //
	// bits												   //
	//													   //
	/////////////////////////////////////////////////////////


	DDSURFACEDESC2 sdesc;
	

	// Lock the surface 

	memset(&sdesc, 0, sizeof(DDSURFACEDESC));
	sdesc.dwSize = sizeof(DDSURFACEDESC);

	Surface->Lock(NULL, &sdesc, DDLOCK_SURFACEMEMORYPTR, NULL);
	

	// Copy the bits 

	if (PixelFormat.GetBitCount() == 8)
		{
		unsigned char * src, * dest;

		src		= bits;
		dest	= (unsigned char *)sdesc.lpSurface;

		for (int i = 0; i < sdesc.dwHeight; i++)
			{
			memcpy(dest, src, sizeof(unsigned char) * sdesc.dwWidth);

			src		+= sdesc.dwWidth;
			dest	+= sdesc.lPitch;
			}
		}
	else if (PixelFormat.GetBitCount() == 16) //|| PixelFormat.GetBitCount() == 32)
		{
		unsigned char color;
		unsigned short * dest = (unsigned short *)sdesc.lpSurface;
		int x, y;
		
		for (y = 0; y < sdesc.dwHeight; y++)
			for (x = 0; x < sdesc.dwWidth; x++)
				{
				color = bits[x + (y * sdesc.dwWidth)];
				dest[x + (y * (sdesc.lPitch / 2))] = (unsigned short)PixelFormat.ToRGB(Pal[color].peRed, Pal[color].peGreen, Pal[color].peBlue);
				}
		}
	else if (PixelFormat.GetBitCount() == 32)
		{
		unsigned char color;
		DWORD * dest = (DWORD *)sdesc.lpSurface;
		int x, y;
		
		for (y = 0; y < sdesc.dwHeight; y++)
			for (x = 0; x < sdesc.dwWidth; x++)
				{
				color = bits[x + (y * sdesc.dwWidth)];
				dest[x + (y * (sdesc.lPitch / 4))] = PixelFormat.ToRGBA(Pal[color].peRed, Pal[color].peGreen, Pal[color].peBlue, 255);
				}
		}


	// Unlock the surface
	
	Surface->Unlock(NULL);
	}





void SURFACE::BlitFast(SURFACE * src, int x, int y, BOOL trans)
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to perform a fast blit of a source surface //
	//												   //
	/////////////////////////////////////////////////////

	
	RECT rect;


	// Return if an invalid surface was passed

	if (src == NULL)
		return;


	// Setup the rect

	rect.left	= 0;
	rect.top	= 0;
	rect.right	= src->Width;
	rect.bottom = src->Height;


	// Call the blit function 

	BlitFast(src, x, y, &rect, trans);
	}




void SURFACE::BlitFast(SURFACE * src, int x, int y, RECT * srcrect, BOOL trans)
	{

	/////////////////////////////////////////////////////
	//												   //
	// Used to perform a fast blit of a source surface //
	//												   //
	/////////////////////////////////////////////////////


	RECT rect;
	int blitwidth;
	int blitheight;


	// Copy the source rect to the stack rect //

	memcpy(&rect, srcrect, sizeof(RECT));

	blitwidth	= rect.right - rect.left;
	blitheight	= rect.bottom - rect.top;



	// Perform horizontal clipping //

	if (x <= Clip.left - blitwidth)
		return;


	if (x < Clip.left)
		{
		rect.left += Clip.left - x;
		x = Clip.left;
		blitwidth = rect.right - rect.left - 1;
		}


	if (x >= Clip.right)
		return;


	if (x + blitwidth >= Clip.right)
		{
		rect.right -= (x + blitwidth) - Clip.right;
		}



	// Perform vertical clipping //

	if (y <= Clip.top - blitheight)
		return;


	if (y < Clip.top)
		{
		rect.top += Clip.top - y;
		y = Clip.top;
		blitheight = rect.bottom - rect.top - 1;
		}


	if (y >= Clip.bottom)
		return;


	if (y + blitheight >= Clip.bottom)
		{
		rect.bottom -= (y + blitheight) - Clip.bottom;
		}



	// Perform the blit //

	if (trans)
		Surface->BltFast(x, y, src->Surface, &rect, DDBLTFAST_SRCCOLORKEY);
	else
		Surface->BltFast(x, y, src->Surface, &rect, DDBLTFAST_NOCOLORKEY);
	}




void SURFACE::BlitFastScaled(SURFACE * src, int x, int y, RECT * srcrect, float scale, BOOL trans)
	{

	////////////////////////////////////////////////////////////
	//												          //
	// Used to perform a fast scaled blit of a source surface //
	//												          //
	////////////////////////////////////////////////////////////


	RECT rect;
	RECT destrect;
	int blitwidth;
	int blitheight;


	// Copy the source rect to the stack rect //

	memcpy(&rect, srcrect, sizeof(RECT));
	
	blitwidth	= rect.right - rect.left;
	blitheight	= rect.bottom - rect.top;

	destrect.left	= x;
	destrect.top	= y;
	destrect.right	= x + blitwidth * scale;
	destrect.bottom = y + blitheight * scale;


	// Perform horizontal clipping //

	if (x <= Clip.left - blitwidth)
		return;


	if (x < Clip.left)
		{
		rect.left += Clip.left - x;
		x = Clip.left;
		blitwidth = rect.right - rect.left - 1;
		}


	if (x >= Clip.right)
		return;


	if (x + blitwidth >= Clip.right)
		{
		rect.right -= (x + blitwidth) - Clip.right;
		}



	// Perform vertical clipping //

	if (y <= Clip.top - blitheight)
		return;


	if (y < Clip.top)
		{
		rect.top += Clip.top - y;
		y = Clip.top;
		blitheight = rect.bottom - rect.top - 1;
		}


	if (y >= Clip.bottom)
		return;


	if (y + blitheight >= Clip.bottom)
		{
		rect.bottom -= (y + blitheight) - Clip.bottom;
		}



	// Perform the blit //

	if (trans)
		Surface->Blt(&destrect, src->Surface, &rect, 0,NULL);
	else
		Surface->Blt(&destrect, src->Surface, &rect, 0,NULL);
	}




void SURFACE::SetClip(int left, int top, int right, int bottom)
	{

	/////////////////////////////////////////////////////////
	//													   //
	// Used to set the clipping rectangle for this surface //
	//													   //
	/////////////////////////////////////////////////////////


	if (left >= Width)		left	= Width - 1;
	if (top >= Height)		top		= Height - 1;
	if (left < 0)			left	= 0;
	if (top < 0)			top		= 0;
	if (right >= Width)		right	= Width;
	if (bottom >= Height)	bottom	= Height;
	if (right < 0)			right	= 0;
	if (bottom < 0)			bottom	= 0;
	

	Clip.left	= left;
	Clip.top	= top;
	Clip.right	= right;
	Clip.bottom	= bottom;
	}




void SURFACE::ResetClip()
	{

	///////////////////////////////////////////////////////////
	//														 //
	// Used to reset the clipping rectangle for this surface //
	//														 //
	///////////////////////////////////////////////////////////


	Clip.left	= 0;
	Clip.top	= 0;
	Clip.right	= Width;
	Clip.bottom	= Height;
	}




void SURFACE::Fill(DWORD color)
	{

	/////////////////////////////////////////
	//									   //
	// Used to fill a surface with a color //
	//									   //
	/////////////////////////////////////////


	ZeroMemory(&DDBltFX, sizeof(DDBltFX));
    
	DDBltFX.dwSize		= sizeof(DDBltFX); 
    DDBltFX.dwFillColor = color;;
 
    Surface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &DDBltFX); 
	}




void SURFACE::FillRect(int left, int top, int right, int bottom, DWORD color)
	{

	//////////////////////////////////////
	//									//
	// Used to fill a rect with a color //
	//									//
	//////////////////////////////////////


	RECT rect;

	rect.left	= left;
	rect.top	= top;
	rect.right	= right + 1;
	rect.bottom = bottom + 1;

	ZeroMemory(&DDBltFX, sizeof(DDBltFX));
    
	DDBltFX.dwSize		= sizeof(DDBltFX); 
    DDBltFX.dwFillColor = color;
 
    Surface->Blt(&rect, NULL, NULL, DDBLT_COLORFILL, &DDBltFX); 
	}





void SURFACE::HLine(int left, int right, int y, DWORD color, int thickness)
	{

	////////////////////////////////////
	//								  //
	// Used to draw a horizontal line //
	//								  //
	////////////////////////////////////


	FillRect(left, y, right, y + thickness - 1, color);
	}





void SURFACE::VLine(int top, int bottom, int x, DWORD color, int thickness)
	{

	////////////////////////////////////
	//								  //
	// Used to draw a vertical line //
	//								  //
	////////////////////////////////////


	FillRect(x, top, x + thickness - 1, bottom, color);
	}





void SURFACE::Rect(int left, int top, int right, int bottom, DWORD color, int thickness)
	{

	//////////////////////////////
	//							//
	// Used to draw a rectangle //
	//							//
	//////////////////////////////


	HLine(left, right, top, color, thickness);
	HLine(left, right, bottom - thickness + 1, color, thickness);
	VLine(top, bottom, left, color, thickness);
	VLine(top, bottom, right - thickness + 1, color, thickness);
	}





void SURFACE::SetColorKey(DWORD value)
	{ 

	///////////////////////////////
	//							 //
	// Used to set the color key //
	//							 //
	///////////////////////////////

	if (PixelFormat.GetBitCount() == 8)
		{
		ColorKey.dwColorSpaceLowValue = value; 
		ColorKey.dwColorSpaceHighValue = value; 
		}
	else
		{
		ColorKey.dwColorSpaceLowValue	= PixelFormat.ToRGB(Pal[value].peRed, Pal[value].peGreen, Pal[value].peBlue); 
		ColorKey.dwColorSpaceHighValue	= PixelFormat.ToRGB(Pal[value].peRed, Pal[value].peGreen, Pal[value].peBlue); 
		}

	Surface->SetColorKey(DDCKEY_SRCBLT, &ColorKey); 
	}
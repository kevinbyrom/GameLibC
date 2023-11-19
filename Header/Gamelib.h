#ifndef __GAMELIB_H
#define __GAMELIB_H


#define DIRECTINPUT_VERSION		0x0700

#include <ddraw.h>
#include <dinput.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include "map.h"



using namespace std;



/////////////
// DEFINES //
/////////////


#define INIT_GUID
#define WINDOW_CLASS_NAME		"GAMELIBCLASS"

#define MAX_COLORS_PALETTE		256
#define BITMAP_ID				0x4D42

#define MAX_JOYSTICKS			8
#define MAX_VPADS				8

#define VPAD_NONE				-1

#define ANIM_END				-1
#define ANIM_LOOP				-2

#define MAX_MIDI_CLIPS			25

#define MEM_SYSTEM				0
#define MEM_VIDEO				1

#define ROUND(x)	((float(x) - float(int(x))) >= 0.5f ? int(x) + 1 : int(x))

#define RGB_555(r,g,b)			((b >> 3) + ((g & 248) << 2) + ((r & 248) << 7))
#define RGB_565(r,g,b)			((b%32) + ((g%64) << 5) + ((r%32) << 11))

#define MAX_BOUNDING_SPHERES	10

#define CHECK_STATE()			if (State == -1) return;
#define BEGIN_STATE(state)		else if (State == state)
#define ON_ENTER(x)				if (x == ENTER_STATE)
#define ON_UPDATE(x)			if (x == UPDATE_STATE)
#define ON_INPUT(x)				if (x == INPUT_STATE)
#define ON_EXIT(x)				if (x == EXIT_STATE)



///////////////
// CONSTANTS //
///////////////


const int MAX_MESSAGE_TARGETS	= 50;

const int MESSAGE_DEST_ALL		= -1;
const int MESSAGE_DEST_SINGLE	= 0;
const int MESSAGE_DEST_GROUP	= 1;

const int MESSAGE_QUEUED		= -1;

const int MAX_ACTIONS			= 20;

const float GRAVITY			= 0.05f;



////////////////
// PROTOTYPES //
////////////////


class SPRITE;
class SURFACE;
class SCREEN;
class VPAD;
class INPUT;
class MIDI;
class DIRECT_MUSIC_MGR;
class MESSAGE_MGR;
class MESSAGE_TARGET;
class GAME;
class STAGE;
class TRANSITION;
class TILE_MAP;
class ACTIONS;
class OBJECT;
class STATE;
class CAMERA;
class DISPLAY_LIST_ITEM;
class DISPLAY_LIST;
class STRING_ARRAY;
class STRING;
class GAME_FILE;



/////////////
// EXTERNS //
/////////////


extern SCREEN	* Screen;
extern INPUT	* Input;
extern DIRECT_MUSIC_MGR		DMusicMgr;




////////////////
// STRUCTURES //
////////////////


struct VECTOR
	{
	VECTOR() : X(0.0f), Y(0.0f), Z(0.0f) {}

	float X, Y, Z;
	};



struct COORDINATE
	{
	COORDINATE() : X(0), Y(0), Z(0) {}
			 
	int X, Y, Z;
	};



struct COORDINATE_F
	{
	COORDINATE_F() : X(0.0f), Y(0.0f), Z(0.0f) {}
			 
	float X, Y, Z;
	};




//////////////
// GRAPHICS //
//////////////



class SURFACE_PIXEL_FORMAT
	{
	public:

		SURFACE_PIXEL_FORMAT();
		~SURFACE_PIXEL_FORMAT();

		BOOL Create(SURFACE * surface);
		BOOL Create(DDPIXELFORMAT * pixelformat);

		DWORD ToRGB(DWORD r, DWORD g, DWORD b);
		DWORD ToRGBA(DWORD r, DWORD g, DWORD b,DWORD a);
		DWORD ToAlpha(DWORD a);

		int GetBitCount() { return BitCount; }

		DWORD GetRedFast(DWORD c);
		DWORD GetRed(DWORD c);

		DWORD GetGreenFast(DWORD c);
		DWORD GetGreen(DWORD c);

		DWORD GetBlueFast(DWORD c);
		DWORD GetBlue(DWORD c);

		DDPIXELFORMAT GetDDPixelFormat() { return DDPixFormat; }


	protected:

		DWORD FindBitCount(DWORD mask);
		DWORD FindBase(DWORD mask);


	protected:

		DDPIXELFORMAT DDPixFormat;
		
		int BitCount;
		int AlphaBitCount;

		DWORD AlphaBase;
		DWORD RedBase;
		DWORD GreenBase;
		DWORD BlueBase;

		DWORD AlphaMissingBits;
		DWORD RedMissingBits;
		DWORD GreenMissingBits;
		DWORD BlueMissingBits;

		DWORD AlphaMask;
		DWORD RedMask;
		DWORD GreenMask;
		DWORD BlueMask;

		__int64 WordRedMask;
		__int64 WordGreenMask;
		__int64 WordBlueMask;

		DWORD RedMod;
		DWORD GreenMod;
		DWORD BlueMod;
	};





class SURFACE
	{
	public:

		SURFACE();
		~SURFACE();

		BOOL Create(SCREEN * pScreen, int width, int height, int memoryType = MEM_VIDEO);
		BOOL CreateFromBitmap(SCREEN * pScreen, const char * filename, int memtype = MEM_VIDEO);
		virtual void Destroy();

		void Attach(SCREEN * screen, LPDIRECTDRAWSURFACE4 surface, int width, int height);
		void CopyBits(unsigned char * bits);

		void BlitFast(SURFACE * src, int x, int y, BOOL trans = FALSE);
		void BlitFast(SURFACE * src, int x, int y, RECT * srcrect, BOOL trans = FALSE);
		void BlitFastScaled(SURFACE * src, int x, int y, RECT * srcrect, float scale, BOOL trans = FALSE);
				
		void SetClip(int left, int top, int right, int bottom);
		void ResetClip();

		void Fill(DWORD color);
		void FillRect(int left, int top, int right, int bottom, DWORD color);

		void HLine(int left, int right, int y, DWORD color, int thickness = 1);
		void VLine(int top, int bottom, int x, DWORD color, int thickness = 1);
		void Rect(int left, int top, int right, int bottom, DWORD color, int thickness = 1);

		void SetColorKey(DWORD value);

		SURFACE_PIXEL_FORMAT GetPixelFormat()	{ return PixelFormat; }
		DDPIXELFORMAT GetDDPixelFormat()		{ return PixelFormat.GetDDPixelFormat(); }


	public:

		int Initialized;

		LPDIRECTDRAWSURFACE4 Surface;
		DDBLTFX DDBltFX; 

		int Width,
			Height;

		SURFACE_PIXEL_FORMAT PixelFormat;

		RECT Clip;
		
		PALETTEENTRY Pal[256];

		DDCOLORKEY ColorKey;

		int Attached;
	};





class SPRITE : public SURFACE
	{
	public:

		SPRITE();
		~SPRITE();

		BOOL CreateFromBitmap(SCREEN * pScreen, const char * filename, int numx, int numy, int width, int height, int border, DWORD transcolor = 0, int memtype = MEM_VIDEO);
		virtual void Destroy();

		void SetRect(int num, int left, int top, int right, int bottom);

		void Draw(SURFACE * surf, int index, int x, int y, int trans = TRUE);
		void DrawScaled(SURFACE * surf, int index, int x, int y, float scale, int trans = TRUE);

		void DrawText(SURFACE * surf, char * text, int x, int y, int trans = TRUE);
		void DrawTextCentered(SURFACE * surf, char * text, int x, int y, int trans = TRUE);

		int GetNumX()	{ return NumSprX; }
		int GetNumY()	{ return NumSprY; }


	protected:

		int NumSprX;
		int NumSprY;
		int NumSprites;

		int SprWidth;
		int SprHeight;

		RECT * SprRect;
	};





class SCREEN
	{
	public:

		SCREEN();
		~SCREEN();
		
		BOOL Init(HWND hwnd, int width, int height, int bpp, int fullscreen = TRUE);
		void DeInit();

		int GetWidth()				{ return Width; }
		int GetHeight()				{ return Height; }
		int GetBPP()				{ return BPP; }

		SURFACE * GetBack()			{ return &BackSurf; }

		int TotalVidMem();
		int AvailableVidMem();
		
		int Flip(BOOL vsync = FALSE);

		LPDIRECTDRAWSURFACE4 CreateOffSurface(int width, int height, int memtype = MEM_VIDEO);

		void SetPalette(PALETTEENTRY * pal, int start, int entries);

		SURFACE_PIXEL_FORMAT GetPixelFormat()	{ return PixelFormat; }


	protected:

		int						Initialized;
		HWND					HWnd;
		int						Width;
		int						Height;
		int						BPP;
		int						FullScreen;
		int						Locked;

		LPDIRECTDRAW4			DDraw;

		LPDIRECTDRAWSURFACE4	Primary;
		LPDIRECTDRAWSURFACE4	Back;
		SURFACE					BackSurf;

		SURFACE_PIXEL_FORMAT	PixelFormat;

		LPDIRECTDRAWPALETTE		Palette;
		LPDIRECTDRAWCLIPPER		Clipper;

		DDSURFACEDESC2			SurfaceDesc;
		DDSCAPS2				DDSCaps;
	};








///////////
// INPUT //
///////////







struct MOUSE
	{
	int  X,
		 Y,
		 Z;

	unsigned char Button[4];
	};




struct JOYSTICK
	{
	LONG X,
		 Y,
		 Z,
		 RX,
		 RY,
		 RZ;

	LONG  Slider[2];
	
	DWORD POV[4];

	BYTE Button[32];
	};




struct VPAD_SETTING
	{
	VPAD_SETTING()	{ Key = VPAD_NONE; MButton = VPAD_NONE; 
					  JX = VPAD_NONE; JY = VPAD_NONE; 
					  LT = VPAD_NONE; LT = VPAD_NONE;
					  JButton = VPAD_NONE;  }

	unsigned char Key;
	unsigned char MButton;
	float JX;
	float JY;
	float LT;
	float RT;
	unsigned char JButton;
	};



class VPAD
	{
	public:

		VPAD()					{ pInput = NULL; }
		VPAD(INPUT * input);

		void Attach(INPUT * input) { pInput = input; }

		int Up();
		int Down();
		int Left();
		int Right();
	
		int UpClicked();
		int DownClicked();
		int LeftClicked();
		int RightClicked();

		int Start();
		int Select();

		int LTrigger();
		int RTrigger();

		int Button(int btn);
		int ButtonClicked(int btn);

		void AssignJoystick(int joy);

		void AssignUp(unsigned char key, float ypos, unsigned char jbutton);
		void AssignDown(unsigned char key, float ypos, unsigned char jbutton);
		void AssignLeft(unsigned char key, float xpos, unsigned char jbutton);
		void AssignRight(unsigned char key, float xpos, unsigned char jbutton);

		void AssignStart(unsigned char key, unsigned char jbutton);
		void AssignSelect(unsigned char key, unsigned char jbutton);

		void AssignLTrigger(unsigned char key, unsigned char mbutton, unsigned char jbutton);
		void AssignRTrigger(unsigned char key, unsigned char mbutton, unsigned char jbutton);

		void AssignButton(int btn, unsigned char key, unsigned char mbutton, unsigned char jbutton);


	private:
	
		INPUT * pInput;

		VPAD_SETTING VUp,
					 VDown,
					 VLeft,
					 VRight,
					 VStart,
					 VSelect,
					 VLTrigger,
					 VRTrigger,
					 VButton[10];

		int Joy;
	};




class INPUT
	{
	public:

		INPUT();
		~INPUT();

		int Init(HINSTANCE instance, HWND hwnd);
		void DeInit();

		int InstallKeyboard();
		int InstallMouse();
		int InstallJoysticks();
		
		void RemoveKeyboard();
		void RemoveMouse();
		void RemoveJoystick();

		void Poll();


	public:

		int GetKey(unsigned char key)				{ return Keyboard[key] & 0x80; }
		int GetKeyClicked(unsigned char key)		{ return (LastKeyboard[key] & 0x80 && !Keyboard[key]); }

		int GetMouseX()								{ return Mouse.X; }
		int GetMouseY()								{ return Mouse.Y; }
		int GetMouseZ()								{ return Mouse.Z; }
		int GetMouseButton(int btn)					{ return Mouse.Button[btn] == TRUE; }
		int GetMouseButtonClicked(int btn)			{ return (LastMouse.Button[btn] == TRUE && Mouse.Button[btn] != TRUE); }

		int GetJoyX(int joy)						{ return Joystick[joy].X; }
		int GetJoyY(int joy)						{ return Joystick[joy].Y; }
		int GetJoyZ(int joy)						{ return Joystick[joy].Z; }
		int GetJoyRX(int joy)						{ return Joystick[joy].RX; }
		int GetJoyRY(int joy)						{ return Joystick[joy].RY; }
		int GetJoyRZ(int joy)						{ return Joystick[joy].RZ; }
		int GetJoySlider(int joy, int pos)			{ return Joystick[joy].Slider[pos]; }
		int GetJoyButton(int joy, int btn)			{ return Joystick[joy].Button[btn] & 0x80; }
		int GetJoyButtonClicked(int joy, int btn)	{ return ((LastJoystick[joy].Button[btn] & 0x80) && !(Joystick[joy].Button[btn] & 0x80)); }
		int GetJoyPOV(int joy, int pov)				{ return Joystick[joy].POV[pov] == TRUE; }

		static BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID guid_ptr);


	private:

		int						Initialized;
		HWND					HWnd;

		LPDIRECTINPUT			DInput;

		LPDIRECTINPUTDEVICE		DIKeyboard;
		LPDIRECTINPUTDEVICE		DIMouse;
		LPDIRECTINPUTDEVICE2	DIJoystick[MAX_JOYSTICKS];

		BOOL					KeyboardInstalled,
								MouseInstalled,
								JoystickInstalled[MAX_JOYSTICKS];

		unsigned char			Keyboard[256];
		MOUSE					Mouse;
		JOYSTICK				Joystick[MAX_JOYSTICKS];

		unsigned char			LastKeyboard[256];
		MOUSE					LastMouse;
		JOYSTICK				LastJoystick[MAX_JOYSTICKS];

		void					EnumJoysticks();

		static int				NumJoysticks;
		static STRING			JoyName[MAX_JOYSTICKS];
		static GUID				JoyGUID[MAX_JOYSTICKS];

		void SetDefaultJoystickProperties(int joynum);
	};





///////////////////
// MUSIC & SOUND //
///////////////////



struct MIDI_CLIP
	{
	MIDI_CLIP()					{ Loaded = FALSE; Segment = NULL; SegmentState = NULL; ID = -1; State = -1; }

	int							Loaded;
	IDirectMusicSegment *		Segment;
	IDirectMusicSegmentState *	SegmentState;
	int							ID;
	int							State;
	};



class MIDI
	{
	public:

		MIDI();
		~MIDI();

		int Load(int index, char * filename);
		void Unload(int index);
		void ReleaseAll();
		
		void Play(int index, int loop = FALSE);
		void Stop(int index);


	protected:

		MIDI_CLIP	Clip[MAX_MIDI_CLIPS];
	};



class DIRECT_MUSIC_MGR
	{
	public:

		DIRECT_MUSIC_MGR();
		~DIRECT_MUSIC_MGR();

		int Init(HWND hwnd, LPDIRECTSOUND dsound);
		void DeInit();

		int LoadMidi(char * filename, MIDI_CLIP * clip);


	private:

		int						Initialized;

		IDirectMusicPerformance	*	DMPerf;
		IDirectMusicLoader *		DMLoader;
	};




///////////////
// MESSAGING //
///////////////



class MESSAGE
	{
	public:
		
		int		DestType;
		int		From;
		int		To;
		int		Msg;
		int		Time;
		DWORD	Data1;
		DWORD	Data2;

		MESSAGE * Prev, * Next;
	};




class MESSAGE_TARGET_ITEM
	{
	public:

		MESSAGE_TARGET * Target;
		int GroupID;
	};




class MESSAGE_MGR
	{
	public:

		MESSAGE_MGR();
		~MESSAGE_MGR();

		void Update();

		int RegisterTarget(MESSAGE_TARGET * target, int group);
		void RemoveTargets();
		MESSAGE_TARGET * GetTargetByID(int id) { if (id < 0 || id >= NumTargets) return NULL; return Target[id].Target; }

		DWORD SendMessage(int from, int to, int msg, int time = 0, DWORD data1 = 0, DWORD data2 = 0);
		DWORD SendMessageToGroup(int from, int togroup, int msg, int time = 0, DWORD data1 = 0, DWORD data2 = 0);
		
		void ForceMessages(int id, int sentby = -1);
		void ForceGroupMessages(int id, int sentby = -1);

		void FlushMessages(int id, int sentby = -1);
		void FlushGroupMessages(int id, int sentby = -1);

		
	protected:

		void AddQueuedMessage(MESSAGE * msg);
		void RemoveQueuedMessage(MESSAGE * msg);
		void RemoveAllQueuedMessages();

		DWORD DispatchMessage(MESSAGE * msg);
		DWORD DispatchMessageToSingle(MESSAGE * msg);
		DWORD DispatchMessageToGroup(MESSAGE * msg);
		DWORD DispatchMessageToAll(MESSAGE * msg);

		int NumTargets;
		MESSAGE_TARGET_ITEM Target[MAX_MESSAGE_TARGETS];

		MESSAGE * FirstQueued, * LastQueued;
	};





class MESSAGE_TARGET
	{
	public:

		int EnableMessaging(MESSAGE_MGR * mgr, int group = 0)	{ MessageMgr = mgr; if (MessageMgr == NULL) return -1; MsgTargetID = MessageMgr->RegisterTarget(this, group); return MsgTargetID; }

		virtual DWORD OnMessage(int from, int msg, DWORD data1, DWORD data2) { return 0; }

		DWORD SendMessage(int to, int msg, int time = 0,  DWORD data1 = 0, DWORD data2 = 0) { if (MessageMgr != NULL) return MessageMgr->SendMessage(MsgTargetID, to, msg, time, data1, data2); return -1; }
		DWORD SendMessageToGroup(int togroup, int msg, int time = 0, DWORD data1 = 0, DWORD data2 = 0) { if (MessageMgr != NULL) return MessageMgr->SendMessageToGroup(MsgTargetID, togroup, msg, time, data1, data2); return -1; }

		int GetMsgTargetID() { return MsgTargetID; }


	protected:

		MESSAGE_MGR * MessageMgr;

		int MsgTargetID;
	};





//////////////////
// GAME OBJECTS //
//////////////////



class GAME
	{
	public:

		/////////////////////////////
		// Init / DeInit functions //
		/////////////////////////////

		BOOL Init(HINSTANCE hInst, WNDPROC wproc, const char * wndname, int width = 320, int height = 240, int bpp = 8, int fullscreen = 1, int fps = 30);
		void ShutDown();

		void End()								{ Exiting = TRUE; }
		BOOL IsExiting()						{ return Exiting; }


		///////////////////////////////////
		// Overridable Virtual Functions //
		///////////////////////////////////
		
		virtual void Loop();
		virtual void Opening(){}
		virtual void Closing(){}


		////////////////////////////////////////
		// State and Transition set functions //
		////////////////////////////////////////

		void SetStage(STAGE * stage);
		void SetTransition(TRANSITION * transition);


		///////////////////////////////
		// Property attain functions //
		///////////////////////////////

		HINSTANCE	GetInstance()				{ return Instance; }
		HWND		GetWindow()					{ return Wnd; }

		BOOL		IsFullScreen()				{ return FullScreen; }


		//////////////////////
		// Palette Routines //
		//////////////////////

		//void SetPalette(PALETTEENTRY * pPal)	{ m_pDisplay->SetPalette(pPal); }
		//PALETTEENTRY * GetPalette()				{ return m_pDisplay->GetPalette(); }
		

		////////////////////
		// Debug Routines //
		////////////////////

		int GetFPSCount()						{ return LastFPSCount; }



	protected:

		///////////////////////
		// Window Properties //
		///////////////////////

		HINSTANCE	Instance;
		HWND		Wnd;
		char		WndName[256];
		WNDPROC		WndProc;
		BOOL		FullScreen;

		STAGE		* CurrStage;
		TRANSITION	* Transition;


		/////////////////
		// Game States //
		/////////////////

		BOOL	Exiting;
		BOOL	InTransition;
		

		////////////////////////////////
		// Frames Per Second Counters //
		////////////////////////////////

		int		FPS;
		DWORD	LastTickCount;


		////////////////////////
		// Debug FPS Counters //
		////////////////////////

		int FPSCount;
		int LastFPSCount;
		DWORD LastTCount;

		virtual int CreateMainWindow(int Width, int Height, BOOL FullScreen);
	};





class STAGE : public MESSAGE_TARGET
	{
	public:

		///////////////////
		// Overrideables //
		///////////////////

		virtual void Opening()						{}
		virtual void Closing(STAGE * stage)			{}
		virtual void Loop()							{}


	protected:

		int State;
	};




class TRANSITION
	{
	public:

		TRANSITION()								{ Done = TRUE; }

		virtual void Start()						{ Done = FALSE; }
		virtual void Loop()							{}
		
		int IsDone()								{ return Done; }


	protected:

		int Done;
	};




class TILE_MAP
	{
	public:

		TILE_MAP();
		~TILE_MAP();

		void Create(int numx, int numy, int resx, int resy);
		void Create(unsigned char * tiles, int numx, int numy, int resx, int resy);

		void Destroy();

		void Resize(int numx, int numy);

		unsigned char GetTile(int x, int y);
		void SetTile(int x, int y, unsigned char val);
		void SetRow(int row, unsigned char val);
		void SetColumn(int col, unsigned char val);
		void Fill(unsigned char val);

		int NumTilesX()			{ return SizeX; }
		int NumTilesY()			{ return SizeY; }

		void SetSprite(SPRITE * sprite)		{ Sprite = sprite; }

		void Draw(SURFACE * surface, int x, int y, int width, int height, int sx, int sy);


	protected:

		SPRITE * Sprite;

		unsigned char * Tile;

		int SizeX, SizeY, ResX, ResY;
	};





class GAME_TIMER
	{
	public:

		GAME_TIMER();
		~GAME_TIMER();

		static void ProcessAll(int msecs);
		static void AddTimer(GAME_TIMER * timer);
		static void RemoveTimer(GAME_TIMER * timer);

		BOOL IsDone();
		BOOL IsEnabled();
		void Enable(BOOL val);
	
		void CountUp(int val = 0);
		void CountDown(int val = 1000);

		void CountUpSecs(int val = 0);
		void CountDownSecs(int val = 1);

		int GetCount();


	protected:

		void Process(int msecs);


	protected:

		BOOL Enabled;
		BOOL Done;
		BOOL CountDirUp;
		BOOL MSecs;

		int Count;

		GAME_TIMER * Prev;
		GAME_TIMER * Next;

		static GAME_TIMER * First;
		static GAME_TIMER * Last;
	};




/////////////////
// MESSAGE MAP //
/////////////////


typedef void (OBJECT::*MESSAGE_MAP_FUNCTION)(int);
typedef map<int, MESSAGE_MAP_FUNCTION> MESSAGE_MAP;
typedef MESSAGE_MAP::value_type MESSAGE_MAP_VALUE;
typedef MESSAGE_MAP::iterator MESSAGE_MAP_ITERATOR;



/////////////
// OBJECTS //
/////////////


const int ACTION_END					= -1;

#define DEFINE_ACTION(actionname, actionnum)		const int ACTION_##actionname = actionnum;
#define DECLARE_ACTION_MAP()						virtual void DefineActionMap();
#define BEGIN_ACTION_MAP(classname, basename)		void classname::DefineActionMap(){ basename::DefineActionMap();
#define END_ACTION_MAP()							}
#define ACTION(actionnum, funcname)					ActionMap.insert(MESSAGE_MAP_VALUE(ACTION_##actionnum, (MESSAGE_MAP_FUNCTION)funcname));
#define ON_ACTION_ENTER(x)							if ((x) == ENTER_ACTION)
#define ON_ACTION_UPDATE(x)						if ((x) == UPDATE_ACTION)
#define ON_ACTION_MESSAGE(x)						if ((x) == MESSAGE_ACTION)
#define ON_ACTION_EXIT(x)							if ((x) == EXIT_ACTION)



struct ACTION
	{
	int Type;
	int Time;
	float Param[3];
	};




class ACTIONS
	{
	public:

		ACTIONS()		{ Clear(); }

		void Clear()	{ NumActions = 0; DelayTime = 0; }

		void Pop();
		void Push(int type, int time = -1, float param1 = 0.0f, float param2 = 0.0f, float param3 = 0.0f);
		void PushEnd(int type, int time = -1, float param1 = 0.0f, float param2 = 0.0f, float param3 = 0.0f);

		void Delay(int amt)	{ DelayTime = amt; }
		BOOL IsDelayed()	{ return (DelayTime > 0); }

		int	GetNumActions()	{ return NumActions; }

		int		GetType()		{ if (NumActions == 0) return -1; return Action[0].Type; }
		int		GetTime()		{ if (NumActions == 0) return -1; return Action[0].Time; }
		float	GetParam(int p)	{ if (NumActions == 0 || p < 0 || p >= 3) return 0.0f; return Action[0].Param[p]; }

		void SetTime(int time);
		void DecrementTime(int amt)			{ if (NumActions == 0) return; Action[0].Time -= amt; }
		void DecrementDelayTime(int amt)	{ if (DelayTime > 0) { DelayTime -= amt;  if (DelayTime < 0) DelayTime = 0; }}


	protected:

		int NumActions;
		int DelayTime;

		ACTION Action[MAX_ACTIONS];
	};





struct BOUNDING_SPHERE
	{
	COORDINATE_F	Offset;
	float			Radius;
	};





#define DEFINE_STATE(statename, statenum)			const int STATE_##statename = statenum;
#define DECLARE_STATE_MAP()							virtual void DefineStateMap();
#define BEGIN_STATE_MAP(classname, basename)		void classname::DefineStateMap(){ basename::DefineStateMap();
#define END_STATE_MAP()								}
#define STATE(statenum, funcname)					StateMap.insert(MESSAGE_MAP_VALUE(STATE_##statenum, (MESSAGE_MAP_FUNCTION)funcname));
#define ON_STATE_ENTER(x)							if ((x) == ENTER_STATE)
#define ON_STATE_UPDATE(x)							if ((x) == UPDATE_STATE)
#define ON_STATE_INPUT(x)							if ((x) == INPUT_STATE)
#define ON_STATE_EXIT(x)							if ((x) == EXIT_STATE)




class OBJECT : public MESSAGE_TARGET
	{
	public:

		OBJECT();

		enum { ENTER_STATE = 0,
			   UPDATE_STATE = 1,
			   INPUT_STATE = 2,
			   EXIT_STATE = 3 };

		enum { ENTER_ACTION = 0,
			   UPDATE_ACTION,
			   EXIT_ACTION };


		virtual void Init()	{ DefineActionMap(); DefineStateMap(); }

		void Draw(SURFACE * surface);
		void Draw(SURFACE * surface, int frame, int x, int y);

		virtual void Update();

		void UpdateMove();
		virtual void CheckCoordinates(){}

		void UpdateAnimation();
		void SetAnimation(int * anim)			{ Anim.Frames = anim; }
		void SetAnimationSpeed(int animspeed)	{ Anim.Speed = animspeed; }
		void SetAnimationPos(int pos)			{ Anim.Pos = pos; }
		void Animate(int val)					{ Anim.Enabled = val; }

		virtual void SetState(int state);
		void UpdateState();
		void DoState(int reason);
	
		virtual void UpdateDeltas();

		virtual void PreUpdate()	{}
		virtual void PostUpdate()	{}

		void UpdateAction();
		void DoAction(int reason);
		void DoNextAction();
		
		void LockObject(OBJECT * obj, BOOL lockx = TRUE, BOOL locky = TRUE, BOOL lockz = TRUE, float offsetx = 0.0f, float offsety = 0.0f, float offsetz = 0.0f);
		void UnlockObject();

		void SetNumBounds(int num)				{ NumBounds = num; }
		void SetBounds(int index, float radius, float xoffset = 0.0f, float yoffset = 0.0f, float zoffset = 0.0f);
		BOOL Collision(OBJECT * object);

		int Roll(int sides);
		BOOL Roll(int sides, int modifier, int difficulty);


		void StopVelocity();
		void Accelerate(float accel, float maxvel);


	public:

		//////////////////////
		// Basic Attributes //
		//////////////////////

		COORDINATE_F	Wld;							// World position
		COORDINATE_F	LastWld;						// Last World position
		COORDINATE_F	Dest;							// Destination position
		COORDINATE		Scr;							// Screen position
		COORDINATE		SprOffset;						// Sprite offsets

		VECTOR			MoveDir;						// Direction of movement
		VECTOR			Delta;							// Delta vector
		VECTOR			Vel;							// Velocity vector
		VECTOR			DestVel;						// Destination velocity
		VECTOR			Acc;							// Acceleration vector
	
		float			TotalVel;						// Total velocity

		float			Friction;						// Friction value

		int				NumBounds;						// Number of bounding spheres
		BOUNDING_SPHERE	Bound[MAX_BOUNDING_SPHERES];	// Bounding sphere

		float			Width,
						Height;

		int				Visible;


		////////////////////////
		// Sprite & Animation //
		////////////////////////

		SPRITE * Sprite;
		int Frame;

		struct
			{
			int Enabled;
			int Frame;
			int * Frames;
			int Pos;
			int Speed;
			int Count;	
			} Anim;


		////////////////////
		// Object locking //
		////////////////////

		struct 
			{
			int Enabled;
			OBJECT * Object;
			BOOL LockX;
			BOOL LockY;
			BOOL LockZ;
			float OffsetX;
			float OffsetY;
			float OffsetZ;
			} Lock;


		////////////////////
		// SFX attributes //
		////////////////////

		int Scaled;
		float Scale;


		/////////////
		// Actions //
		/////////////

		ACTIONS Actions;
		MESSAGE_MAP ActionMap;
		virtual void DefineActionMap(){}


		////////////
		// States //
		////////////

		int State;
		int StateCount;
		int StateTime;
		BOOL StateDone;

		MESSAGE_MAP StateMap;
		virtual void DefineStateMap(){}



		///////////
		// Input //
		///////////

		BOOL InputEnabled;


	public:

		///////////////////////////
		// Display List Tracking //
		///////////////////////////

		void AddToDisplayList(DISPLAY_LIST * dlist);

		DISPLAY_LIST		* DisplayList;
	};




class STATE 
	{
	public:

		void Enter(OBJECT * obj)						{ OnEnter(obj); }
		void Exit(OBJECT * obj)							{ OnExit(obj); }
		void Update(OBJECT * obj)						{ OnUpdate(obj); }
		void ProcessInput(OBJECT * obj, VPAD * vpad)	{ OnProcessInput(obj, vpad); }


	protected:
	
		virtual void OnEnter(OBJECT * obj){}
		virtual void OnExit(OBJECT * obj){}
		virtual void OnUpdate(OBJECT * obj){}
		virtual void OnProcessInput(OBJECT * obj, VPAD * vpad){}
	};





class CAMERA : public OBJECT
	{
	public:

		CAMERA();

		virtual void Draw(SURFACE * surface, int x, int y) {}
	};




class DISPLAY_LIST_ITEM
	{
	public:

		DISPLAY_LIST_ITEM() { Object = NULL; Prev = NULL; Next = NULL; }

		OBJECT * Object;

		DISPLAY_LIST_ITEM * Prev,
						  * Next;
	};




class DISPLAY_LIST
	{
	public:

		DISPLAY_LIST();
		~DISPLAY_LIST();

		void AddObject(OBJECT * object);
		void RemoveObject(OBJECT * object);
		void RemoveAll();

		DISPLAY_LIST_ITEM * GetDisplayListItemFromObject(OBJECT * obj);
		void ReorderDown(OBJECT * obj);
		void ReorderUp(OBJECT * obj);
		
		void DrawAll(SURFACE * surface);

		
	protected:

		DISPLAY_LIST_ITEM * FirstItem,
						  * LastItem;
	};




class STRING_ARRAY
	{
	public:

		STRING_ARRAY();
		STRING_ARRAY(const STRING_ARRAY & s);
		~STRING_ARRAY();

		void Add(STRING & s);
		void Add(STRING * s);
		void Add(const char * s);

		void RemoveAt(int index);
		void Clear()				{ DeallocateArray(); }

		int Count()					{ return StrCount; }
		
		STRING_ARRAY & operator = (const STRING_ARRAY & s);
		STRING operator [] (const int index) const;


	protected:

		int ArraySize;
		int StrCount;
		STRING ** Strings;


	protected:

		BOOL AllocateArray(int size);
		void DeallocateArray();
		void DeallocateStrings();
		void DeallocateString(int index);

		void AddStringToArray(STRING * pstr);
	};



class STRING
	{
	public:

		STRING();
		STRING(const STRING * s);
		STRING(const STRING & s);
		STRING(const char * s);

		~STRING();


		void Clear()		{ DeallocateBuffer(); }
		int Length()		{ return StrLength; }
	
		

		// Selection routines

		STRING_ARRAY Split(char delimeter);
		STRING Mid(int start, int count);


		// Conversion routines

		STRING ToUpper();
		STRING ToLower();


		// Trim routines

		STRING TrimLeft();
		STRING TrimRight();
		STRING Trim();


		// Pad routines

		STRING PadLeft(char padchar, int num);
		STRING PadRight(char padchar, int num);


		// Assignment operators

		STRING & operator = (const STRING * x);
		STRING & operator = (const STRING & x);
		STRING & operator = (char x);
		STRING & operator = (unsigned char x);
		STRING & operator = (const char * x);
		STRING & operator = (const unsigned char * x);
		STRING & operator = (int x);
		STRING & operator = (unsigned int x);
		STRING & operator = (float x);


		// Comparision operators 

		BOOL operator == (const STRING * x);
		BOOL operator != (const STRING * x);
		BOOL operator == (const STRING & x);
		BOOL operator != (const STRING & x);

		BOOL operator == (const char * x);
		BOOL operator != (const char * x);


		// Concatenate operators

		STRING & operator +=	(const STRING * x);
		STRING & operator +=	(const STRING & x);
		STRING & operator +=	(const char x);
		STRING & operator +=	(const char * x);
		STRING & operator +=	(int x);	
		STRING & operator +=	(unsigned int x);
		STRING & operator +=	(float x);

		STRING operator +	(const STRING * x) const;
		STRING operator +	(const STRING & x) const;
		STRING operator +	(const char x) const;
		STRING operator +	(const char * x) const;
		STRING operator +	(int x) const;	
		STRING operator +	(unsigned int x) const;
		STRING operator +	(float x) const;
		

		// Indexer operators 

		char operator [] (const int x) const;


		// Typecast operators

		operator char * ();
		operator const char * ();
		operator int ();
		operator float ();


	protected:

		int		BufferSize;
		char *	Buffer;	
		int		StrLength;


	protected:

		BOOL AllocateBuffer(int size);
		void DeallocateBuffer();
	
		void SetBufferString(const char * str);
		void SetBufferString(const char c);
		
		void ConcatBufferString(const char * str);
		void ConcatBufferString(const char c);
	};




class GAME_FILE
	{
	public:

		enum{
				MODE_READ = 1,
				MODE_WRITE
			};


		GAME_FILE();
		~GAME_FILE();

		BOOL OpenForReading(const char * strfile);
		BOOL OpenForWriting(const char * strfile);
		void Close();

		BOOL WriteRaw(void * buffer, int size);
		BOOL ReadRaw(void * buffer, int size, DWORD * numread = NULL);

		BOOL WriteLine(STRING * line);
		STRING ReadLine(BOOL skipblanks = FALSE, BOOL skipcomments = TRUE);

		BOOL WriteParams(STRING_ARRAY params);
		STRING_ARRAY ReadParams(BOOL trimarray = TRUE);
		


	protected:

		HANDLE m_hFile;

		BOOL m_FileIsOpen;

		int m_Mode;
	};




class CONSOLE
	{
	public:

		void AddMessage(STRING string);
		void ParseCommand(STRING cmd);

	protected:

		STRING_ARRAY Strings;
	};



#endif
 
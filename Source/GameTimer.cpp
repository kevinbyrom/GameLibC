#include <windows.h>
#include <math.h>
#include "..\Header\Gamelib.h"




GAME_TIMER * GAME_TIMER::First	= NULL;
GAME_TIMER * GAME_TIMER::Last	= NULL;





void GAME_TIMER::ProcessAll(int msecs)
	{

	////////////////////////////////////////
	//									  //
	// Used to process all enabled timers //
	//									  //
	////////////////////////////////////////


	GAME_TIMER * timer = First;


	while(timer)
		{
		if (timer->IsEnabled())
			timer->Process(msecs);

		timer = timer->Next;
		}
	}





void GAME_TIMER::AddTimer(GAME_TIMER * timer)
	{

	///////////////////////////////////////////
	//										 //
	// Used to add a timer to the timer list //
	//										 //
	///////////////////////////////////////////


	if (!Last)
		{
		timer->Prev = NULL;		

		Last		= timer;
		First		= timer;
		}
	else
		{
		timer->Prev = Last;
		Last = timer;
		}

	timer->Next = NULL;
	}





void GAME_TIMER::RemoveTimer(GAME_TIMER * timer)
	{

	////////////////////////////////////////////////
	//											  //
	// Used to remove a timer from the timer list //
	//											  //
	////////////////////////////////////////////////


	GAME_TIMER * curr = First;
	GAME_TIMER * next;

	while(curr)
		{
		next = curr->Next;

		// If the timer is found, remove it

		if (curr == timer)
			{
			if (timer == First)
				First = timer->Next;

			if (timer == Last)
				Last = timer->Next;

			if (timer->Prev)
				timer->Prev->Next = timer->Next;

			if (timer->Next)
				timer->Next->Prev = timer->Prev;
			}

		curr = next;
		}
	}





GAME_TIMER::GAME_TIMER()
	{
	Enabled		= FALSE;
	Done		= FALSE;
	CountDirUp	= FALSE;
	MSecs		= FALSE;
	Count		= 0;

	Prev = NULL;
	Next = NULL;

	AddTimer(this);
	}





GAME_TIMER::~GAME_TIMER()
	{
	RemoveTimer(this);
	}





BOOL GAME_TIMER::IsDone()
	{ 

	///////////////////////////////////////////////
	//											 //
	// Used to check if the timer is done or not //
	//											 //
	///////////////////////////////////////////////


	return Done; 
	}





BOOL GAME_TIMER::IsEnabled()
	{ 

	//////////////////////////////////////////////////
	//												//
	// Used to check if the timer is enabled or not //
	//												//
	//////////////////////////////////////////////////


	return Enabled; 
	}





void GAME_TIMER::Enable(BOOL val)	
	{ 

	////////////////////////////////////////
	//									  //
	// Used to enable / disable the timer //
	//									  //
	////////////////////////////////////////


	Enabled = val; 
	}





void GAME_TIMER::CountUp(int val)
	{ 

	//////////////////////////////////////
	//									//
	// Used to count up in milliseconds //
	//									//
	//////////////////////////////////////


	Count		= val; 
	CountDirUp	= TRUE; 
	MSecs		= TRUE; 
	Done		= FALSE; 
	Enabled		= TRUE; 
	}





void GAME_TIMER::CountDown(int val)	
	{ 

	////////////////////////////////////////
	//									  //
	// Used to count down in milliseconds //
	//									  //
	////////////////////////////////////////


	Count		= val; 
	CountDirUp	= FALSE; 
	MSecs		= TRUE; 
	Done		= FALSE; 
	Enabled		= TRUE; 
	}




void GAME_TIMER::CountUpSecs(int val)
	{ 

	/////////////////////////////////
	//							   //
	// Used to count up in seconds //
	//							   //
	/////////////////////////////////


	Count		= val * 1000; 
	CountDirUp	= TRUE; 
	MSecs		= FALSE; 
	Done		= FALSE; 
	Enabled		= TRUE; 
	}





void GAME_TIMER::CountDownSecs(int val)	
	{ 

	///////////////////////////////////
	//								 //
	// Used to count down in seconds //
	//								 //
	///////////////////////////////////


	Count			= val * 1000; 
	CountDirUp		= FALSE; 
	MSecs			= FALSE; 
	Done			= FALSE; 
	Enabled			= TRUE; 
	}





int GAME_TIMER::GetCount()
	{

	//////////////////////////////////////
	//									//
	// Used to get the count of a timer //
	//									//
	//////////////////////////////////////


	// If the timer is milliseconds, just return the count

	if (MSecs == TRUE)
		return Count;


	// Otherwise, convert the count to seconds

	if (CountDirUp)
		return int(Count / 1000);
	else
		return int(ceil((double)Count / 1000));
	}




void GAME_TIMER::Process(int msecs)
	{

	///////////////////////////////
	//							 //
	// Used to process the timer //
	//							 //
	///////////////////////////////


	if (CountDirUp)
		Count += msecs;
	else
		{
		Count -= msecs;

		if (Count <= 0)
			Done = TRUE;
		}
	}
#include <windows.h>
#include "..\Header\Gamelib.h"





int MIDI::Load(int index, char * filename)
	{

	//////////////////////////////
	//							//
	// Used to load a midi clip //
	//							//
	//////////////////////////////


	return DMusicMgr.LoadMidi(filename, &Clip[index]);
	}




void MIDI::Unload(int index)
	{
	
	////////////////////////////////
	//							  //
	// Used to unload a midi clip //
	//							  //
	////////////////////////////////


	if (index < 0 || index >= MAX_MIDI_CLIPS || !Clip[index].Loaded)
		return;


	// Release the segment 

	if (Clip[index].Segment != NULL)
		{
		Clip[index].Segment->Release();
		Clip[index].Segment = NULL;
		}


	// Release the segment state 

	if (Clip[index].SegmentState != NULL)
		{
		Clip[index].SegmentState->Release();
		Clip[index].SegmentState = NULL;
		}


	Clip[index].Loaded = FALSE;
	}




void MIDI::ReleaseAll()
	{

	////////////////////////////////////
	//								  //
	// Used to release all midi clips //
	//								  //
	////////////////////////////////////


	for (int i = 0; i < MAX_MIDI_CLIPS; i++)
		{
		Unload(i);
		}
	}



		
void MIDI::Play(int index, int loop)
	{
	}




void MIDI::Stop(int index)
	{
	}
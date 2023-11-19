#include <windows.h>
#include "..\Header\Gamelib.h"




VPAD::VPAD(INPUT * input)
	{
	pInput	= input;
	Joy		= 0;
	}




int VPAD::Up()
	{

	////////////////////////////////////
	//								  //
	// Used to return the value of UP //
	//								  //
	////////////////////////////////////


	return ((VUp.Key		!= VPAD_NONE	&& pInput->GetKey(VUp.Key))			|| 
			(VUp.JY			!= VPAD_NONE	&& pInput->GetJoyY(Joy) <= VUp.JY)	||
			(VUp.JButton	!= VPAD_NONE	&& pInput->GetJoyButton(Joy, VUp.JButton)));
	}




int VPAD::Down()
	{

	//////////////////////////////////////
	//								    //
	// Used to return the value of DOWN //
	//								    //
	//////////////////////////////////////


	return ((VDown.Key		!= VPAD_NONE	&& pInput->GetKey(VDown.Key))		|| 
			(VDown.JY		!= VPAD_NONE	&& pInput->GetJoyY(Joy) >= VDown.JY)	||
			(VDown.JButton	!= VPAD_NONE	&& pInput->GetJoyButton(Joy, VDown.JButton)));
	}




int VPAD::Left()
	{

	//////////////////////////////////////
	//								    //
	// Used to return the value of LEFT //
	//								    //
	//////////////////////////////////////


	return ((VLeft.Key		!= VPAD_NONE	&& pInput->GetKey(VLeft.Key))		|| 
			(VLeft.JX		!= VPAD_NONE	&& pInput->GetJoyX(Joy) <= VLeft.JX)	||
			(VLeft.JButton	!= VPAD_NONE	&& pInput->GetJoyButton(Joy, VLeft.JButton)));
	}




int VPAD::Right()
	{

	///////////////////////////////////////
	//								     //
	// Used to return the value of RIGHT //
	//								     //
	///////////////////////////////////////


	return ((VRight.Key		!= VPAD_NONE	&& pInput->GetKey(VRight.Key))		|| 
			(VRight.JX		!= VPAD_NONE	&& pInput->GetJoyX(Joy) >= VRight.JX)	||
			(VRight.JButton != VPAD_NONE	&& pInput->GetJoyButton(Joy, VRight.JButton)));
	}




int VPAD::UpClicked()
	{

	////////////////////////////////////////////
	//								          //
	// Used to return the value of UP Clicked //
	//								          //
	////////////////////////////////////////////


	return ((VUp.Key		!= VPAD_NONE	&& pInput->GetKeyClicked(VUp.Key))			|| 
			(VUp.JY			!= VPAD_NONE	&& pInput->GetJoyY(Joy) > VUp.JY)	||
			(VUp.JButton	!= VPAD_NONE	&& pInput->GetJoyButtonClicked(Joy, VUp.JButton)));
	}




int VPAD::DownClicked()
	{

	//////////////////////////////////////////////
	//								            //
	// Used to return the value of DOWN Clicked //
	//								            //
	//////////////////////////////////////////////


	return ((VDown.Key		!= VPAD_NONE	&& pInput->GetKeyClicked(VDown.Key))		|| 
			(VDown.JY		!= VPAD_NONE	&& pInput->GetJoyY(Joy) < VDown.JY)	||
			(VDown.JButton	!= VPAD_NONE	&& pInput->GetJoyButtonClicked(Joy, VDown.JButton)));
	}




int VPAD::LeftClicked()
	{

	//////////////////////////////////////////////
	//								            //
	// Used to return the value of LEFT Clicked //
	//								            //
	//////////////////////////////////////////////


	return ((VLeft.Key		!= VPAD_NONE	&& pInput->GetKeyClicked(VLeft.Key))		|| 
			(VLeft.JX		!= VPAD_NONE	&& pInput->GetJoyX(Joy) < VDown.JX)	||
			(VLeft.JButton	!= VPAD_NONE	&& pInput->GetJoyButtonClicked(Joy, VLeft.JButton)));
	}




int VPAD::RightClicked()
	{

	///////////////////////////////////////////////
	//											 //
	// Used to return the value of RIGHT Clicked //
	//											 //
	///////////////////////////////////////////////


	return ((VRight.Key		!= VPAD_NONE	&& pInput->GetKeyClicked(VRight.Key))		|| 
			(VRight.JX		!= VPAD_NONE	&& pInput->GetJoyX(Joy) > VDown.JX)	||
			(VRight.JButton != VPAD_NONE	&& pInput->GetJoyButtonClicked(Joy, VRight.JButton)));
	}




int VPAD::Start()
	{

	///////////////////////////////////////
	//								     //
	// Used to return the value of START //
	//								     //
	///////////////////////////////////////


	return (pInput->GetKey(VStart.Key) ||
			pInput->GetJoyButton(Joy, VStart.JButton));
	}




int VPAD::Select()
	{

	////////////////////////////////////////
	//								      //
	// Used to return the value of SELECT //
	//								      //
	////////////////////////////////////////


	return (pInput->GetKey(VSelect.Key) ||
			pInput->GetJoyButton(Joy, VSelect.JButton));
	}




int VPAD::LTrigger()
	{

	//////////////////////////////////////////
	//								        //
	// Used to return the value of LTRIGGER //
	//								        //
	//////////////////////////////////////////


	return (pInput->GetKey(VLTrigger.Key)				|| 
			pInput->GetMouseButton(VLTrigger.MButton)	||
			pInput->GetJoyButton(Joy, VLTrigger.JButton));
	}




int VPAD::RTrigger()
	{

	//////////////////////////////////////////
	//										//
	// Used to return the value of RTRIGGER //
	//										//
	//////////////////////////////////////////


	return (pInput->GetKey(VRTrigger.Key)				|| 
			pInput->GetMouseButton(VRTrigger.MButton)	||
			pInput->GetJoyButton(Joy, VRTrigger.JButton));
	}




int VPAD::Button(int btn)
	{

	////////////////////////////////////////
	//									  //
	// Used to return the value of BUTTON //
	//									  //
	////////////////////////////////////////


	if (btn < 0 || btn >= 10)
		return FALSE;

	return (pInput->GetKey(VButton[btn].Key)				|| 
			pInput->GetMouseButton(VButton[btn].MButton)	||
			pInput->GetJoyButton(Joy, VButton[btn].JButton));
	}




int VPAD::ButtonClicked(int btn)
	{

	////////////////////////////////////////////////
	//									          //
	// Used to return the value of BUTTON CLICKED //
	//									          //
	////////////////////////////////////////////////


	if (btn < 0 || btn >= 10)
		return FALSE;

	return (pInput->GetKeyClicked(VButton[btn].Key)				|| 
			pInput->GetMouseButtonClicked(VButton[btn].MButton)	||
			pInput->GetJoyButtonClicked(Joy, VButton[btn].JButton));
	}




void VPAD::AssignJoystick(int joy)
	{

	///////////////////////////////////////////////////////////
	//														 //
	// Used to assign which joystick the VPAD is attached to //
	//														 //
	///////////////////////////////////////////////////////////


	if (joy >= 0 && joy < MAX_JOYSTICKS)
		Joy = joy;
	else
		Joy = 0;
	}




void VPAD::AssignUp(unsigned char key, float ypos, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VUp.Key		= key;
	VUp.JY		= ypos;
	VUp.JButton	= jbutton;
	}




void VPAD::AssignDown(unsigned char key, float ypos, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VDown.Key		= key;
	VDown.JY		= ypos;
	VDown.JButton	= jbutton;
	}




void VPAD::AssignLeft(unsigned char key, float xpos, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VLeft.Key		= key;
	VLeft.JX		= xpos;
	VLeft.JButton	= jbutton;
	}




void VPAD::AssignRight(unsigned char key, float xpos, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VRight.Key		= key;
	VRight.JX		= xpos;
	VRight.JButton	= jbutton;
	}




void VPAD::AssignStart(unsigned char key, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VStart.Key		= key;
	VStart.JButton	= jbutton;
	}




void VPAD::AssignSelect(unsigned char key, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VSelect.Key		= key;
	VSelect.JButton	= jbutton;
	}




void VPAD::AssignLTrigger(unsigned char key, unsigned char mbutton, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VLTrigger.Key		= key;
	VLTrigger.MButton 	= mbutton;
	VLTrigger.JButton	= jbutton;
	}




void VPAD::AssignRTrigger(unsigned char key, unsigned char mbutton, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	VRTrigger.Key		= key;
	VRTrigger.MButton 	= mbutton;
	VRTrigger.JButton	= jbutton;
	}




void VPAD::AssignButton(int btn, unsigned char key, unsigned char mbutton, unsigned char jbutton)
	{

	/////////////////////////////////
	//							   //
	// Used to assign the VPAD key //
	//							   //
	/////////////////////////////////


	if (btn >= 0 || btn < 10)
		{
		VButton[btn].Key		= key;
		VButton[btn].MButton 	= mbutton;
		VButton[btn].JButton	= jbutton;
		}
	}
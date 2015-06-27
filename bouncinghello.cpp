//makes a bouncing hello world

// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h> //needed to play sound. Also have to insert winmm.lib C:\Program Files\Microsoft Visual Studio\VC98\Lib
#include <string.h>
#include <stdio.h>
#include <math.h>

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINXCLASS"  // class name
#define WINDOW_WIDTH  420              // size of window
#define WINDOW_HEIGHT 200

// MACROS /////////////////////////////////////////////////

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// PROTOTYPES /////////////////////////////////////////////


// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
char buffer[80];                // used to print text

// FUNCTIONS //////////////////////////////////////////////

//\\\\\\\\\\\\\\\\\\\\\\\\\ WinProc ///////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC			hdc;	   //creates a handle to the device context

	//find out what the message is
	switch(msg)
	{	
	
	case WM_CREATE: //called when window is created
		{// do initialization stuff here
			return(0);
		} break;

	case WM_PAINT: //called when window needs repainting
		{//simply validate the window
		    
			//ValidateRect(hwnd, NULL);
			hdc = BeginPaint(hwnd,&ps);
		    EndPaint(hwnd,&ps);
			
			return(0);
		} break;

	case WM_DESTROY: 
		{// kill the application			
			//close the program
			PostQuitMessage(0);
			return(0);
		} break;

	default:break;

    } // end main switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

//\\\\\\\\\\\\\\\\\\\\\\\\ WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{

	WNDCLASS winclass;	// this will hold the class created
	HWND	 hwnd;		// generic window handle
	MSG		 msg;		// generic message
	HDC      hdc;       // generic dc
	//PAINTSTRUCT ps;     // generic paintstruct
	
	float gravity=1.0f, 
		  velocity= 0.0f, 
		  friction= 0.999f;
	int msg_x=0,
		msg_y= WINDOW_HEIGHT/2,
		msg2_x= WINDOW_WIDTH,
		msg2_y= WINDOW_HEIGHT/2;

	//fill in the window class stucture
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName	= WINDOW_CLASS_NAME;

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);

	// create the window
	if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
							  "WinX Game Console",	 // title
							  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						 	  200,200,	   // x,y
							  WINDOW_WIDTH,  // width
						      WINDOW_HEIGHT, // height
							  NULL,	   // handle to parent 
							  NULL,	   // handle to menu
							  hinstance,// instance
							  NULL)))	// creation parms
	{
		MessageBox(hwnd, "Window Could not be Created", NULL, MB_OK); //NULL is default for Error
		return(0);
	}

	// save the window handle and instance in a global
	main_window_handle = hwnd;
	main_instance      = hinstance;

	
	// enter main event loop
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit message
			if (msg.message == WM_QUIT)
				break;
	
			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
    
    
		hdc= GetDC(hwnd);
		
		SetBkMode(hdc, TRANSPARENT); //set background color
		
		//first create the eraser string
		SetTextColor(hdc,RGB(0,0,0)); //set text color black
		TextOut(hdc, msg_x, msg_y,"Hello", strlen("Hello")); //print text

		SetTextColor(hdc,RGB(0,0,0)); //set text color black
		TextOut(hdc, msg2_x, msg2_y,"World", strlen("World")); //print text
		
		velocity+= gravity;
		velocity*= friction;
		msg_x+= (int) velocity; //hello msg. comes in from the left
		msg2_x-= (int) velocity; //world msg. world comes in from the right so its opposite hello

		//eraser rectanle for when word goes past middle of screen
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		Rectangle(hdc, (WINDOW_WIDTH/2)-60, WINDOW_HEIGHT/2, (WINDOW_WIDTH/2)+60, (WINDOW_HEIGHT/2)+20);
		
		//move the strings
		SetTextColor(hdc,RGB(255,255,255)); //set text color white
		TextOut(hdc, msg_x, msg_y,"Hello", strlen("Hello")); //print text, NULL means to the NULL at the end of the string
		
		SetTextColor(hdc,RGB(255,255,255)); //set text color white
		TextOut(hdc, msg2_x, msg2_y,"World", strlen("World")); //print text

		if(msg_x+35 >= WINDOW_WIDTH/2)
		{
			velocity = -velocity;
			msg_x = (WINDOW_WIDTH/2)-35;
			msg2_x = (WINDOW_WIDTH/2)+5;
		}
		
		//SelectObject(hdc, GetStockObject(WHITE_PEN));
		//MoveToEx(hdc, 0,0, NULL); //just the starting curse. does not draw. It holds the starting point of the line
		LineTo(hdc, 0, 0); //draws line from MoveToEx() spot to LineTo() spot. 
		
		
		//slow things down a bit
		Sleep(10);
		ReleaseDC(hwnd, hdc);

	} // end while

	// return to Windows like this
	return(msg.wParam);

} // end WinMain


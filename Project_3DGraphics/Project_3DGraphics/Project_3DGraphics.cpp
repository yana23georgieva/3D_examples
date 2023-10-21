// Win32Project2OGL.cpp : Defines the entry point for the application.
//

//#include "stdafx.h"
#include "framework.h"
#include "Project_3DGraphics.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "glut.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//arrays for dynamic light Ex 4 Pyramid 
GLfloat ambient_light[] = { 0.0, 0.7, 0.0, 0.2 }; // color for AMBIENT light 
GLfloat diffuse_light[] = { 1.0, 1.0, 1.0, 0.0 }; // color for DIFFUSE light 
/*GLfloat light_position[] = { 0.0, 0.0, -20.0, 0.0 }; // source position
GLfloat emission[] = { 0.8, 0.8, 0.0, 1.0 }; // color for EMISSION light  
GLfloat emission_none[] = { 0.0, 0.0, 0.0, 1.0 };// black color for shut off EMISSION 
GLfloat light_direction[] = { 0.0, 0.0, -20.0, 0.0 };*/
float r = 0;

//Ex5 Sun and planets
float Sphere_Angle = 0.0;
GLfloat light_position[] = { 0.0, 0.0, -12.0, 1.0 };
int position = 0;

//Ex5 Part 2
float Sphere_Translate_left = 70.0;
float Sphere_Translate_right = -60.0;
float Sphere_Position = 60.0;
int direction = -1;

//Exercise 6
int a = 1;
int showPoints = 0;

HGLRC hRC = NULL;
HDC hdc;
HWND hWnd;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void DefineView(int width, int height); //prototype of the initializer function
void Render(); // render function prototype
void Lighting(void); // prototype of the dynamic light feature

//Exercise 6
void Render_NURBSSurf(void);
void Render_NURBS(void);
void Render_BezierSurf(void);
void Render_Bezier(void);
void Render_Hat(void);

void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {

		sizeof(PIXELFORMATDESCRIPTOR), // Size of this structure
		1,                             // Version of this structure
		PFD_DRAW_TO_WINDOW |           // Draw to window (not to bitmap)
		PFD_SUPPORT_OPENGL |           // Support OpenGL calls in window
		PFD_DOUBLEBUFFER,              // Double-buffered mode
		PFD_TYPE_RGBA,                 // RGBA color mode
		32,                            // Want 32-bit color
		0, 0, 0, 0, 0, 0,                   // Not used to select mode

		0, 0,                           // Not used to select mode
		0, 0, 0, 0, 0,                     // Not used to select mode
		16,                            // Size of depth buffer
		0,                             // Not used here
		0,                             // Not used here
		0,                             // Not used here
		0,                             // Not used here
		0, 0, 0 };                       // Not used here

	// Choose a pixel format that best matches that described in pfd

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJECT3DGRAPHICS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT3DGRAPHICS));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_PROJECT3DGRAPHICS));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_PROJECT3DGRAPHICS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		// store the device context
		hdc = GetDC(hWnd);
		// selecting the pixel format
		SetDCPixelFormat(hdc);
		// creating a render context and targeting as current
		hRC = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hRC);

		break;
		// Window is being destroyed, clean up
	case WM_DESTROY:

		// deselect the current render context and delete
		hdc = GetDC(hWnd);
		wglMakeCurrent(hdc, NULL);
		wglDeleteContext(hRC);

		//the application stops after closing the window  

		PostQuitMessage(0);
		break;

		// Resize the window.

	case WM_SIZE:
		// Call the function that sets the size of the window and view
	{ 
		int height = HIWORD(lParam);
		int width = LOWORD(lParam);
		DefineView(width, height);
	}
	break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);EndPaint(hWnd, &ps);


		//Render();	// TODO: Add any drawing code here...
		//Render_NURBSSurf();
		//Render_NURBS();
		//Render_BezierSurf();
		//Render_Bezier();
		Render_Hat();

		break;
	case WM_LBUTTONDOWN:
		position++;
		if (5 == position) {
			position = 0;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void DefineView(int width, int height)
{
    if (height == 0) height = 1;

    // set view to full screen
    glViewport(0, 0, width, height);
    // loading the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // specifying the type of projection  
    gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.1, 200.0);
    // loading the model matrix        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // loading dynamic light parameters
    Lighting();
    // determining distances to objects
    glEnable(GL_DEPTH_TEST);
    // set background color
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void Lighting(void)
{ // lighting

    glEnable(GL_LIGHTING); // turn on the dynamic light
        // global backlight
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
    // determination of figure illumination in AMBIENT and DIFFUSE light
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_light);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_light);
    // set object color to be preserved when dynamic light is turned on
    glEnable(GL_COLOR_MATERIAL);
    // determining the types of light involved in the final coloring
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

//Exercise 6 Mexican hat
void Render_Hat(void)
{
	// clearing buffers and initialization

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// directional source lighting
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	GLfloat position[] = { -5.0, -10.0, 2.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);  glMaterialf(GL_FRONT, GL_DIFFUSE, 30.0);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	// determining the position of the observer
	gluLookAt(0.0, -55.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	GLfloat ctlpoints[9][9][3] = 
	{
		{{3, 0, 0}, {3, 2, 0}, {3, 4, 0}, {3, 6, -1}, {3, 8, -1}, {3, 10, -1}, {3, 12, 0}, {3, 14, 0}, {3, 16, 0}},
		{{5, 0, 0}, {5, 2, -1}, {5, 4, -1}, {5, 6, -3}, {5, 8, -3}, {5, 10, -3}, {5, 12, -1}, {5, 14, -1}, {5, 16, 0}},
		{{7, 0, 0}, {7, 2, -1}, {7, 4, -3}, {7, 6, -3}, {7, 8, -1}, {7, 10, -3}, {7, 12, -3}, {7, 14, -1}, {7, 16, 0}},
		{{9, 0, -1}, {9, 2, -3}, {9, 4, -3}, {9, 6, 6}, {9, 8, 13}, {9, 10, 6}, {9, 12, -3}, {9, 14, -3}, {9, 16, -1}},
		{{11, 0, -1}, {11, 2, -3}, {11, 4, -1}, {11, 6, 13}, {11, 8, 24}, {11, 10, 13}, {11, 12, -1}, {11, 14, -3}, {11, 16, -1}},
		{{13, 0, -1}, {13, 2, -3}, {13, 4, -3}, {13, 6, 6}, {13, 8, 13}, {13, 10, 6}, {13, 12, -3}, {13, 14, -3}, {13, 16, -1}},
		{{15, 0, 0}, {15, 2, -1}, {15, 4, -3}, {15, 6, -3}, {15, 8, -1}, {15, 10, -3}, {15, 12, -3}, {15, 14, -1}, {15, 16, 0}},
		{{17, 0, 0}, {17, 2, -1}, {17, 4, -1}, {17, 6, -3}, {17, 8, -3}, {17, 10, -3}, {17, 12, -1}, {17, 14, -1}, {17, 16, 0}},
		{{19, 0, 0}, {19, 2, 0}, {19, 4, 0}, {19, 6, -1}, {19, 8, -1}, {19, 10, -1}, {19, 12, 0}, {19, 14, 0}, {19, 16, 0}}
	};

	// an array of nodes
	GLfloat knots[13] = { 0.0, 0.0, 0.0, 0.0, 0.25, 0.35, 0.45, 0.65, 0.85, 1.0, 1.0, 1.0, 1.0 };

	r += 0.2; if (r >= 360) r = 0.0;

	// creating an object
	GLUnurbsObj* theNurb;
	theNurb = gluNewNurbsRenderer();
	// set characteristics
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	// preview with rotation
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 0.8, 1.0);
	glRotatef(r, 0.0, 0.0, 1.0);
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 13, knots, 13, knots,  3,9 * 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);

	showPoints = true;

	if (showPoints) {
		glPointSize(3.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				glVertex3f(ctlpoints[i][j][0],
					ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
	gluDeleteNurbsRenderer(theNurb);

	// change buffers	
	SwapBuffers(hdc);
	// returning control to the system
	ReleaseDC(hWnd, hdc);
}

void Render_NURBSSurf(void) // draws a plane, with a clipped area
{
	// clearing buffers and initialization

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// directional source lighting
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	GLfloat position[] = { -5.0, -10.0, 2.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);  glMaterialf(GL_FRONT, GL_DIFFUSE, 30.0);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	// determining the position of the observer
	gluLookAt(0.0, 0.0, -15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	GLfloat ctlpoints[4][4][3]; // an array of control points
	int u, v;
	for (u = 0; u < 4; u++) {
		for (v = 0; v < 4; v++) {
			ctlpoints[u][v][0] = 2.0 * ((GLfloat)u - 1.5);
			ctlpoints[u][v][1] = 2.0 * ((GLfloat)v - 1.5);
			if (((u == 1) || (u == 2)) && ((v == 1) || (v == 2)))
				ctlpoints[u][v][2] = 3.0;
			else
				ctlpoints[u][v][2] = -3.0;
		}
	}

	// an array of nodes
	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };


	// crop line arrays
	GLfloat edgePt[5][2] = /* counter clockwise */ //covers all the coordinates of the figure
	{ { 0.0, 0.0 }, { 1.0, 0.0 }, { 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0 } };
	GLfloat curvePt[4][2] = /* clockwise */
	{ { 0.25, 0.5 }, { 0.25, 0.75 }, { 0.75, 0.75 }, { 0.75, 0.5 } };
	GLfloat curveKnots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	GLfloat pwlPt[4][2] = /* clockwise */{ { 0.75, 0.5 }, { 0.5, 0.25 }, { 0.25, 0.5 } };

	r += 0.5; if (r >= 360) r = 0.0;

	// creating an object
	GLUnurbsObj* theNurb;
	theNurb = gluNewNurbsRenderer();
	// set characteristics
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	// preview with rotation
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	glRotatef(r, 0.0, 1.0, 0.0);
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3, &ctlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);

	// clipping
	gluBeginTrim(theNurb);
	gluPwlCurve(theNurb, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
	gluEndTrim(theNurb);
	gluBeginTrim(theNurb);
	gluNurbsCurve(theNurb, 8, curveKnots, 2, &curvePt[0][0], 4, GLU_MAP1_TRIM_2);
	gluPwlCurve(theNurb, 3, &pwlPt[0][0], 2, GLU_MAP1_TRIM_2);
	gluEndTrim(theNurb);
	gluEndSurface(theNurb);

	showPoints = true;

	if (showPoints) {
		glPointSize(3.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				glVertex3f(ctlpoints[i][j][0],
					ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
	gluDeleteNurbsRenderer(theNurb);

	// changing buffers	
	SwapBuffers(hdc);
	// returning control to the system
	ReleaseDC(hWnd, hdc);
}

void Render_NURBS(void) // draws a base spline
{	// an array of checkpoints
	GLfloat ctlpoints[7][4][3] = {
		{ { -1.5, -2.5, 3.0 }, { -0.5, -2.5, 2.0 },
		{ 0.5, -2.5, -1.0 }, { 1.5, -2.5, 2.0 } },
		{ { -1.5, -1.5, 1.0 }, { -0.5, -1.5, 3.0 },
		{ 0.5, -1.5, 0.0 }, { 1.5, -1.5, -1.0 } },
		{ { -1.5, 0.5, 3.0 }, { -0.5, 0.5, 0.0 },
		{ 0.5, 0.5, 3.0 }, { 1.5, 0.5, 4.0 } },
		{ { -1.5, 2.5, -2.0 }, { -0.5, 2.5, -2.0 },
		{ 0.5, 2.5, 0.0 }, { 1.5, 2.5, -1.0 } },
		{ { -1.5, -1.5, 3.0 }, { -0.5, -1.5, 2.0 },
		{ 0.5, -1.5, -1.0 }, { 1.5, -1.5, 2.0 } },
		{ { -1.5, -1.5, 1.0 }, { -0.5, -1.5, 3.0 },
		{ 0.5, -1.5, 0.0 }, { 1.5, -1.5, -1.0 } },
		{ { -1.5, 1.5, 3.0 }, { -0.5, 1.5, 0.0 },
		{ 0.5, 1.5, 3.0 }, { 1.5, 1.5, 4.0 } }
	};

	r += 0.02; 	if (r >= 360.0)
	{
		r = 0.00; a++;
	}
	// an array of nodes
	GLfloat knots[11] = { 0.0, 0.0, 0.0, 0.0, 0.34, 0.58, 0.76, 1.0, 1.0, 1.0, 1.0 };
	// delete the contents of the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();// initial initialization 
	// setting the lighting
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialf(GL_FRONT, GL_DIFFUSE, 50.0);
	glEnable(GL_COLOR_MATERIAL);
	// determining the position of the observer
	gluLookAt(0.0, 0.0, 13.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// creating an object
	GLUnurbsObj* theNurb;
	theNurb = gluNewNurbsRenderer();
	// description of the characteristics of the object
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 20.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
	// object rotation
	glPushMatrix();
	glRotatef(r, 0.0, 1.0, 0.0);
	// object color
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 1.0);
	// plotting the curve
	gluBeginCurve(theNurb);
	if (a > 4) a = 1;
	gluNurbsCurve(theNurb, 11, knots, a * 3, &ctlpoints[0][0][0], 4, GL_MAP1_VERTEX_3);
	gluEndCurve(theNurb);
	glPopMatrix();
	gluDeleteNurbsRenderer(theNurb);
	// changing buffers		
	SwapBuffers(hdc);
	// returning control to the system
	ReleaseDC(hWnd, hdc);
}

void Render_BezierSurf(void) // draws a plane using Bezier splines
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	r += 0.001; if (r >= 6) r = 0.0;
	// setting the lighting
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialf(GL_FRONT, GL_DIFFUSE, 50.0);
	glEnable(GL_COLOR_MATERIAL);

	// determining the position of the observer
	gluLookAt(0.0, 0.0, -15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// array of checkpoints
	GLfloat ctrlpoints[4][4][3] = {
	{{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0},
	{0.5, +1.5, -1.0}, {1.5, -1.5, 2.0}},
	{{-1.5, -0.5 + r, 1.0}, {-0.5, -0.5 + r, 3.0},
	{0.5, -0.5 + r, 0.0}, {1.5, -0.5 + r, -1.0}},
	{{-1.5, 0.5 + r, 4.0}, {-0.5, 0.5 + r, 0.0},
	{0.5, 0.5 + r, 3.0}, {1.5, 0.5 + r, 4.0}},
	{{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0},
	{0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
	};

	// map formation
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

	// determination of coloration
	glDisable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
	glColor3f(0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	int i, j;
	// calculation and visualization of the points of the plane
	glPushMatrix(); // set rotation
	glRotatef(r * 60, 0.0, 1.0, 0.0);

	// drawing lines on the surface
	for (j = 0; j <= 8; j++) {
		glBegin(GL_LINES);
		for (i = 0; i <= 30; i++)
			glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 8.0);
		glEnd();
		glBegin(GL_LINES);
		for (i = 0; i <= 30; i++)
			glEvalCoord2f((GLfloat)j / 8.0, (GLfloat)i / 30.0);
		glEnd();
	}

	/* //drawing a solid surface
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	*/

	glPopMatrix();
	SwapBuffers(hdc);
	ReleaseDC(hWnd, hdc);
}

void Render_Bezier(void) // reads Bezier curves
{
	// delete the contents of the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity(); // initial initialization   
	// setting the lighting
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialf(GL_FRONT, GL_DIFFUSE, 50.0);
	glEnable(GL_COLOR_MATERIAL);

	// determining the position of the observer
	gluLookAt(0.0, -10.0, -50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// setting movement of the control points
	r += 0.002; if (r >= 23.0) r = 0.0;
	GLfloat ctrlpoints[7][3]; // array of checkpoints
	// setting control and control point values
	ctrlpoints[0][0] = -4.0; ctrlpoints[0][1] = -4.0; ctrlpoints[0][2] = 0.0;
	ctrlpoints[1][0] = -2.0 + r; ctrlpoints[1][1] = 4.0 - r; ctrlpoints[1][2] = 0.0 + r;
	ctrlpoints[2][0] = 2.0 - r; ctrlpoints[2][1] = -4.0 - 0.2 * r; ctrlpoints[2][2] = 0.0;
	ctrlpoints[3][0] = 4.0; ctrlpoints[3][1] = 4.0; ctrlpoints[3][2] = 0.0;
	ctrlpoints[4][0] = 6.0 - r; ctrlpoints[4][1] = 6.0 + 0.5 * r; ctrlpoints[4][2] = 0.0 + r;
	ctrlpoints[5][0] = -2.0 + r; ctrlpoints[5][1] = 8.0 - r; ctrlpoints[5][2] = 0.0;
	ctrlpoints[6][0] = -4.0; ctrlpoints[6][1] = -4.0; ctrlpoints[6][2] = 0.0;

	// uniform coloring of the object
	glShadeModel(GL_FLAT);
	int i;

	// setting a line color
	glColor3f(1.0, 0.0, 0.0);

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);

	glBegin(GL_LINE_STRIP);
	// calculating the coordinates of the points on the curve
	for (i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();

	// an alternative variant of drawing through a grid on the first line
	glPointSize(3.0); // size of points on the line
	glMapGrid1f(30, 0.0, 1.0);
	// visualization of the dotted curve without the first and last points
	glEvalMesh1(GL_POINT, 1, 29);

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[3][0]);
	glEnable(GL_MAP1_VERTEX_3);

	glBegin(GL_LINE_STRIP);
	// calculating the coordinates of the points on the curve
	for (i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();

	// alternative option of grid drawing on the second line
	glPointSize(3.0); // size of points on the line
	glMapGrid1f(30, 0.0, 1.0);
	// visualization of the dotted curve without the first and last points
	glEvalMesh1(GL_POINT, 1, 29);

	glBegin(GL_LINE_STRIP);
	// calculating the coordinates of the points on the curve
	for (i = 0; i <= 30; i++)
		glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();


	// set visualization of endpoints and control points  
	glDisable(GL_LIGHTING);
	glPointSize(5.0); // size of control points
	glColor3f(1.0, 0.0, 1.0); // color of dots

	glBegin(GL_POINTS);
	for (i = 0; i < 7; i++)
		glVertex3fv(&ctrlpoints[i][0]);
	glEnd();
	// changing buffers	
	SwapBuffers(hdc);
	// returning control to the system
	ReleaseDC(hWnd, hdc);
}

//Ex5 Torus and sphere
//void Render(void)
//{ // delete the contents of the buffers
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glLoadIdentity(); // initial initialization
//  // determining the position of the observer        
//	r += 0.1; if (r >= 18.0) r = 0.0;
//	// turning on a light source
//	glEnable(GL_LIGHT0);
//	// determining DIFFUSE radiation
//	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
//	// source position determination
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//	// оlight brightness setting
//	glMaterialf(GL_FRONT, GL_DIFFUSE, 50.0);
//	glEnable(GL_COLOR_MATERIAL);
//	//  define angular rotation step for shapes
//	Sphere_Angle += 0.5;
//
//	// determining the position of the observer
//	switch (position)
//	{
//	case 0:
//		gluLookAt(0.0, -10.0, -50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//		break;
//	case 1:
//		gluLookAt(180.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//		break;
//	case 2:
//		gluLookAt(0.0, 180.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//		break;
//	case 3:
//		gluLookAt(60.0, 0.0, 120.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//		break;
//	case 4:
//		gluLookAt(0.0, 10.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//		break;
//	default:
//		break;
//	}
//	// placing the service model die in the shape stack
//	// positioning of the sun
//	glPushMatrix();
//	// set shape color 
//	if (Sphere_Angle >= 140 && Sphere_Angle <= 200) {
//		glColor3f(1.0, 0.3, 0.0);
//	} else {
//		glColor3f(0.8, 0.5, 0.0);
//	}
//
//	if (Sphere_Angle == 360.0) {
//		Sphere_Angle = 0.0;
//	}
//
//	glutSolidTorus(3.0, 10, 60, 60);
//	glPopMatrix();
//
//	// positioning
//	glPushMatrix();
//	// rotation
//	glTranslatef(-10.0, 0.0, 0.0);
//	glRotatef(Sphere_Angle, 0.0, 1.0, 0.0);
//	// offset
//	glTranslatef(-10.0, 0.0, 0.0);
//	// set shape color   	   
//	glColor3f(1.0, 0.0, 0.0);
//	glutSolidSphere(3, 10, 10); // solid pattern
//	glPopMatrix();
//
//	// changing buffers	
//	SwapBuffers(hdc);
//	// returning control to the system
//	ReleaseDC(hWnd, hdc);
//}

//Ex5 5 Torus and sphere Part 2 
//void Render(void)
//{ // delete the contents of the buffers
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	static bool isTouched = false;
//	static bool isTouchedTorus2 = false;
//
//	glLoadIdentity(); // initial initialization 
//  // determining the position of the observer             
//	r += 0.1; if (r >= 18.0) r = 0.0;
//	// turning on a light source
//	glEnable(GL_LIGHT0);
//	// determining DIFFUSE radiation
//	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 130.0);
//	// determining the position of the observer
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//	// light brightness setting
//	glMaterialf(GL_FRONT, GL_DIFFUSE, 50.0);
//	glEnable(GL_COLOR_MATERIAL);
//	// define angular rotation step for shapes
//	Sphere_Angle += 0.5;
//
//	// determining the position of the observer
//	gluLookAt(0.0, 0.0, -60.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//
//	glColor3f(0.8, 0.5, 0.0);
//
//	// Torus 1 
//	glPushMatrix();
//	// set shape color
//
//	glTranslatef(50.0, 0.0, 0.0);
//	glRotatef(120.0, 0.0, 1.0, 0.0);
//
//	glutSolidTorus(2.0, 7, 60, 60);
//	glPopMatrix();
//
//	if (Sphere_Position >= 15.0 && Sphere_Position <= 25.0 && Sphere_Angle > 45.0 && Sphere_Angle < 135.0 ||
//		Sphere_Position >= 10.0 && Sphere_Position <= 5.0 && Sphere_Angle > 225.0 && Sphere_Angle < 315.0)
//	{
//		direction = -direction;
//		isTouched = true;
//	}
//
//	// Torus 2
//	glPushMatrix();
//	// set shape color 
//	if (isTouched)
//	{
//		glColor3f(0.0, 0.0, 0.0);
//	}
//
//	glTranslatef(20.0, 0.0, 0.0);
//
//	// rotation about its axis
//	glRotatef(Sphere_Angle, 0.0, 1.0, 0.0);
//
//	glutSolidTorus(2.0, 7, 60, 60);
//	glPopMatrix();
//
//	// Torus 3 
//	glPushMatrix();
//	// set shape color   
//	glColor3f(0.8, 0.5, 0.0);
//	glRotatef(90.0, 0.0, 1.0, 0.0);
//
//	glutSolidTorus(2.0, 7, 60, 60);
//	glPopMatrix();
//
//	if (Sphere_Position >= -25.0 && Sphere_Position <= -15.0 && Sphere_Angle > 45.0 && Sphere_Angle < 135.0 ||
//		Sphere_Position >= -30.0 && Sphere_Position <= -35.0 && Sphere_Angle > 225.0 && Sphere_Angle < 315.0)
//	{
//		direction = -direction;
//		isTouchedTorus2 = true;
//	}
//
//	// Torus 4
//	glPushMatrix();
//	// set shape color 
//	if (isTouchedTorus2)
//	{
//		glColor3f(0.0, 0.0, 0.0);
//	}
//
//	glTranslatef(-20.0, 0.0, 0.0);
//
//	// rotation about its axis
//	glRotatef(Sphere_Angle, 0.0, 1.0, 0.0);
//
//	glutSolidTorus(2.0, 7, 60, 60);
//	glPopMatrix();
//
//	// Torus 5
//	glPushMatrix();
//	// set shape color  
//	glColor3f(0.8, 0.5, 0.0);
//
//	glTranslatef(-50.0, 0.0, 0.0);
//	glRotatef(-120.0, 0.0, 1.0, 0.0);
//
//	glutSolidTorus(2.0, 7, 60, 60);
//	glPopMatrix();
//
//	if (Sphere_Angle == 360) {
//		Sphere_Angle = 0.0;
//	}
//
//	// positioning
//	glPushMatrix();
//	// offset
//	glTranslatef(Sphere_Position, 0.0, 0.0);
//	Sphere_Position += direction * 0.2;
//	if (Sphere_Position <= Sphere_Translate_right) {
//		direction = 1;
//		isTouched = false;
//		isTouchedTorus2 = false;
//	} else if (Sphere_Position >= Sphere_Translate_left) {
//		direction = -1;
//		isTouched = false;
//		isTouchedTorus2 = false;
//	}
//
//	// set shape color	   
//	glColor3f(1.0, 0.0, 0.0);
//	glutSolidSphere(3, 10, 10); // solid pattern
//	glPopMatrix();
//
//	// changing buffers		
//	SwapBuffers(hdc);
//	// returning control to the system
//	ReleaseDC(hWnd, hdc);
//}

//Ex 4 Pyramid
/*void Render()
{ // delete the contents of the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // loading parameters 
        // determining DIFFUSE radiation
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    // source position determination
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 70.0);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);


    // turning on a light source
    glEnable(GL_LIGHT0);
    // observer position control             
    r += 0.06; if (r >= 360.0) r = 0.0;
    // determining the position of the observer/camera

    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 15.0, 0.0, 1.0, 0.0);
	glTranslated(0.0, 2.0, 15.00);
	glRotated(r, 0.0, 1.0, 0.0);
    glPushMatrix();
    // set shape color
	glColor3f(0.0f, 1.00f, 0.0f); 
    // creating the figure

	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid 
	  // Front
	glVertex3f(0.0f, 5.0f, 0.0f);   // 5
	glVertex3f(5.0f, -5.0f, 5.0f);  // 1
	glVertex3f(-5.0f, -5.0f, 5.0f); // 4

	// Right
	glVertex3f(5.0f, -5.0f, 5.0f);  // 1
	glVertex3f(5.0f, -5.0f, -5.0f); // 2
	glVertex3f(-5.0f, -5.0f, 5.0f); // 4

	// Back
	glVertex3f(5.0f, -5.0f, -5.0f); // 2
	glVertex3f(-5.0f, -5.0f, -5.0f);// 3
	glVertex3f(0.0f, 5.0f, 0.0f);   // 5

	// Left
	glVertex3f(-5.0f, -5.0f, -5.0f);// 3
	glVertex3f(-5.0f, -5.0f, 5.0f);// 4
	glVertex3f(0.0f, 5.0f, 0.0f);   // 5
	glEnd();   // Done drawing the pyramid

	glBegin(GL_QUADS);
	glVertex3f(5.0f, -5.0f, 5.0f);  // 1
	glVertex3f(5.0f, -5.0f, -5.0f); // 2
	glVertex3f(-5.0f, -5.0f, -5.0f);// 3
	glVertex3f(-5.0f, -5.0f, 5.0f);// 4
	glEnd();

    // pop the array off the stack
    glPopMatrix();
    // changing buffers	
    SwapBuffers(hdc);
    // returning control to the system
    ReleaseDC(hWnd, hdc);
}*/

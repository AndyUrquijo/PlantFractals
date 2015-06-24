/*		WinApp.cpp		by Andres Urquijo		*/

#include "WinApp.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include "GLRenderer.h"


UINT		WinApp::clientWidth		= 1600;
UINT		WinApp::clientHeight	= 900;

HINSTANCE	WinApp::application;
WNDPROC		WinApp::appWndProc;
HWND		WinApp::window;
HDC			WinApp::deviceContext;
HGLRC		WinApp::renderContext;

PIXELFORMATDESCRIPTOR GetPixelFormatDescriptor( )
{
	PIXELFORMATDESCRIPTOR pfDesc = { };
	pfDesc.nSize = sizeof( pfDesc );
	pfDesc.nVersion = 1;
	pfDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfDesc.iPixelType = PFD_TYPE_RGBA;
	pfDesc.cColorBits = 24;
	pfDesc.cDepthBits = 16;
	pfDesc.iLayerType = PFD_MAIN_PLANE;

	return pfDesc;
}

void WinApp::CreateApp( HINSTANCE hinst, WNDPROC proc )
{
	application = hinst;
	appWndProc = proc;

	InitializeExtensions( );

	WNDCLASSEX  wndClass = { };
	wndClass.cbSize = sizeof( WNDCLASSEX );
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"Fractals";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndClass.hbrBackground = (HBRUSH) ( COLOR_WINDOWFRAME );

	if ( !RegisterClassEx( &wndClass ) ) ShowErrorMessage( L"Could not register window class" );

	RECT window_size = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect( &window_size, WS_OVERLAPPEDWINDOW, false );

	window = CreateWindow( wndClass.lpszClassName, wndClass.lpszClassName, WS_OVERLAPPEDWINDOW & ~( WS_THICKFRAME | WS_MAXIMIZEBOX ),
						   CW_USEDEFAULT, CW_USEDEFAULT,
						   window_size.right - window_size.left,
						   window_size.bottom - window_size.top,
						   NULL, NULL, application, NULL );

	InitializeOpenGL( );

	ShowWindow( window, SW_SHOW );

}

void WinApp::TerminateApp( )
{
	wglDeleteContext( renderContext );
	ReleaseDC( window, deviceContext );
	wglMakeCurrent( NULL, NULL );
}

void WinApp::ShowErrorMessage( PCWSTR message, PCWSTR title )
{
	MessageBox( HWND_DESKTOP, message, title, MB_OK | MB_ICONERROR );
}

HRESULT WinApp::ShowErrorMessage( HRESULT hr, PCWSTR message, PCWSTR title )
{
	ShowErrorMessage( message, title );
	return hr;
}

void WinApp::InitializeExtensions( )
{
	BOOL fail = FALSE;

	HDC tempDC = GetDC( NULL );

	PIXELFORMATDESCRIPTOR pfDesc = GetPixelFormatDescriptor();
	
	int iPixelFormat = ChoosePixelFormat(tempDC, &pfDesc);
	DWORD error = GetLastError();

	fail |=!	SetPixelFormat( tempDC, iPixelFormat, &pfDesc );

	HGLRC tempRC = wglCreateContext( tempDC );
	fail |=!	wglMakeCurrent( tempDC, tempRC );

	fail |= 	glewInit( );

	fail |=!	wglMakeCurrent( NULL, NULL );
	fail |=!	wglDeleteContext( tempRC );
	fail |=!	ReleaseDC( NULL, tempDC );
	
	if(fail) ShowErrorMessage(L"The OpenGL Extensions could not be initialized");
}

void WinApp::InitializeOpenGL( )
{
	BOOL fail = FALSE;

	deviceContext = GetDC( window );
	GLRenderer& renderer = GLRenderer::GetInstance( );
	renderer.Resize( clientWidth, clientHeight );

	//*****
	int nPixCount = 0;
	int nPixelFormat = -1;
#define NUM_SAMPLES 1

	// Specify the important attributes we care about
	int pixAttribs[] = { WGL_SUPPORT_OPENGL_ARB, 1, // Must support OGL rendering
		WGL_DRAW_TO_WINDOW_ARB, 1, // pf that can run a window
		//WGL_ACCELERATION_ARB,   1, // must be HW accelerated
		WGL_COLOR_BITS_ARB, 24, // 8 bits of each R, G and B
		WGL_DEPTH_BITS_ARB, 16, // 16 bits of depth precision for window
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE, // Double buffered context
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, // MSAA on
		WGL_SAMPLES_ARB, NUM_SAMPLES, // 8x MSAA 
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // pf should be RGBA type
		WGL_STENCIL_BITS_ARB, 0,
		0 }; // NULL termination

	// Ask OpenGL to find the most relevant format matching our attribs
	// Only get one format back.
	fail |=!	wglChoosePixelFormatARB( deviceContext, &pixAttribs[0], NULL, 1, &nPixelFormat, (UINT*) &nPixCount );

	// Check for MSAA
	int attrib[] = { WGL_SAMPLES_ARB };
	int nResults = 0;
	fail |=!	wglGetPixelFormatAttribivARB( deviceContext, nPixelFormat, 0, 1, attrib, &nResults );


	PIXELFORMATDESCRIPTOR pfDesc = GetPixelFormatDescriptor();

	// Got a format, now set it as the current one
	fail |=!	SetPixelFormat( deviceContext, nPixelFormat, &pfDesc );

	GLint attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		0 };

	renderContext = wglCreateContextAttribsARB( deviceContext, 0, attribs );

	fail |=!	wglMakeCurrent( deviceContext, renderContext );
	
	if(fail) ShowErrorMessage(L"OpenGL could not be initialized");
}


bool WinApp::Update( )
{
	if ( GetAsyncKeyState( VK_ESCAPE ) )
		return false;

	return true;
}


LRESULT WinApp::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}
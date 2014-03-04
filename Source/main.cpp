#include "WinApp.h"
#include "GLRenderer.h"

#include <stdlib.h>
#include <ctime>

#pragma comment(lib, "opengl32.lib")

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_PAINT:
		GLRenderer::GetInstance( ).Render( );
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	srand( unsigned int( time( 0 ) ) );



	WinApp::CreateApp( hInstance, (WNDPROC) WndProc );
	GLRenderer& renderer = GLRenderer::GetInstance( );
	renderer.Initialize( );

	MSG msg = { 0 };
	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
			renderer.Render( );
	}

	renderer.Terminate( );
	WinApp::TerminateApp( );

	return 0;
}
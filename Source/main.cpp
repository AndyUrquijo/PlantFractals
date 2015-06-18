#include "WinApp.h"
#include "GLRenderer.h"

#include <stdlib.h>
#include <ctime>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#pragma comment(lib,"GL/glew32.lib")



int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	srand( unsigned int( time( 0 ) ) );

	WinApp::CreateApp( hInstance, (WNDPROC) WinApp::WndProc );
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
			
		bool appOn = WinApp::Update();
		if( !appOn )
			break;
		renderer.Render( );
	}

	renderer.Terminate( );
	WinApp::TerminateApp( );

	return 0;
}
/*		WinApp.h		by Andres Urquijo		*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


//Wrapper for OpenGL Windows Applications
struct WinApp
{	
	static HINSTANCE	application;	//Handle to the instance
	static WNDPROC		appWndProc;		//Pointer to the function processing windows messages
	static HWND			window;			//Handle to the window

	static UINT			clientWidth;
	static UINT			clientHeight;

	static HDC			deviceContext;	//OpenGL Device Context
	static HGLRC		renderContext;	//OpenGL Render Context

	static void CreateApp(HINSTANCE hinst, WNDPROC proc);
	static void TerminateApp();

	static void ShowErrorMessage( PCWSTR message, PCWSTR title = L"Error");
	static HRESULT ShowErrorMessage( HRESULT hr, PCWSTR message, PCWSTR title = L"Error" );


	static void InitializeOpenGL();
	static void InitializeExtensions();
};

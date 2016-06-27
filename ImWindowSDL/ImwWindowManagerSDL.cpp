#include "ImwWindowManagerSDL.h"
#include "ImwPlatformWindowSDL.h"

//#include "imgui_impl_sdl.h"
#include "SDL.h"

using namespace ImWindow;


static bool         g_MousePressed[3] = { false, false, false };

ImwWindowManagerSDL::ImwWindowManagerSDL()
{
	ImwPlatformWindowSDL::InitSDL();
}

ImwWindowManagerSDL::~ImwWindowManagerSDL()
{
	ImwPlatformWindowSDL::ShutdownSDL();
	//ImGui_ImplDX11_Shutdown();
}

void ImwWindowManagerSDL::InternalRun()
{
	PreUpdate();



	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
			if (event.button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
			if (event.button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
		}
		}

	}


	SDL_Delay(16);
	//Sleep(16);
}

ImwPlatformWindow* ImwWindowManagerSDL::CreatePlatformWindow(bool bMain, ImwPlatformWindow* pParent, bool bDragWindow)
{
	IM_ASSERT(m_pCurrentPlatformWindow == NULL);
	ImwPlatformWindowSDL* pWindow = new ImwPlatformWindowSDL(bMain, bDragWindow, CanCreateMultipleWindow());
	if (pWindow->Init(pParent))
	{
		return (ImwPlatformWindow*)pWindow;
	}
	else
	{
		delete pWindow;
		return NULL;
	}
}

ImVec2 ImwWindowManagerSDL::GetCursorPos()
{
	//POINT oPoint;
	//::GetCursorPos(&oPoint);

	int mx, my;
	//Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
	//if (SDL_GetWindowFlags(window) & SDL_WINDOW_MOUSE_FOCUS)
		SDL_GetMouseState(&mx, &my);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
	//else
	//	mx = -1; my = -1;


	return ImVec2((float) mx, (float) my);
}

bool ImwWindowManagerSDL::IsLeftClickDown()
{
	int mx, my;
	Uint32 mouseMask = SDL_GetMouseState(&mx, &my);

	bool isLeftDown = g_MousePressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.


	return isLeftDown;
}

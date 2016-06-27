
#ifndef __IM_WINDOW_MANAGER_SDL_H__
#define __IM_WINDOW_MANAGER_SDL_H__

#include "ImwConfig.h"

#include "ImwWindowManager.h"

namespace ImWindow
{
	class ImwWindowManagerSDL : public ImwWindowManager
	{
	public:
		ImwWindowManagerSDL();
		virtual							~ImwWindowManagerSDL();
	protected:
		virtual bool					CanCreateMultipleWindow() { return false; }
		virtual ImwPlatformWindow*		CreatePlatformWindow(bool bMain, ImwPlatformWindow* pParent, bool bDragWindow);

		virtual void					InternalRun();
		virtual ImVec2					GetCursorPos();
		virtual bool					IsLeftClickDown();
	};
}

#endif //__IM_WINDOW_MANAGER_SDL_H__
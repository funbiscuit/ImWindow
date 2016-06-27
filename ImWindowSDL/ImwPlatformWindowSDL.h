
#ifndef __IM_PLATFORM_WINDOW_SDL_H__
#define __IM_PLATFORM_WINDOW_SDL_H__

#include "ImwConfig.h"
#include "ImwPlatformWindow.h"

#include <map>

#include "imgui_impl_sdl.h"

#include "SDL.h"

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>


namespace ImWindow
{
	class ImwPlatformWindowSDL : ImwPlatformWindow
	{
		friend class ImwWindowManagerSDL;
	public:
		//typedef ImwMap<HWND, ImwPlatformWindowSDL*> InstanceMap;
		typedef ImwMap<SDL_Window*, ImwPlatformWindowSDL*> InstanceMap;
	public:

		ImwPlatformWindowSDL(bool bMain, bool bIsDragWindow, bool bCreateState);
		virtual ~ImwPlatformWindowSDL();

		virtual bool Init(ImwPlatformWindow* pMain);

		virtual const ImVec2& GetPosition() const;
		virtual const ImVec2& GetSize() const;

		virtual void Show();
		virtual void Hide();
		virtual void SetSize(int iWidth, int iHeight);
		virtual void SetPosition(int iX, int iY);
		virtual void SetTitle(const char* pTtile);

		SDL_Window* GetSDLWnd();

	protected:
		virtual void PreUpdate();
		virtual void Paint();
		virtual void Destroy();

		virtual void StartDrag();
		virtual void StopDrag();
		virtual bool IsDraging();

		void OnSDLEvent(SDL_Event* e);
		//LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

		SDL_Window* m_sdlWnd;

		// Our OpenGL context handle
		SDL_GLContext glContext;

		// Static
	public:
		static int GetInstanceCount();

	protected:
		//static void InitWndClassEx();
		//static LRESULT CALLBACK ImwPlatformWindowDX11Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static bool InitSDL();
		static void ShutdownSDL();


		static InstanceMap s_mInstances;
		static bool	s_bClassInitialized;
		//static WNDCLASSEX s_oWndClassEx;
		//static IDXGIFactory* s_pFactory;
		//static ID3D11Device* s_pDevice;
		//static ID3D11DeviceContext* s_pDeviceContext;
		static ImwPlatformWindow* s_pLastHoveredWindow;
		//static INT64 g_Time;
		//static INT64 g_TicksPerSecond;

		ImVec2 m_oPosition;
		ImVec2 m_oSize;

		//IDXGISwapChain* m_pSwapChain;
		//ID3D11RenderTargetView* m_pRenderTargetView;

		bool m_bDrag;
		ImVec2 m_iCursorPosStartDrag;
		ImVec2 m_iWindowPosStartDrag;

		SDL_Cursor* m_hCursorArrow;
		SDL_Cursor* m_hCursorResizeNS;
		SDL_Cursor* m_hCursorResizeWE;

	};
}

#endif // __IM_PLATFORM_WINDOW_SDL_H__
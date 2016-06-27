#include "ImwPlatformWindowSDL.h"

#include "ImwWindowManager.h"

#include <iostream>

#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")

using namespace ImWindow;

ImwPlatformWindowSDL::InstanceMap		ImwPlatformWindowSDL::s_mInstances;
bool									ImwPlatformWindowSDL::s_bClassInitialized = false;
//WNDCLASSEX								ImwPlatformWindowSDL::s_oWndClassEx;

// IDXGIFactory*							ImwPlatformWindowSDL::s_pFactory = NULL;
// ID3D11Device*							ImwPlatformWindowSDL::s_pDevice = NULL;
// ID3D11DeviceContext*					ImwPlatformWindowSDL::s_pDeviceContext = NULL;

ImwPlatformWindow*						ImwPlatformWindowSDL::s_pLastHoveredWindow = NULL;

// INT64									ImwPlatformWindowSDL::g_Time = 0;
// INT64									ImwPlatformWindowSDL::g_TicksPerSecond = 0;


IMGUI_API void							ImGui_ImplSdl_RenderDrawLists(ImDrawData* draw_data);


ImwPlatformWindowSDL::ImwPlatformWindowSDL(bool bMain, bool bIsDragWindow, bool bCreateState)
	: ImwPlatformWindow(bMain, bIsDragWindow, bCreateState)
{
	//m_pSwapChain = NULL;
	//m_pRenderTargetView = NULL;
	m_bDrag = false;
	m_oSize = ImVec2(0, 0);
	m_oPosition = ImVec2(-1, -1);
}

ImwPlatformWindowSDL::~ImwPlatformWindowSDL()
{
	s_mInstances.erase(m_sdlWnd);

	//ImwSafeRelease(m_pDevice);
	//ImwSafeRelease(m_pDeviceContext);

	//ImwSafeRelease(m_pSwapChain);
	//ImwSafeRelease(m_pRenderTargetView);


	// Delete our OpengL context
	SDL_GL_DeleteContext(glContext);

	// Destroy our window
	SDL_DestroyWindow(m_sdlWnd);
	//DestroyWindow(m_sdlWnd);
}

bool ImwPlatformWindowSDL::Init(ImwPlatformWindow* pMain)
{
	//InitWndClassEx();

	//HRESULT hr;

	//DWORD iWindowStyle;
	if (m_bIsDragWindow)
	{
		//iWindowStyle = WS_POPUP;
	}
	else if (pMain != NULL)
	{
		// 		iWindowStyle = WS_POPUP | WS_VISIBLE | WS_THICKFRAME;
		// 		iWindowStyle = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		//iWindowStyle = WS_OVERLAPPEDWINDOW;
	}

	//RECT wr = { 0, 0, 800, 600 };
	//AdjustWindowRect(&wr, iWindowStyle, FALSE);

	m_oSize = ImVec2(800, 600);

	// Create an application window with the following settings:
	m_sdlWnd = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		m_oSize.x,                               // width, in pixels
		m_oSize.y,                               // height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);

	// 	m_sdlWnd = CreateWindowEx(NULL,
	// 		"ImwPlatformWindowSDL",
	// 		"ImwWindow",
	// 		iWindowStyle,
	// 		300,
	// 		300,
	// 		wr.right - wr.left,
	// 		wr.bottom - wr.top,
	// 		(pMain != NULL) ? ((ImwPlatformWindowSDL*)pMain)->GetSDLWnd() : NULL,
	// 		NULL,
	// 		GetModuleHandle(NULL),
	// 		NULL);

	if (m_bIsDragWindow)
	{
		//SetWindowLong(m_sdlWnd, GWL_EXSTYLE, GetWindowLong(m_sdlWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//SetLayeredWindowAttributes(m_sdlWnd, RGB(0, 0, 0), 128, LWA_ALPHA);
	}

	s_mInstances.insert(std::pair<SDL_Window*, ImwPlatformWindowSDL*>(m_sdlWnd, this));


	// Create our OpenGL context and attach it to our window
	glContext = SDL_GL_CreateContext(m_sdlWnd);


	// Init GLEW
	glewExperimental = GL_TRUE;
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}


	//SetOpenGLAttributes();
	//setup gl 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//setup gl 3.1
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Turn on double buffering with a 16bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);



	// This makes our buffer swap synchronized with the monitor's vertical refresh
	//SDL_GL_SetSwapInterval(0);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//if (!Init())
	//	return -1;

	// Clear our buffer with a gray background
	glClearColor(0.9, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);


	SDL_GL_SwapWindow(m_sdlWnd);



	// 	DXGI_SWAP_CHAIN_DESC scd;
	// 	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	// 	scd.BufferCount = 1;
	// 	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 	scd.OutputWindow = m_sdlWnd;
	// 	scd.SampleDesc.Count = 4;
	// 	scd.Windowed = true;

	//hr = s_pFactory->CreateSwapChainForHwnd( )
	//hr = s_pFactory->CreateSwapChain( s_pDevice, &scd, &m_pSwapChain );

	// 	if (FAILED(hr))
	// 	{
	// 		MessageBox(NULL, DXGetErrorDescription(hr), TEXT("D3D11CreateDeviceAndSwapChain"), MB_OK);
	// 		return false;
	// 	}
	// 
	// 	hr = s_pFactory->MakeWindowAssociation(m_sdlWnd, DXGI_MWA_NO_ALT_ENTER );
	// 
	// 	if (FAILED(hr))
	// 	{
	// 		MessageBox(NULL, DXGetErrorDescription(hr), TEXT("s_pFactory->MakeWindowAssociation"), MB_OK);
	// 		return false;
	// 	}

	//Create our BackBuffer
	// 	ID3D11Texture2D* pBackBuffer;
	// 	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	// 	if (FAILED(hr))
	// 	{
	// 		MessageBox(NULL, DXGetErrorDescription(hr), TEXT("m_pSwapChain->GetBuffer"), MB_OK);
	// 		return false;
	// 	}

	//Create our Render Target
	// 	hr = s_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	// 	pBackBuffer->Release();
	// 	if (FAILED(hr))
	// 	{
	// 		MessageBox(NULL, DXGetErrorDescription(hr), TEXT("m_pDevice->CreateRenderTargetView"), MB_OK);
	// 		return false;
	// 	}

	//Set our Render Target
	//s_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	SetState();
	ImGui_ImplSdl_Init(m_sdlWnd);

	// 	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&g_TicksPerSecond)) 
	// 		return false;
	// 	if (!QueryPerformanceCounter((LARGE_INTEGER *)&g_Time))
	// 		return false;

	ImGuiIO& io = ImGui::GetIO();


	RestoreState();

	m_hCursorArrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_hCursorResizeNS = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	m_hCursorResizeWE = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);

	// 	m_hCursorArrow = LoadCursor( NULL, IDC_ARROW );
	// 	m_hCursorResizeNS = LoadCursor( NULL, IDC_SIZENS );
	// 	m_hCursorResizeWE = LoadCursor( NULL, IDC_SIZEWE );

	if (pMain == NULL)
	{
		ImGui_ImplSdl_NewFrame(m_sdlWnd);
	}

	return true;
}

const ImVec2& ImwPlatformWindowSDL::GetPosition() const
{
	return m_oPosition;
}

const ImVec2& ImwPlatformWindowSDL::GetSize() const
{
	return m_oSize;
}

void ImwPlatformWindowSDL::Show()
{
	SDL_ShowWindow(m_sdlWnd);
	//ShowWindow(m_sdlWnd, SW_SHOW);
}

void ImwPlatformWindowSDL::Hide()
{
	std::cout << "is drag: " << m_bIsDragWindow << "\n";
	SDL_HideWindow(m_sdlWnd);
	//ShowWindow(m_sdlWnd, SW_HIDE);
}

void ImwPlatformWindowSDL::SetSize(int iWidth, int iHeight)
{
	SDL_SetWindowSize(m_sdlWnd, iWidth, iHeight);

	// 	RECT oRect;
	// 	oRect.left = 0;
	// 	oRect.top = 0;
	// 	oRect.right = iWidth;
	// 	oRect.bottom = iHeight;
	// 	AdjustWindowRect(&oRect, GetWindowLong(m_sdlWnd, GWL_STYLE), false);
	// 	SetWindowPos(m_sdlWnd, 0, 0, 0, oRect.right - oRect.left, oRect.bottom - oRect.top, SWP_NOMOVE);
}

void ImwPlatformWindowSDL::SetPosition(int iX, int iY)
{
	SDL_SetWindowPosition(m_sdlWnd, iX, iY);

	// 	RECT oRect;
	// 	oRect.left = iX;
	// 	oRect.top = iY;
	// 	oRect.right = iX + m_oSize.x;
	// 	oRect.bottom = iY + m_oSize.y;
	// 	AdjustWindowRect(&oRect, GetWindowLong(m_sdlWnd, GWL_STYLE), false);
	// 	SetWindowPos(m_sdlWnd, 0, oRect.left, oRect.top, 0, 0, SWP_NOSIZE);
}

void ImwPlatformWindowSDL::SetTitle(const char* pTtile)
{
	SDL_SetWindowTitle(m_sdlWnd, pTtile);
	//SetWindowText(m_sdlWnd, pTtile);
}

void ImwPlatformWindowSDL::PreUpdate()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		OnSDLEvent(&e);
	}

	// 	MSG msg;
	// 	int iCount = 0;
	// 	while (PeekMessage(&msg, m_sdlWnd, 0, 0, PM_REMOVE) && iCount < 10) // Max 10 messages
	// 	{
	// 		TranslateMessage(&msg);
	// 		DispatchMessage(&msg);
	// 		//OnMessage(msg.)
	// 		//OnMessage(msg.message, msg.wParam, msg.lParam);
	// 		++iCount;
	// 	}
}

void ImwPlatformWindowSDL::Paint()
{
	if (m_bDrag)
	{
		//GetCursorPos()

		//RECT oRect;
		//GetWindowRect(m_sdlWnd, &oRect);


		int mx, my;
		SDL_GetMouseState(&mx, &my);

		//POINT oCursorPoint;
		//GetCursorPos(&oCursorPoint);

		int iX = m_iWindowPosStartDrag.x + mx - m_iCursorPosStartDrag.x;
		int iY = m_iWindowPosStartDrag.y + my - m_iCursorPosStartDrag.y;
		//SetWindowPos(m_sdlWnd, 0, iX, iY, 0, 0, SWP_NOSIZE);
		SDL_SetWindowPosition(m_sdlWnd, iX, iY);
	}

	//if ( NULL != m_pSwapChain )
	{
		//D3DXCOLOR bgColor(0.4f, 0.4f, 0.4f, 1.0f);


		SDL_GL_MakeCurrent(m_sdlWnd, glContext);;
		//s_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

		//glClearColor(0.4f, 0.8f, 0.4f, 1.0f);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ImwIsSafe(s_pDeviceContext)->ClearRenderTargetView(m_pRenderTargetView, bgColor);



		//Initialize Projection Matrix
		// 		glMatrixMode(GL_PROJECTION);
		// 		glLoadIdentity();
		// 
		// 		//Initialize Modelview Matrix
		// 		glMatrixMode(GL_MODELVIEW);
		// 		glLoadIdentity();


// 		glMatrixMode(GL_PROJECTION);
// 		glPushMatrix();
// 		glLoadIdentity();
// 		glOrtho(0.0f, 500, 500, 0.0f, -1.0f, +1.0f);
// 		glMatrixMode(GL_MODELVIEW);
// 		glPushMatrix();
// 		glLoadIdentity();

		//std::cout << "cleared\n";

		//Render quad




		SetState();

		ImGui::GetIO().DisplaySize = ImVec2(m_oSize.x, m_oSize.y);

		ImGui::NewFrame();

		ImwPlatformWindow::Paint();

		/*
		if (this == s_pLastHoveredWindow)
		{
		switch (ImGui::GetMouseCursor())
		{
		case ImGuiMouseCursor_Arrow:
		SDL_SetCursor(m_hCursorArrow);
		//SetCursor(m_hCursorArrow);
		break;
		case ImGuiMouseCursor_TextInput:         // When hovering over InputText, etc.
		SDL_SetCursor(m_hCursorArrow);
		//SetCursor(m_hCursorArrow);
		break;
		case ImGuiMouseCursor_Move:              // Unused
		SDL_SetCursor(m_hCursorArrow);
		//SetCursor(m_hCursorArrow);
		break;
		case ImGuiMouseCursor_ResizeNS:          // Unused
		SDL_SetCursor(m_hCursorResizeNS);
		//SetCursor(m_hCursorResizeNS);
		break;
		case ImGuiMouseCursor_ResizeEW:          // When hovering over a column
		SDL_SetCursor(m_hCursorResizeWE);
		//SetCursor(m_hCursorResizeWE);
		break;
		case ImGuiMouseCursor_ResizeNESW:        // Unused
		SDL_SetCursor(m_hCursorArrow);
		//SetCursor(m_hCursorArrow);
		break;
		case ImGuiMouseCursor_ResizeNWSE:        // When hovering over the bottom-right corner of a window
		SDL_SetCursor(m_hCursorArrow);
		//SetCursor(m_hCursorArrow);
		break;
		}
		}*/

		RestoreState();

		//Present the backbuffer to the screen
		//ImwIsSafe(m_pSwapChain)->Present(0, 0);
		SDL_GL_SwapWindow(m_sdlWnd);
		//std::cout << "swapped\n";
	}
}

void ImwPlatformWindowSDL::Destroy()
{

}

void ImwPlatformWindowSDL::StartDrag()
{
	m_bDrag = true;
	//RECT oRect;
	//GetWindowRect(m_sdlWnd, &oRect);

	int iX, iY;

	SDL_GetWindowPosition(m_sdlWnd, &iX, &iY);

	m_iWindowPosStartDrag.x = iX;
	m_iWindowPosStartDrag.y = iY;

	//POINT oCursorPoint;
	//GetCursorPos(&oCursorPoint);

	int mx, my;
	SDL_GetMouseState(&mx, &my);

	m_iCursorPosStartDrag.x = mx;
	m_iCursorPosStartDrag.y = my;
}

void ImwPlatformWindowSDL::StopDrag()
{
	m_bDrag = false;
}

bool ImwPlatformWindowSDL::IsDraging()
{
	return m_bDrag;
}


SDL_Window* ImWindow::ImwPlatformWindowSDL::GetSDLWnd()
{
	return m_sdlWnd;
}

void ImwPlatformWindowSDL::OnSDLEvent(SDL_Event* e)
{
	if (IsStateSet())
	{
		//std::string sMsg = GetStringFromMessages(message);
		//ImwWindowManager::GetInstance()->Log("OnMessage %d %s\n", message, sMsg.c_str());
		//return DefWindowProc(m_sdlWnd, message, wParam, lParam);
	}
	SetState();
	ImGuiIO& io = ImGui::GetIO();
	RestoreState();

	switch (e->type)
	{
		//case WM_CLOSE:
	case SDL_QUIT:
		OnClose();
		//return 1;
		break;
		//case WM_ENTERSIZEMOVE:
		//case WM_EXITSIZEMOVE:
	case SDL_WINDOWEVENT:
		//case WM_SIZE:
	{

		switch (e->window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{

			//RECT wr = { 0, 0, LOWORD(lParam), HIWORD(lParam) };
			//AdjustWindowRect(&wr, GetWindowLong(m_hWnd, GWL_STYLE), FALSE);
			//if (NULL != m_pSwapChain)
			//{
			//s_pDeviceContext->OMSetRenderTargets(0, 0, 0);

			// Release all outstanding references to the swap chain's buffers.
			//m_pRenderTargetView->Release();

			//HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			//hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			//hr = s_pFactory->MakeWindowAssociation(m_sdlWnd, DXGI_MWA_NO_ALT_ENTER);

			// Perform error handling here!

			// Get buffer and create a render-target-view.
			//ID3D11Texture2D* pBuffer;
			//hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
			// Perform error handling here!

			//hr = s_pDevice->CreateRenderTargetView(pBuffer, NULL, &m_pRenderTargetView);
			// Perform error handling here!
			//pBuffer->Release();

			//s_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

			m_oSize = ImVec2(e->window.data1, e->window.data2);

			//RECT oRect;
			//GetClientRect(m_sdlWnd, &oRect);
			//m_oSize = ImVec2(oRect.right - oRect.left, oRect.bottom - oRect.top);
			//ClientToScreen(m_sdlWnd, reinterpret_cast<POINT*>(&oRect.left)); // convert top-left
			//ClientToScreen(m_sdlWnd, reinterpret_cast<POINT*>(&oRect.right)); // convert bottom-right

			//m_oPosition = ImVec2(oRect.left, oRect.top);

			//IM_ASSERT(m_oSize.x == LOWORD(lParam));
			//IM_ASSERT(m_oSize.y == HIWORD(lParam));

			IM_ASSERT(m_oSize.x > 0);
			IM_ASSERT(m_oSize.y > 0);
			//m_iWidth = LOWORD(lParam);
			//m_iHeight = HIWORD(lParam);

			// Set up the viewport.

			glViewport(0, 0, m_oSize.x, m_oSize.y);
// 			D3D11_VIEWPORT vp;
// 			vp.Width = m_oSize.x;
// 			vp.Height = m_oSize.y;
// 			vp.MinDepth = 0.0f;
// 			vp.MaxDepth = 1.0f;
// 			vp.TopLeftX = 0;
// 			vp.TopLeftY = 0;
// 			s_pDeviceContext->RSSetViewports(1, &vp);

			if (NULL == ImwWindowManager::GetInstance()->GetCurrentPlatformWindow())
			{
				Paint();
			}
			//}
			//return 0;
		}
		break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			OnLoseFocus();
			break;
		case SDL_WINDOWEVENT_MOVED:
		{
			int iX, iY;
			SDL_GetWindowPosition(m_sdlWnd, &iX, &iY);

			//TODO:
			//for some reason we should use zero values.
			//that's bug for sure, will need to figure that out later
			iX = 0;
			iY = 0;
			m_oPosition = ImVec2(iX, iY);

			std::cout << iX << ":" << iY << "\n";

// 			RECT oRect;
// 			GetClientRect(m_sdlWnd, &oRect);
// 			m_oSize = ImVec2(oRect.right - oRect.left, oRect.bottom - oRect.top);
// 			ClientToScreen(m_sdlWnd, reinterpret_cast<POINT*>(&oRect.left)); // convert top-left
// 			ClientToScreen(m_sdlWnd, reinterpret_cast<POINT*>(&oRect.right)); // convert bottom-right
// 			m_oPosition = ImVec2(oRect.left, oRect.top);
			//AdjustWindowRect(&oRect, GetWindowLong(m_hWnd, GWL_STYLE), FALSE);
			//m_oPosition = ImVec2(oRect.left, oRect.top);
		}
		break;
		}

	}
	//break; // Not a forget
	// 	case WM_GETMINMAXINFO:
	// 		{
	// 			MINMAXINFO* mmi = (MINMAXINFO*)lParam;
	// 			mmi->ptMinTrackSize.x = 100;
	// 			mmi->ptMinTrackSize.y = 100;
	// 			return 0;
	// 		}
// 	case WM_MOVE:
// 	{
// 		RECT oRect;
// 		GetClientRect(m_sdlWnd, &oRect);
// 		m_oSize = ImVec2(oRect.right - oRect.left, oRect.bottom - oRect.top);
// 		ClientToScreen(m_sdlWnd, reinterpret_cast<POINT*>(&oRect.left)); // convert top-left
// 		ClientToScreen(m_sdlWnd, reinterpret_cast<POINT*>(&oRect.right)); // convert bottom-right
// 		m_oPosition = ImVec2(oRect.left, oRect.top);
// 		//AdjustWindowRect(&oRect, GetWindowLong(m_hWnd, GWL_STYLE), FALSE);
// 		//m_oPosition = ImVec2(oRect.left, oRect.top);
// 	}
	break;
// 	case WM_KILLFOCUS:
// 	{
// 		OnLoseFocus();
// 	}
	break;

	//case WM_DESTROY:
	//OutputDebugString("WM_DESTROY\n");
	//PostQuitMessage(0);
	break;
	// 	case WM_ERASEBKGND:
	// 		return 1;
	// 		break;
	// 	case WM_PAINT:
	// 		return 1;
	// 		break;

	case SDL_MOUSEBUTTONDOWN:
	{
		std::cout << "button down: " << (e->button.button == SDL_BUTTON_LEFT) << "\n";
		if (e->button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
		if (e->button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
		if (e->button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = true;
		//return true;
	}
	break;
	case SDL_MOUSEBUTTONUP:
	{
		std::cout << "button up: " << (e->button.button == SDL_BUTTON_LEFT) << "\n";
		if (e->button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = false;
		if (e->button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = false;
		if (e->button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = false;
		//return true;
	}
	break;
	// 	case WM_LBUTTONDOWN:
	// 		io.MouseDown[0] = true;
	// 		return 1;
	// 	case WM_LBUTTONUP:
	// 		io.MouseDown[0] = false;
	// 		return 1;
	// 	case WM_RBUTTONDOWN:
	// 		io.MouseDown[1] = true; 
	// 		return 1;
	// 	case WM_RBUTTONUP:
	// 		io.MouseDown[1] = false; 
	// 		return 1;
	// 	case WM_MBUTTONDOWN:
	// 		io.MouseDown[2] = true; 
	// 		return 1;
	// 	case WM_MBUTTONUP:
	// 		io.MouseDown[2] = false; 
	// 		return 1;
	case SDL_MOUSEWHEEL:
	{
		if (e->wheel.y > 0)
			io.MouseWheel = 1;
		if (e->wheel.y < 0)
			io.MouseWheel = -1;
		//return true;
	}
	break;
	// 	case WM_MOUSEWHEEL:
	// 		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
	// 		return 1;
	case SDL_MOUSEMOTION:
		io.MousePos.x = e->motion.x;
		io.MousePos.y = e->motion.y;
		s_pLastHoveredWindow = this;
		break;
		// 	case WM_MOUSEMOVE:
		// 		io.MousePos.x = (signed short)(lParam);
		// 		io.MousePos.y = (signed short)(lParam >> 16);
		// 		s_pLastHoveredWindow = this;
		// 		return 1;
		// 	case WM_KEYDOWN:
		// 		if (wParam < 256)
		// 			io.KeysDown[wParam] = 1;
		// 		return 1;
		// 	case WM_KEYUP:
		// 		if (wParam < 256)
		// 			io.KeysDown[wParam] = 0;
		// 		return 1;
		// 	case WM_CHAR:
		// 		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		// 		if (wParam > 0 && wParam < 0x10000)
		// 			io.AddInputCharacter((unsigned short)wParam);
		// 		return 1;

	case SDL_TEXTINPUT:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharactersUTF8(e->text.text);
		//return true;
	}
	break;
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	{
		int key = e->key.keysym.sym & ~SDLK_SCANCODE_MASK;
		io.KeysDown[key] = (e->type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		//return true;
	}
	break;
	}

	//return DefWindowProc(m_sdlWnd, message, wParam, lParam);
}


// Static

int ImwPlatformWindowSDL::GetInstanceCount()
{
	return s_mInstances.size();
}

/*
void ImwPlatformWindowSDL::InitWndClassEx()
{
if (!s_bClassInitialized)
{
WNDCLASSEX wc;

ZeroMemory(&wc, sizeof(WNDCLASSEX));

wc.cbSize = sizeof(WNDCLASSEX);
wc.style = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc = ImwPlatformWindowSDLProc;
//wc.hInstance = hInstance;
wc.hInstance = GetModuleHandle(NULL);
wc.hCursor = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
wc.lpszClassName = "ImwPlatformWindowSDL";

RegisterClassEx(&wc);

s_bClassInitialized = true;
}
}
*/

// LRESULT ImwPlatformWindowSDL::ImwPlatformWindowDX11Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
// 	std::map<HWND, ImwPlatformWindowSDL*>::iterator it = s_mInstances.find(hWnd);
// 	if (it != s_mInstances.end())
// 	{
// 		return it->second->OnMessage(message, wParam, lParam);
// 	}
// 	/*else
// 	{
// 		ImAssert(false, "HWND not found in ImwPlatformWindowSDL instances");
// 	}
// 	*/
// 
// 	return DefWindowProc(hWnd, message, wParam, lParam);
// }

bool ImwPlatformWindowSDL::InitSDL()
{

	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//std::cout << "Failed to init SDL\n";
		return false;
	}

	// 
	// 	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&s_pFactory));
	// 
	// 	if (FAILED(hr))
	// 	{
	// 		MessageBox(NULL, DXGetErrorDescription(hr), TEXT("CreateDXGIFactory"), MB_OK);
	// 		return false;
	// 	}
	// 
	// 	hr = D3D11CreateDevice(NULL,
	// 		D3D_DRIVER_TYPE_HARDWARE,
	// 		NULL,
	// 		NULL,
	// 		NULL,
	// 		NULL,
	// 		D3D11_SDK_VERSION,
	// 		&s_pDevice,
	// 		NULL,
	// 		&s_pDeviceContext);
	// 
	// 	if (FAILED(hr))
	// 	{
	// 		MessageBox(NULL, DXGetErrorDescription(hr), TEXT("CreateDXGIFactory"), MB_OK);
	// 		return false;
	// 	}

	return true;
}

void ImwPlatformWindowSDL::ShutdownSDL()
{


	// Shutdown SDL 2
	SDL_Quit();

	// 	ImwSafeRelease(s_pDevice);
	// 	ImwSafeRelease(s_pDeviceContext);
	// 	ImwSafeRelease(s_pFactory);
}
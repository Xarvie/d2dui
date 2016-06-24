#include "GuiWindow.h"
int                     GuiWindow::NumBerOfMainWindow = 0;
MSG                     GuiWindow::msg;
IDWriteFactory*         GuiWindow::DWriteFactory = NULL;
ID2D1Factory*           GuiWindow::Factory = NULL;
GuiWindow**             GuiWindow::Window = NULL;
IWICImagingFactory*		GuiWindow::WICFactory = NULL;
float					GuiWindow::CharWidth[65536];
GuiWindow::~GuiWindow()//释放D2D工厂
{

}


void GuiWindow::WriteText(const WCHAR* _String, float _x, float _y, float _width, float _height, float _Size, const WCHAR* _FontName)
{
	HRESULT hr;
	IDWriteTextFormat* TextFormat = NULL;

	hr = DWriteFactory->CreateTextFormat(
		_FontName,
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)_Size,
		L"",
		&TextFormat
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create IDWriteTextFormat failed!", L"Error", 0);
		return;
	}
	D2D1_RECT_F TextLayoutRect = D2D1::RectF(
		(float)_x,
		(float)_y,
		(float)_x + _width,
		(float)_y + _height
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create IDWriteTextFormat failed!", L"Error", 0);
		return;
	}
	hwndRenderTarget->DrawText(_String, (UINT32)wcslen(_String), TextFormat, TextLayoutRect, BrushWhite);

}
void GuiWindow::Refresh()
{
	GetClientRect(this->hwnd, &this->MainRc);
	D2D_RECT_F rect = D2D1::RectF((float)0, (float)0, (float)MainRc.right, (float)MainRc.bottom);
	ID2D1SolidColorBrush            *BrushBg;
	ID2D1SolidColorBrush            *BrushBorder;
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x1E1E1E, 1.0F), &BrushBg);
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x007ACC, 1.0F), &BrushBorder);
	hwndRenderTarget->FillRectangle(rect, BrushBg);
	if (this->ElementHead->image != NULL)
	{
		hwndRenderTarget->DrawBitmap(this->ElementHead->image, this->ElementHead->rc);
	}
	else
	{

	}
	hwndRenderTarget->DrawRectangle(rect, BrushBorder, 1);


	BrushBg->Release();
	BrushBorder->Release();
}
int GuiWindow::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
{
	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hwnd, SW_SHOW);
		return 0;
	case WM_ERASEBKGND:
		message = 0;
		return 0;
	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		if (x >= this->ElementHead->rc->left && y >= this->ElementHead->rc->top && x <= this->ElementHead->rc->right && y <= this->ElementHead->rc->bottom)
		{
			this->ElementHead->window->ActivatedControlId = this->ElementHead->id;
		}
		return 0;
	}
	case WM_LBUTTONUP:
		return 0;
	case WM_PAINT:
		Refresh();
		return 0;
	case WM_DESTROY:
		return 0;
	case WM_CLOSE:
		return 0;
	case WM_SIZE:
		hwndRenderTarget->Resize(D2D1::SizeU(LOWORD(lparam), HIWORD(lparam)));
		return 0;
	}
	return 0;
}
LRESULT CALLBACK GuiWindow::WndMsgProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	GuiWindow* pthis = NULL;
	for (int i = 0; i < GuiWindow::NumBerOfMainWindow; i++)
	{
		pthis = GuiWindow::Window[i];

		if (hwnd == pthis->hwnd)
		{
			GuiElement* tmp = pthis->ElementHead;
			if (message == WM_PAINT)
			{
				pthis->hwndRenderTarget->BeginDraw();
			}
			if (message == WM_MOUSEMOVE)
			{
				GuiElement* msgTarget = pthis->ElementHead;
				while (tmp != NULL)
				{
					int x = LOWORD(lparam);
					int y = HIWORD(lparam);

					if (x >= tmp->rc->left && y >= tmp->rc->top && x <= tmp->rc->right && y <= tmp->rc->bottom && !tmp->through)
					{
						if (tmp->id > msgTarget->id)
						{
							msgTarget = tmp;
						}

					}
					tmp = tmp->next;
				}
				msgTarget->window->IgniteId = msgTarget->id;
			}
			if (
				
				message == WM_LBUTTONDOWN ||
				message == WM_RBUTTONDOWN ||
				message == WM_MBUTTONDOWN 
				)
			{
				GuiElement* msgTarget = pthis->ElementHead;
				while (tmp != NULL)
				{
					int x = LOWORD(lparam);
					int y = HIWORD(lparam);

					if (x >= tmp->rc->left && y >= tmp->rc->top && x <= tmp->rc->right && y <= tmp->rc->bottom && !tmp->through)
					{
						if (tmp->id > msgTarget->id)
						{
							msgTarget = tmp;
						}

					}
					tmp = tmp->next;
				}
				if (message == WM_LBUTTONDOWN)
				{
					msgTarget->window->ActivatedControlId = msgTarget->id;
					if (msgTarget->vfunc->MouseLBState != StateMouseLBDown)
					{
						msgTarget->vfunc->MouseLBState = StateMouseLBDown;
					}
					SetCapture(msgTarget->window->hwnd);
					SendMessage(msgTarget->window->hwnd, WM_PAINT, 0, 0);
				}
				else if (message == WM_RBUTTONDOWN)
				{
					msgTarget->window->ActivatedControlId = msgTarget->id;
					if (msgTarget->vfunc->MouseRBState != StateMouseRBDown)
					{
						msgTarget->vfunc->MouseRBState = StateMouseRBDown;
					}
					SetCapture(msgTarget->window->hwnd);
					SendMessage(msgTarget->window->hwnd, WM_PAINT, 0, 0);
				}
				else if (message == WM_MBUTTONDOWN)
				{
					msgTarget->window->ActivatedControlId = msgTarget->id;
					if (msgTarget->vfunc->MouseMBState != StateMouseMBDown)
					{
						msgTarget->vfunc->MouseMBState = StateMouseMBDown;
					}
					SetCapture(msgTarget->window->hwnd);
				}

				msgTarget->vfunc->WndProc(hwnd, message, wparam, lparam);
			}
			else
			{
				tmp = pthis->ElementHead;
				while (tmp != NULL)
				{
					/*
					if (message == WM_LBUTTONUP)
					{
						tmp->vfunc->MouseLBState = StateMouseLBUp;
						ReleaseCapture();
						tmp->vfunc->WndProc(hwnd, message, wparam, lparam);
						SendMessage(tmp->window->hwnd, WM_PAINT, 0, 0);//修改高性能
					}
					else if (message == WM_RBUTTONUP)
					{
						tmp->vfunc->MouseRBState = StateMouseRBUp;
						ReleaseCapture();
						tmp->vfunc->WndProc(hwnd, message, wparam, lparam);
						SendMessage(tmp->window->hwnd, WM_PAINT, 0, 0);
					}
					else if (message == WM_MBUTTONUP)
					{
						tmp->vfunc->MouseMBState = StateMouseMBUp;
						ReleaseCapture();
						tmp->vfunc->WndProc(hwnd, message, wparam, lparam);
						SendMessage(tmp->window->hwnd, WM_PAINT, 0, 0);
					}
					else*/ if (tmp->vfunc->WndProc(hwnd, message, wparam, lparam) != 0)
					{
						break;
					}

					tmp = tmp->next;
				}
			}

			if (message == WM_PAINT)
			{
				pthis->hwndRenderTarget->EndDraw();
			}
		}
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}
HINSTANCE GuiWindow::GuiRegisterClass(LPCWSTR _lpszClassName)
{
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wc.lpfnWndProc = WndMsgProc;
	wc.lpszClassName = _lpszClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wc);
	return wc.hInstance;
}

void GuiWindow::DelWindow()//要改:不是每个GuiWindow都是new出来的.
{

}

GuiWindow::GuiWindow()
{

}
int GuiWindow::D2DInit()
{
	srand((int)clock());
	//创建单线程D2D工厂
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &GuiWindow::Factory);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Factory Failed", L"Error:", 0);
		return 0;
	}
	// Create WIC factory
	CoInitialize(NULL);
	hr = CoCreateInstance(
		CLSID_WICImagingFactory1,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		reinterpret_cast<void **>(&GuiWindow::WICFactory)
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create WIC factory Failed", L"Error:", 0);
		return 0;
	}
	//主程序入口点
	GuiMain();
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return 0;
}

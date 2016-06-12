#include "GuiWindow.h"
int                     GuiWindow::NumBerOfMainWindow = 0;
MSG                     GuiWindow::msg;
IDWriteFactory*         GuiWindow::DWriteFactory = NULL;
ID2D1Factory*           GuiWindow::Factory = NULL;
GuiWindow**             GuiWindow::Window = NULL;

GuiWindow::~GuiWindow()//释放D2D工厂
{

}


void GuiWindow::WriteText(const WCHAR * _String, const WCHAR * _FontName, float _Size, int _x, int _y, int _width, int _height)
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
	hwndRenderTarget->BeginDraw();
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x1E1E1E, 1.0F), &BrushBg);
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x007ACC, 1.0F), &BrushBorder);

	hwndRenderTarget->FillRectangle(rect, BrushBg);
	hwndRenderTarget->DrawRectangle(rect, BrushBorder);
	WriteText(L"你好");
	hwndRenderTarget->EndDraw();
	BrushBg->Release();
	BrushBorder->Release();
}
void GuiWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hwnd, SW_SHOW);
		return;
	case WM_ERASEBKGND:
		return;
	case WM_MOUSEMOVE:
		return;
	case WM_LBUTTONDOWN:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		return;
	case WM_LBUTTONUP:
		return;
	case WM_PAINT:
		Refresh();
		return;
	case WM_DESTROY:
		return;
	case WM_SIZE:
		hwndRenderTarget->Resize(D2D1::SizeU(LOWORD(lparam), HIWORD(lparam)));
		return;
	}
}
LRESULT CALLBACK GuiWindow::WndMsgProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message== WM_ERASEBKGND)
		return 0;
	GuiWindow* pthis = NULL;
	for (int i = 0; i < GuiWindow::NumBerOfMainWindow; i++)
	{
		pthis = GuiWindow::Window[i];
		if (hwnd == pthis->hwnd)
		{
			GuiElement* tmp = pthis->ElementHead;
			while (tmp!=NULL)
			{
				tmp->vfunc->WndProc(hwnd, message, wparam, lparam);
				tmp = tmp->next;
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
void GuiWindow::NewWindow(
	GuiWindow* d2d,
	LPCWSTR  _title,
	int _x,
	int _y,
	int _width,
	int _height,
	D2D1_COLOR_F _colorBorder,
	D2D1_COLOR_F _colorBg,
	int layer,
	bool _visible)
{
	HRESULT hr;
	GuiWindow::NumBerOfMainWindow += 1;
	Window = (GuiWindow**)realloc(Window, sizeof(GuiWindow*)*GuiWindow::NumBerOfMainWindow);
	Window[GuiWindow::NumBerOfMainWindow - 1] = this;
	//元素链头初始化
	if (ElementHead==NULL)
	{
		RECT* rc = new RECT;
		rc->left = 0;
		rc->top = 0;
		rc->right = _width;
		rc->bottom = _height;
		ElementHead = new GuiElement;
		ElementHead->child = NULL;
		ElementHead->next = NULL;
		ElementHead->id = 1;
		ElementHead->image = NULL;
		ElementHead->last = NULL;
		ElementHead->next = NULL;
		ElementHead->parent = NULL;
		ElementHead->rc = rc;
		ElementHead->text = _title;
		ElementHead->through = false;
		ElementHead->visible = true;
		ElementHead->vfunc = (GuiBase*)this;
	}
	ElementBack = ElementHead;
	HINSTANCE hInstance = GuiRegisterClass(_title);
	//窗口创建
	//WS_POPUP | WS_MINIMIZEBOX
	hwnd = CreateWindowEx(NULL, _title, _title, WS_OVERLAPPEDWINDOW, _x, _y, _width, _height, NULL, NULL, hInstance, NULL);

	GetClientRect(hwnd, &MainRc);

	hr = GuiWindow::Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(MainRc.right - MainRc.left, MainRc.bottom - MainRc.top)),
		&(hwndRenderTarget));
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Create HwndRenderTarget Failed", L"Error:", 0);
		return;
	}

	// 创建DirectWrite 工厂
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&DWriteFactory)
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DirectWrite factory failed!", L"Error", 0);
		return;
	}
	hr = hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &BrushWhite);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DirectWrite factory failed!", L"Error", 0);
		return;
	}

	hwndRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	ID2D1SolidColorBrush            *BrushBg;
	ID2D1SolidColorBrush            *BrushBorder;
	D2D_RECT_F rect = D2D1::RectF((float)0, (float)0, (float)_width, (float)_height);

	hwndRenderTarget->CreateSolidColorBrush(_colorBg, &BrushBg);
	hwndRenderTarget->CreateSolidColorBrush(_colorBorder, &BrushBorder);
	hwndRenderTarget->BeginDraw();
	hwndRenderTarget->FillRectangle(rect, BrushBg);
	hwndRenderTarget->DrawRectangle(rect, BrushBorder);
	WriteText(_title);
	hwndRenderTarget->EndDraw();
	BrushBg->Release();
	BrushBorder->Release();
	ShowWindow(hwnd, SW_SHOW);
	return;
}
void GuiWindow::DelWindow()
{

}
/*
GuiWindow GuiWindow::NewWindow(GuiWindow* window, char * _title, D2D1_COLOR_F _colorBorder, D2D1_COLOR_F _colorBg, int _top, int _bottom, int _left, int _right, int layer, bool _visible)
{
d2dui* d2d = &window->GetD2d();
RECT rc = d2d->MainRcGet();
ID2D1SolidColorBrush *BrushBg;
ID2D1SolidColorBrush *BrushBorder;
D2D_RECT_F rect = D2D1::RectF((float)_left, (float)_top, (float)_right, (float)_bottom);
d2d->HwndRenderTarget->CreateSolidColorBrush(_colorBg, &BrushBg);
d2d->HwndRenderTarget->CreateSolidColorBrush(_colorBorder, &BrushBorder);
d2d->HwndRenderTarget->BeginDraw();
d2d->HwndRenderTarget->DrawRectangle(rect, BrushBorder);
d2d->HwndRenderTarget->FillRectangle(rect, BrushBg);
d2d->HwndRenderTarget->EndDraw();
BrushBg->Release();
BrushBorder->Release();
return ;
}
*/




GuiWindow::GuiWindow()
{

}
int GuiWindow::D2DInit()
{
	//创建单线程D2D工厂
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &GuiWindow::Factory);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create Factory Failed", L"Error:", 0);
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

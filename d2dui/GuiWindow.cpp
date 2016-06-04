#include "GuiWindow.h"

int                     GuiWindow::NumBerOfMainWindow = 0;
MSG                     GuiWindow::msg;
IDWriteFactory*         GuiWindow::DWriteFactory = NULL;
ID2D1Factory*           GuiWindow::Factory = NULL;
GuiWindow**             GuiWindow::Window = NULL;

GuiWindow::~GuiWindow()//释放D2D工厂
{

}
void GuiWindow::Refresh()
{
	GetClientRect(Hwnd, &MainRc);
	//HwndRenderTarget->Resize(D2D1::SizeU(this->MainRc.right - this->MainRc.left, this->MainRc.bottom - this->MainRc.top));
	HwndRenderTarget->BeginDraw();
	HwndRenderTarget->EndDraw();
	//ShowWindow(this->Hwnd, SW_SHOW);
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
	HwndRenderTarget->BeginDraw();
	HwndRenderTarget->DrawText(_String, wcslen(_String), TextFormat, TextLayoutRect, BrushWhite);
	HwndRenderTarget->EndDraw();
}
LRESULT CALLBACK GuiWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	GuiWindow* pthis = NULL;
	for (int i = 0; i < GuiWindow::NumBerOfMainWindow; i++)
	{
		pthis = GuiWindow::Window[i];
		if (hwnd == pthis->Hwnd)
		{
			switch (message)
			{
			case WM_MOUSEMOVE:
				break;
			case WM_LBUTTONDOWN:
				break;
			case WM_LBUTTONUP:
				break;
			case WM_PAINT:
				pthis->Refresh();
				break;
			case WM_DESTROY:
				break;
			}

		}
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}
HINSTANCE GuiWindow::GuiRegisterClass(LPCWSTR _lpszClassName) {
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
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
	HINSTANCE hInstance = GuiRegisterClass(_title);
	//窗口创建
	//WS_POPUP | WS_MINIMIZEBOX
	Hwnd = CreateWindowEx(NULL, _title, _title, WS_OVERLAPPEDWINDOW, _x, _y, _width, _height, NULL, NULL, hInstance, NULL);

	GetClientRect(Hwnd, &MainRc);

	hr = GuiWindow::Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			Hwnd,
			D2D1::SizeU(MainRc.right - MainRc.left, MainRc.bottom - MainRc.top)),
		&(HwndRenderTarget));
	if (FAILED(hr))
	{
		MessageBox(Hwnd, L"Create HwndRenderTarget Failed", L"Error:", 0);
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
	hr = HwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &BrushWhite);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DirectWrite factory failed!", L"Error", 0);
		return;
	}

	HwndRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	ID2D1SolidColorBrush            *BrushBg;
	ID2D1SolidColorBrush            *BrushBorder;
	D2D_RECT_F rect = D2D1::RectF((float)0, (float)0, (float)_width, (float)_height);

	HwndRenderTarget->CreateSolidColorBrush(_colorBg, &BrushBg);
	HwndRenderTarget->CreateSolidColorBrush(_colorBorder, &BrushBorder);
	HwndRenderTarget->BeginDraw();
	HwndRenderTarget->FillRectangle(rect, BrushBg);
	HwndRenderTarget->DrawRectangle(rect, BrushBorder);
	HwndRenderTarget->EndDraw();
	WriteText(_title);
	BrushBg->Release();
	BrushBorder->Release();


	ShowWindow(Hwnd, SW_SHOW);

	return;
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
GuiLabel GuiLabel::NewLabel(
	char*   _title,
	int     _top,
	int     _bottom,
	int     _left,
	int     _right,
	int     _layer,
	bool    _visible)
{
	return GuiLabel();
}
GuiButton GuiButton::NewButton(
	GuiWindow* _window,
	char*   _title,
	int     _top,
	int     _bottom,
	int     _left,
	int     _right,
	COLORREF _colorBg,
	COLORREF _colortext,
	int     _layer,
	bool    _visible)
{
	/*
	RECT rc = d2d.MainRcGet();
	ID2D1SolidColorBrush *tmpBrush;
	d2d.HwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x11111111, 1.0f), &tmpBrush);
	d2d.HwndRenderTarget->DrawRectangle(D2D1::RectF((float)_left, (float)_top, (float)_right, (float)_bottom), tmpBrush);
	tmpBrush->Release();
	d2d.HwndRenderTarget->Resize(D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));
	d2d.HwndRenderTarget->BeginDraw();
	d2d.HwndRenderTarget->EndDraw();
	*/
	return *this;

}
GuiEdit GuiEdit::NewEdit(
	char*   _title,
	int     _top,
	int     _bottom,
	int     _left,
	int     _right,
	int     _layer,
	bool    _visible)
{
	return GuiEdit();
}


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

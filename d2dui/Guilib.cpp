#include "Guilib.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) { return GuiWindow::D2DInit(); }
GuiWindow* GuiNew::NewWindow(
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
	GuiWindow* control = new GuiWindow;
	HRESULT hr;
	GuiWindow::NumBerOfMainWindow += 1;
	GuiWindow::Window = (GuiWindow**)realloc(GuiWindow::Window, sizeof(GuiWindow*)*GuiWindow::NumBerOfMainWindow);//要改
	GuiWindow::Window[GuiWindow::NumBerOfMainWindow - 1] = control;
	//元素链头初始化
	if (control->ElementHead == NULL)
	{
		RECT* rc = new RECT;
		rc->left = 0;
		rc->top = 0;
		rc->right = _width;
		rc->bottom = _height;
		control->ElementHead = new GuiElement;
		control->ElementHead->child = NULL;
		control->ElementHead->next = NULL;
		control->ElementHead->id = 1;
		control->ElementHead->image = NULL;
		control->ElementHead->last = NULL;
		control->ElementHead->next = NULL;
		control->ElementHead->parent = NULL;
		control->ElementHead->rc = rc;
		control->ElementHead->text = _title;
		control->ElementHead->through = false;
		control->ElementHead->visible = true;
		control->ElementHead->vfunc = (GuiBase*)control;
		control->ElementHead->window = control;
	}
	control->ElementBack = control->ElementHead;
	HINSTANCE hInstance = control->GuiRegisterClass(_title);
	//窗口创建
	//WS_POPUP | WS_MINIMIZEBOX
	control->hwnd = CreateWindowEx(NULL, _title, _title, WS_OVERLAPPEDWINDOW, _x, _y, _width, _height, NULL, NULL, hInstance, NULL);

	GetClientRect(control->hwnd, &control->MainRc);

	hr = GuiWindow::Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			control->hwnd,
			D2D1::SizeU(control->MainRc.right - control->MainRc.left, control->MainRc.bottom - control->MainRc.top)),
		&(control->hwndRenderTarget));
	if (FAILED(hr))
	{
		MessageBox(control->hwnd, L"Create HwndRenderTarget Failed", L"Error:", 0);
		return NULL;
	}

	// 创建DirectWrite 工厂
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&GuiWindow::DWriteFactory)
	);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DirectWrite factory failed!", L"Error", 0);
		return NULL;
	}
	hr = control->hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &control->BrushWhite);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DirectWrite factory failed!", L"Error", 0);
		return NULL;
	}

	control->hwndRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	ID2D1SolidColorBrush            *BrushBg;
	ID2D1SolidColorBrush            *BrushBorder;
	D2D_RECT_F rect = D2D1::RectF((float)0, (float)0, (float)_width, (float)_height);

	control->hwndRenderTarget->CreateSolidColorBrush(_colorBg, &BrushBg);
	control->hwndRenderTarget->CreateSolidColorBrush(_colorBorder, &BrushBorder);
	control->hwndRenderTarget->BeginDraw();
	control->hwndRenderTarget->FillRectangle(rect, BrushBg);
	control->hwndRenderTarget->DrawRectangle(rect, BrushBorder);
	control->WriteText(_title);
	control->hwndRenderTarget->EndDraw();
	BrushBg->Release();
	BrushBorder->Release();
	ShowWindow(control->hwnd, SW_SHOW);
	return control;
}

GuiLabel* GuiNew::NewLabel(GuiWindow * _window, LPCWSTR _title, int _x, int _y, int _width, int _height, D2D1_COLOR_F _colorBorder, D2D1_COLOR_F _colorBg, bool _visible)
{
	GuiLabel* control = new GuiLabel;
	GuiElement *tmp = new GuiElement;
	RECT* rc = new RECT;
	rc->left = _x;
	rc->top = _y;
	rc->right = _x + _width;
	rc->bottom = _y + _height;
	tmp->child = NULL;
	tmp->next = NULL;
	tmp->id = _window->ElementBack->id + 1;
	tmp->image = NULL;
	tmp->last = _window->ElementBack;
	tmp->next = NULL;
	tmp->parent = NULL;
	tmp->rc = rc;
	tmp->text = _title;
	tmp->through = false;
	tmp->visible = true;
	tmp->vfunc = (GuiBase*)control;
	tmp->window = _window;
	control->Element = tmp;
	_window->ElementBack->next = tmp;
	_window->ElementBack = tmp;
	return control;
}

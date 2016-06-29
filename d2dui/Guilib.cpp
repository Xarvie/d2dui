#include "Guilib.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) { return GuiWindow::D2DInit(); }
namespace GuiNew
{
	GuiWindow* NewWindow(
		LPCWSTR  _title,
		float _x,
		float _y,
		float _width,
		float _height,
		LPCWSTR _filePath,
		D2D1_COLOR_F _colorBg,
		D2D1_COLOR_F _colorBorder,
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
			D2D_RECT_F* rc = new D2D_RECT_F;
			rc->left = 0.F;
			rc->top = 0.F;
			rc->right = _width;
			rc->bottom = _height;
			control->ElementHead = new GuiElement;
			control->ElementHead->child = NULL;
			control->ElementHead->next = NULL;
			control->ElementHead->id = 1;
			control->ElementHead->Bd = _colorBorder;
			control->ElementHead->Bg = _colorBg;
			control->ElementHead->image = NULL;
			control->ElementHead->last = NULL;
			control->ElementHead->next = NULL;
			control->ElementHead->parent = NULL;
			control->ElementHead->rc = rc;
			wchar_t* tmpstr = new wchar_t[wcslen(_title) + 1];
			wcscpy_s(tmpstr, wcslen(_title) + 1, _title);
			control->ElementHead->text = tmpstr;
			control->ElementHead->through = false;
			control->ElementHead->visible = true;
			control->ElementHead->vfunc = (GuiBase*)control;
			control->ElementHead->window = control;
		}
		control->ElementBack = control->ElementHead;
		HINSTANCE hInstance = control->GuiRegisterClass(_title);
		//窗口创建
		//WS_POPUP | WS_MINIMIZEBOX
		//WS_OVERLAPPEDWINDOW
		control->hwnd = CreateWindowEx(NULL, _title, _title, WS_POPUP | WS_MINIMIZEBOX, (int)_x, (int)_y, (int)_width, (int)_height, NULL, NULL, hInstance, NULL);


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

		control->hwndRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);//抗锯齿
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
		if (_filePath != NULL)
		{
			GuiFunc::CreateImageFromFile(_filePath, control->WICFactory, control->hwndRenderTarget, &control->ElementHead->image);
		}
		
		ShowWindow(control->hwnd, SW_SHOW);
		return control;
	}

	GuiLabel* NewLabel(GuiWindow * _window, LPCWSTR _title, float _x, float _y, float _width, float _height, D2D1_COLOR_F _colorBg, D2D1_COLOR_F _colorBorder, bool _visible)
	{
		GuiLabel* control = new GuiLabel;
		GuiElement *tmp = new GuiElement;
		D2D_RECT_F* rc = new D2D_RECT_F;
		rc->left = _x;
		rc->top = _y;
		rc->right = _x + _width;
		rc->bottom = _y + _height;
		tmp->child = NULL;
		tmp->next = NULL;
		tmp->id = _window->ElementBack->id + 1;
		tmp->Bd = _colorBorder;
		tmp->Bg = _colorBg;
		tmp->image = NULL;
		tmp->last = _window->ElementBack;
		tmp->next = NULL;
		tmp->parent = NULL;
		tmp->rc = rc;
		wchar_t* tmpstr = new wchar_t[wcslen(_title) + 1];
		wcscpy_s(tmpstr, wcslen(_title) + 1, _title);
		tmp->text = tmpstr;
		tmp->through = false;
		tmp->visible = true;
		tmp->vfunc = (GuiBase*)control;
		tmp->window = _window;
		control->Element = tmp;
		_window->ElementBack->next = tmp;
		_window->ElementBack = tmp;
		return control;
	}
	GuiButton* NewButton(GuiWindow * _window, LPCWSTR _title, float _x, float _y, float _width, float _height, D2D1_COLOR_F _colorBg, D2D1_COLOR_F _colorBorder, bool _visible)
	{
		GuiButton* control = new GuiButton;
		GuiElement *tmp = new GuiElement;
		D2D_RECT_F* rc = new D2D_RECT_F;
		rc->left = _x;
		rc->top = _y;
		rc->right = _x + _width;
		rc->bottom = _y + _height;
		tmp->child = NULL;
		tmp->next = NULL;
		tmp->id = _window->ElementBack->id + 1;
		tmp->Bd = _colorBorder;
		tmp->Bg = _colorBg;
		tmp->image = NULL;
		tmp->last = _window->ElementBack;
		tmp->next = NULL;
		tmp->parent = NULL;
		tmp->rc = rc;
		wchar_t* tmpstr = new wchar_t[wcslen(_title) + 1];
		wcscpy_s(tmpstr, wcslen(_title) + 1, _title);
		tmp->text = tmpstr;
		tmp->through = false;
		tmp->visible = true;
		tmp->vfunc = (GuiBase*)control;
		tmp->window = _window;
		control->Element = tmp;
		_window->ElementBack->next = tmp;
		_window->ElementBack = tmp;
		return control;
	}

	GuiImage* NewImage(GuiWindow * _window, LPCWSTR _title, float _x, float _y, float _width, float _height, LPCWSTR _filePath, bool _visible)
	{
		GuiImage* control = new GuiImage;
		GuiElement *tmp = new GuiElement;
		D2D_RECT_F* rc = new D2D_RECT_F;
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
		wchar_t* tmpstr = new wchar_t[wcslen(_title) + 1];
		wcscpy_s(tmpstr, wcslen(_title) + 1, _title);
		tmp->text = tmpstr;
		tmp->through = false;
		tmp->visible = true;
		tmp->vfunc = (GuiBase*)control;
		tmp->window = _window;
		control->Element = tmp;
		_window->ElementBack->next = tmp;
		_window->ElementBack = tmp;
		GuiFunc::CreateImageFromFile(_filePath, _window->WICFactory, _window->hwndRenderTarget, &control->Element->image);
		GuiFont ft;
		return control;
	}

	GuiTextBox* NewTextBox(GuiWindow * _window, LPCWSTR _title, float _x, float _y, float _width, float _height, D2D1_COLOR_F _colorBg, D2D1_COLOR_F _colorBorder, bool _visible)
	{
		GuiTextBox* control = new GuiTextBox;
		GuiElement *tmp = new GuiElement;
		D2D_RECT_F* rc = new D2D_RECT_F;
		GuiFont *font = new GuiFont;
		rc->left = _x;
		rc->top = _y;
		rc->right = _x + _width;
		rc->bottom = _y + _height;
		tmp->child = NULL;
		tmp->next = NULL;
		tmp->id = _window->ElementBack->id + 1;
		tmp->Bd = _colorBorder;
		tmp->Bg = _colorBg;
		tmp->image = NULL;
		tmp->last = _window->ElementBack;
		tmp->next = NULL;
		tmp->parent = NULL;
		tmp->rc = rc;
		wchar_t* tmpstr = new wchar_t[wcslen(_title) + 1];
		wcscpy_s(tmpstr, wcslen(_title) + 1, _title);
		tmp->text = tmpstr;
		tmp->through = false;
		tmp->visible = true;
		tmp->font = font;
		tmp->vfunc = (GuiBase*)control;
		tmp->window = _window;
		control->Element = tmp;
		_window->ElementBack->next = tmp;
		_window->ElementBack = tmp;
		if (GuiTextBox::CaretTimer == NULL)
		{
			GuiTextBox::CaretTimer=rand()%100001;
			SetTimer(_window->hwnd, GuiTextBox::CaretTimer,600,(TIMERPROC)GuiTextBox::ReDrawCaret);
		}
		return control;
	}
}

namespace GuiFunc
{
	void CreateImageFromFile(LPCWSTR _filePath, IWICImagingFactory* _WICFactory, ID2D1HwndRenderTarget* _hwndRenderTarget, ID2D1Bitmap** _image)
	{
		HRESULT hr;
		IWICBitmapDecoder *Decoder = NULL;
		hr = _WICFactory->CreateDecoderFromFilename(
			_filePath,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&Decoder
		);
		if (FAILED(hr))
		{
			MessageBox(0, L"Create BitmapDecoder failed", L"Error", 0);
		}
		IWICBitmapFrameDecode *Frame = NULL;
		hr = Decoder->GetFrame(0, &Frame);
		if (FAILED(hr))
		{
			MessageBox(0, L"Create BitmapFrameDecode failed", L"Error", 0);
		}
		IWICFormatConverter *Converter = NULL;
		hr = _WICFactory->CreateFormatConverter(&Converter);
		if (FAILED(hr))
		{
			MessageBox(0, L"Create FormatConverter failed", L"Error", 0);
		}
		hr = Converter->Initialize(
			Frame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
		if (FAILED(hr))
		{
			MessageBox(0, L"FormatConverter InitializeFailed", L"Error", 0);
		}

		hr = _hwndRenderTarget->CreateBitmapFromWicBitmap(
			Converter,
			NULL,
			_image
		);
		if (FAILED(hr))
		{
			MessageBox(0, L"Create BitmapFromWicBitmap Failed", L"Error", 0);
		}
		SAFE_RELEASE(Decoder);
		SAFE_RELEASE(Frame);
		SAFE_RELEASE(Converter);
	}
}


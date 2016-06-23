#include "GuiLabel.h"
void GuiLabel::Refresh()
{
	float x, y, r, b;
	x = (float)this->Element->rc->left;
	y = (float)this->Element->rc->top;
	r = (float)this->Element->rc->right;
	b = (float)this->Element->rc->bottom;
	HWND& hwnd = this->Element->window->hwnd;
	ID2D1HwndRenderTarget*& hwndRenderTarget = this->Element->window->hwndRenderTarget;

	D2D_RECT_F rect = D2D1::RectF(x, y, r, b);
	ID2D1SolidColorBrush            *BrushBg;

	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x1E1E1E, 0.0F), &BrushBg);
	hwndRenderTarget->FillRectangle(rect, BrushBg);
	this->Element->window->WriteText(this->Element->text, x + 2, y + 2, r - x, b - y);
	BrushBg->Release();

}
int GuiLabel::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		if (x >= this->Element->rc->left && y >= this->Element->rc->top && x <= this->Element->rc->right && y <= this->Element->rc->bottom)
		{
			this->Element->window->ActivatedControlId = this->Element->id;
		}
		return 0;
	}
	case WM_LBUTTONUP:
		return 0;
	case WM_PAINT:
		//需要判断是否为空
		Refresh();
		return 0;
	case WM_DESTROY:
		return 0;
	case WM_SIZE:
		return 0;
	}
	return 0;
}


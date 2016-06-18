#include "GuiLabel.h"
void GuiLabel::Refresh()
{
	float x, y, r, b;

	x = (float)this->self->rc->left;
	y = (float)this->self->rc->top;
	r = (float)this->self->rc->right;
	b = (float)this->self->rc->bottom;
	HWND& hwnd = this->self->window->hwnd;
	ID2D1HwndRenderTarget*& hwndRenderTarget = this->self->window->hwndRenderTarget;

	D2D_RECT_F rect = D2D1::RectF(x, y, r, b);
	ID2D1SolidColorBrush            *BrushBg;
	ID2D1SolidColorBrush            *BrushBorder;
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x1E1E1E, 1.0F), &BrushBg);
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x007ACC, 1.0F), &BrushBorder);

	hwndRenderTarget->FillRectangle(rect, BrushBg);
	hwndRenderTarget->DrawRectangle(rect, BrushBorder,0.5);
	this->self->window->WriteText(this->self->text, x + 2, y + 2, r - x, b - y);

	BrushBg->Release();
	BrushBorder->Release();
}
int GuiLabel::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONDOWN:
		return 0;
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


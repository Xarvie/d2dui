#include "GuiImage.h"
void draw()
{

}
void GuiImage::Refresh()
{
	float x, y, r, b;
	x = (float)this->Element->rc->left;
	y = (float)this->Element->rc->top;
	r = (float)this->Element->rc->right;
	b = (float)this->Element->rc->bottom;
	HWND& hwnd = this->Element->window->hwnd;
	ID2D1HwndRenderTarget*& hwndRenderTarget = this->Element->window->hwndRenderTarget;

	this->Element->window->hwndRenderTarget->DrawBitmap(
		this->Element->image,
		this->Element->rc
	);
}
int GuiImage::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
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


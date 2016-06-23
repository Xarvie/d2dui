#include "GuiTextBox.h"
unsigned int GuiTextBox::CaretTimer = NULL;
bool GuiTextBox::CaretVisibleState=false;
GuiTextBox* GuiTextBox::ActivatedTextBox=NULL;
D2D_POINT_2F GuiTextBox::DrawCaret(int pos)
{
	D2D_POINT_2F p = { (float)this->Element->rc->left + 1,(float)this->Element->rc->top + 2.0f };
	return p;
}
void GuiTextBox::Refresh()
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
	ID2D1SolidColorBrush            *CaretColor;
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF, 0.3F), &BrushBg);
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x000000, 1.0F), &CaretColor);
	hwndRenderTarget->FillRectangle(rect, BrushBg);
	this->Element->window->WriteText(this->Element->text, x, y, r - x, b - y,16.0f);
	if (CaretVisibleState)
	{
		D2D_POINT_2F p1 = { x + 157,y + 2.0f };
		D2D_POINT_2F p2 = { x + 157,y + 19 + 2.0f };
		hwndRenderTarget->DrawLine(p1, p2, CaretColor);
	}
	BrushBg->Release();

}
void GuiTextBox::ReDrawCaret()
{
	if (ActivatedTextBox != NULL)
	{
		CaretVisibleState = !CaretVisibleState;
		SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);
	}
}
int GuiTextBox::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
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
			GuiTextBox::ActivatedTextBox = this;
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


#include "GuiButton.h"

void GuiButton::Refresh()
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
	ID2D1SolidColorBrush            *BrushBorder;
	int ColorValue;

	if (MousePosState == StateMouseIn)
	{
		if (MouseLBState == StateMouseLBDown)
		{
			ColorValue = 0x007ACC;//点燃+按下
		}
		else
		{
			ColorValue = 0x5A2A2A;
		}

	}
	else if (MousePosState == StateMouseOut)
	{
		if (MouseLBState == StateMouseLBDown)
		{
			ColorValue = 0x5A2A2A;
		}
		else
		{
			ColorValue = 0x1E1E1E;
		}

	}

	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(ColorValue, 0.3F), &BrushBg);//0x1E1E1E
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x007ACC, 1.0F), &BrushBorder);//0x007ACC

	hwndRenderTarget->DrawRectangle(rect, BrushBorder);
	hwndRenderTarget->FillRectangle(rect, BrushBg);

	this->Element->window->WriteText(this->Element->text, x + 2, y + 2, r - x, b - y);

	BrushBg->Release();
	BrushBorder->Release();
}

void GuiButton::MouseIn()
{
	SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);
}
void GuiButton::MouseOut()
{
	SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);
}
void GuiButton::MouseLBDown()
{
	SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);
}
int GuiButton::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
	{
		if (this->Element->id == this->Element->window->IgniteId)
		{
			if (MousePosState != StateMouseIn)
			{
				MousePosState = StateMouseIn;
				MouseIn();
			}
		}
		else
		{
			if (MousePosState != StateMouseOut)
			{
				MousePosState = StateMouseOut;
				MouseOut();
			}
		}
		/*
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		
		if (x >= this->Element->rc->left && y >= this->Element->rc->top && x <= this->Element->rc->right && y <= this->Element->rc->bottom)
		{
			if (MousePosState != StateMouseIn)
			{
				MousePosState = StateMouseIn;
				MouseIn();
			}
		}
		else
		{
			if (MousePosState != StateMouseOut)
			{
				MousePosState = StateMouseOut;
				MouseOut();
			}
		}
		*/
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		this->Element->window->ActivatedControlId = this->Element->id;
		if (MouseLBState != StateMouseLBDown)
		{
			MouseLBState = StateMouseLBDown;
			MouseLBDown();
		}
		SetCapture(this->Element->window->hwnd);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);
		if (x >= this->Element->rc->left && y >= this->Element->rc->top && x <= this->Element->rc->right && y <= this->Element->rc->bottom)
		{
			//按钮内松开
			
			

		}

		MouseLBState = StateMouseLBUp;
		ReleaseCapture();

		SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);//修改高性能
		return 0;
	}
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


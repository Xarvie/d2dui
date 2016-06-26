#include "GuiTextBox.h"
unsigned int	GuiTextBox::CaretTimer = NULL;
bool			GuiTextBox::CaretVisibleState = false;
GuiTextBox*		GuiTextBox::ActivatedTextBox = NULL;

int GuiTextBox::DrawCaret(int pos)
{
	float Leftwidth = 0;
	float lastLeftwidth = 0;
	LPCWSTR str = this->Element->text;
	float BoxX = this->Element->rc->left;
	float BoxY = this->Element->rc->top;
	float x = pos - BoxX;
	int choosePos = 0;

	for (int i = 0; i < wcslen(this->Element->text); i++)
	{
		choosePos = i;
		float charWidth = this->Element->font->CharWidth(str[i]);
		if ((Leftwidth + charWidth*0.6F) >= x)
		{
			lastLeftwidth = Leftwidth;
			Leftwidth += charWidth;
			break;
		}
		lastLeftwidth = Leftwidth;
		Leftwidth += charWidth;
	}

	selectEnd = (float)this->Element->rc->left + lastLeftwidth;
	return choosePos;
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
	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x990000, 1.0F), &CaretColor);
	hwndRenderTarget->FillRectangle(rect, BrushBg);
	this->Element->window->WriteText(this->Element->text, x, y, r - x, b - y, this->Element->font->size);
	if (CaretVisibleState)
	{
		D2D_POINT_2F p1 = { selectEnd,this->Element->rc->top };
		D2D_POINT_2F p2 = { p1.x,p1.y + this->Element->font->CharHeight(' ') };
		hwndRenderTarget->DrawLine(p1, p2, CaretColor);
	}


	ID2D1SolidColorBrush *selectColor;

	hwndRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF, 0.5F), &selectColor);
	hwndRenderTarget->FillRectangle(D2D1::RectF(selectBegin, y, selectEnd, y + this->Element->font->CharHeight(' ')), selectColor);

	BrushBg->Release();
	CaretColor->Release();
	selectColor->Release();
}

void GuiTextBox::ReDrawCaret()
{
	if (ActivatedTextBox != NULL && ActivatedTextBox->Element->window->ActivatedControlId == ActivatedTextBox->Element->id)
	{
		CaretVisibleState = !CaretVisibleState;
		SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);
	}
	else if (CaretVisibleState)
	{
		CaretVisibleState = false;
		SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);
	}
}
void GuiTextBox::MouseIn()
{

}
void GuiTextBox::MouseOut()
{

}
void GuiTextBox::DrawSelectRect(float x1, float x2)
{

}
int GuiTextBox::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);

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
		if (MouseLBState == StateMouseLBDown)
		{
			DrawCaret(x);
			if (selectEnd > clickpos)
			{
				selectBegin = clickpos;
				DrawSelectRect(selectBegin, selectEnd);
				SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			else if (selectEnd < clickpos)
			{
				selectBegin = clickpos;
				DrawSelectRect(selectBegin, selectEnd);
				SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			else if (selectEnd == clickpos)
			{
				selectBegin = selectEnd;
			}
		}
	}
	return 0;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lparam);
		int y = HIWORD(lparam);

		this->Element->window->ActivatedControlId = this->Element->id;
		GuiTextBox::ActivatedTextBox = this;

		DrawCaret(x);
		clickpos = selectEnd;
		selectBegin = selectEnd;
		if (MouseLBState != StateMouseLBDown)
		{
			MouseLBState = StateMouseLBDown;
			//MouseLBDown();
		}
		SetCapture(this->Element->window->hwnd);
		SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);

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


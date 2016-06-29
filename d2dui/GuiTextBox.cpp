#include "GuiTextBox.h"
#include <stdio.h>
unsigned int	GuiTextBox::CaretTimer = NULL;
bool			GuiTextBox::CaretVisibleState = false;
GuiTextBox*		GuiTextBox::ActivatedTextBox = NULL;

void replace(wchar_t** source, wchar_t* cpy, int begin, int len)
{
	wchar_t tmp[1024];
	int i;
	int i2;
	for (i = 0; i < begin; i++)
	{
		tmp[i] = (*source)[i];
	}
	i2 = i;
	for (; i - i2 < wcslen(cpy); i++)
	{
		tmp[i] = cpy[i - begin];
	}
	i2 = i;
	for (i = 0; i < wcslen(*source + begin + len); i++)
	{

		tmp[i + i2] = (*source + begin + len)[i];

	}
	tmp[i + i2] = '\0';
	delete *source;
	*source = new wchar_t[wcslen(tmp) + 1];
	wcscpy_s(*source, wcslen(tmp) + 1, tmp);
	(*source)[wcslen(*source)] = '\0';
}

int GuiTextBox::DrawCaretByPos(int pos)
{
	float Leftwidth = 0;
	float lastLeftwidth = 0;
	LPCWSTR str = this->Element->text;
	float BoxX = this->Element->rc->left;
	float BoxY = this->Element->rc->top;
	float x = pos - BoxX;
	int choosePos = -1;
	int len = (int)wcslen(this->Element->text);
	float charWidth = 0;
	int i;
	for (i = 0; i < len; i++)
	{
		choosePos++;
		charWidth = this->Element->font->CharWidth(str[i]);
		if ((Leftwidth + charWidth*0.5F) >= x)
		{
			lastLeftwidth = Leftwidth;
			Leftwidth += charWidth;
			break;
		}
		lastLeftwidth = Leftwidth;
		Leftwidth += charWidth;
	}
	if (i == len)
	{
		choosePos++;
		lastLeftwidth += charWidth;
	}
	selectEnd = (float)this->Element->rc->left + lastLeftwidth;
	return choosePos;
}
int GuiTextBox::DrawCaretByCount(int count)
{
	float Leftwidth = 0;
	LPCWSTR str = this->Element->text;
	float BoxX = this->Element->rc->left;
	float BoxY = this->Element->rc->top;
	int len = (int)wcslen(this->Element->text);
	float charWidth = 0;
	int i;

	for (i = 0; i < len; i++)
	{
		if (i == count)
		{
			break;
		}
		charWidth = this->Element->font->CharWidth(str[i]);
		Leftwidth += charWidth;
	}
	selectBegin = selectEnd = BoxX + Leftwidth;
	selectBeginCount = i;
	selectEndCount = i;
	return 0;
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
			selectEndCount = DrawCaretByPos(x);
			if (selectEnd > clickpos)
			{
				selectBegin = clickpos;
				selectBeginCount = clickposCount;
				SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			else if (selectEnd < clickpos)
			{
				selectBegin = selectEnd;
				selectEnd = clickpos;
				selectBeginCount = selectEndCount;
				selectEndCount = clickposCount;
				SendMessage(ActivatedTextBox->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			else if (selectEnd == clickpos)
			{
				selectBegin = selectEnd;
				selectBeginCount = selectEndCount;
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
		clickposCount = DrawCaretByPos(x);
		selectBeginCount = clickposCount;
		selectEndCount = clickposCount;
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
	case WM_KEYDOWN:
	{
		if (this->Element->id != this->Element->window->IgniteId)
		{
			return 0;
		}
		HIMC hImc = ImmGetContext(this->Element->window->hwnd);
		COMPOSITIONFORM form;
		form.dwStyle = CFS_RECT;
		POINT pt = { (int)selectBegin ,(int)this->Element->rc->top };
		form.ptCurrentPos = pt;
		RECT rt = { pt.x, 0, pt.x + 300, 100 };
		form.rcArea = rt;
		ImmSetCompositionWindow(hImc, &form);
		return 0;
	}
	case WM_CHAR:
	{
		if (this->Element->id != this->Element->window->IgniteId)
		{
			return 0;
		}
		int len = (selectEndCount - selectBeginCount) + 1;
		wchar_t* tmp = new wchar_t[len];
		wcsncpy_s(tmp, len, this->Element->text + selectBeginCount, len - 1);
		tmp[len - 1] = '\0';

		switch ((WCHAR)wparam)
		{
		case '\b':
			if (selectBeginCount == selectEndCount)
			{
				if (selectBeginCount < 1)
				{
					break;
				}
				replace(&this->Element->text, L"", --selectBeginCount, 1);
				DrawCaretByCount(selectBeginCount);
				SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			else
			{
				replace(&this->Element->text, L"", selectBeginCount, selectEndCount - selectBeginCount);
				DrawCaretByCount(selectBeginCount);
				SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			break;

		default:
			if (selectBeginCount == selectEndCount)
			{
				wchar_t tmp[2] = { (WCHAR)wparam ,0 };
				replace(&this->Element->text, tmp, selectBeginCount++, 0);
				DrawCaretByCount(selectBeginCount);
				SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			else
			{
				wchar_t tmp[2] = { (WCHAR)wparam ,0 };
				replace(&this->Element->text, tmp, selectBeginCount, selectEndCount - selectBeginCount);
				DrawCaretByCount(++selectBeginCount);
				SendMessage(this->Element->window->hwnd, WM_PAINT, 0, 0);
			}
			break;
		}
	}
	case WM_DESTROY:
		return 0;
	case WM_SIZE:
		return 0;
	}
	return 0;
}
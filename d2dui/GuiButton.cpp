#include "GuiButton.h"



GuiButton::GuiButton(GuiWindow * gw)
{
	this->hwnd = gw->hwnd;
}

GuiButton::GuiButton()
{
}



void GuiButton::WndMsgProc(UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		MessageBox(this->hwnd, L"虚函数->按钮消息", 0, 0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		break;
	case WM_SIZE:
		MessageBox(this->hwnd, L"虚函数->按钮消息", 0, 0);
		break;
	}
}

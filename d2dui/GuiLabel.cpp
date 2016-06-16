#include "GuiLabel.h"

int GuiLabel::WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam)
{
	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hwnd, SW_SHOW);
		return 0;
	case WM_ERASEBKGND:
		message = 0;
		return 0;
	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONDOWN:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		return 0;
	case WM_LBUTTONUP:
		return 0;
	case WM_PAINT:
		return 0;
	case WM_DESTROY:
		return 0;
	case WM_SIZE:
		return 0;
	}
}

void GuiLabel::NewLabel(GuiWindow * _window, LPCWSTR _title, int _x, int _y, int _width, int _height, D2D1_COLOR_F _colorBorder, D2D1_COLOR_F _colorBg, bool _visible)
{

}

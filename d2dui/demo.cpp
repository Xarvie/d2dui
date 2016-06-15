#include "Guilib.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance, LPSTR CmdLine, int nCmdShow) { return GuiWindow::D2DInit(); }

void GuiMain()
{
	GuiWindow* window1 = new GuiWindow;
	window1->NewWindow( L"タイトル", 500, 600, 400, 250);
	GuiWindow* window2 = new GuiWindow;
	window2->NewWindow(L"제목");
	GuiWindow* window3 = new GuiWindow;
	window3->NewWindow(L"제목");
	GuiWindow* window4 = new GuiWindow;
	window4->NewWindow(L"제목");
	GuiWindow* window5 = new GuiWindow;
	window5->NewWindow(L"제목");
	GuiWindow* window6 = new GuiWindow;
	window6->NewWindow(L"제목");

}


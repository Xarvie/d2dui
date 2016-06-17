#include "Guilib.h"
int WINAPI WinMain(HINSTANCE , HINSTANCE , LPSTR , int ) { return GuiWindow::D2DInit(); }
void GuiMain()
{
	GuiWindow* window1 = new GuiWindow;
	GuiWindow* window2 = new GuiWindow;
	GuiLabel* label1 = new GuiLabel;
	GuiLabel* label2 = new GuiLabel;
	GuiLabel* label3 = new GuiLabel;
	GuiLabel* label4 = new GuiLabel;
	GuiLabel* label5 = new GuiLabel;

	window1->NewWindow( L"Demo", 500, 600, 400, 250);
	window2->NewWindow(L"제목2");
	label1->NewLabel(window1, L"你好", 20, 10, 44, 24);
	label2->NewLabel(window1, L"Hello", 20, 40, 42, 24);
	label3->NewLabel(window1, L"こんにちは", 20, 70, 80, 24);
	label4->NewLabel(window1, L"안녕하세요", 20, 100, 80, 24);
	label5->NewLabel(window1, L"Здравствыйте", 20, 130, 110, 24);
}
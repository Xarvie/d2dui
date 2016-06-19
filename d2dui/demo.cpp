#include "Guilib.h" 
void GuiMain()
{
	using namespace GuiNew;
	GuiWindow* window1 = NewWindow(L"Demo Form1", 500, 600, 400, 250);
	GuiWindow* window2 = NewWindow(L"Demo Form2");
	GuiLabel* label1 = NewLabel(window1, L"你好", 20, 10, 44, 24);
	GuiLabel* label2 = NewLabel(window1, L"Hello", 20, 40, 42, 24);
	GuiLabel* label3 = NewLabel(window1, L"こんにちは", 20, 70, 80, 24);
	GuiLabel* label4 = NewLabel(window1, L"안녕하세요", 20, 100, 80, 24);

	GuiButton* button1 = NewButton(window1, L"登录", 20, 130, 110, 24);
}
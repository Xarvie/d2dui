#include "Guilib.h" 
void GuiMain()
{
	using namespace GuiNew;
	GuiWindow* window1	= NewWindow(L"Demo Form1"							, 20, 60, 700, 360, L"C:\\backGround.jpg");
	GuiWindow* window2	= NewWindow(L"Demo Form2"							, 900);
	GuiLabel* label1	= NewLabel(window1, L"今天的风儿好喧嚣啊."				, 20,  90 + 0 * 30, 250, 24);
	GuiLabel* label2	= NewLabel(window1, L"这风儿似有略略欲泣."				, 20,  90 + 1 * 30, 250, 24);
	GuiLabel* label3	= NewLabel(window1, L"风儿把不祥的东西吹到镇子里去了."	, 20,  90 + 2 * 30, 250, 24);
	GuiLabel* label4	= NewLabel(window1, L"快走吧,在风停止之前."			, 20,  90 + 3 * 30, 250, 24);
	GuiLabel* label5	= NewLabel(window1, L"……"							, 20,  90 + 4 * 30, 250, 24);
	GuiLabel* label6	= NewLabel(window1, L"喂！不得了了！那边的薯片半价啊！"	, 20,  90 + 5 * 30, 250, 24);
	GuiButton* button1	= NewButton(window1, L" 按钮A"						, 20,  90 + 6 * 30, 100, 24);
	GuiButton* button2	= NewButton(window1, L"   按钮B"						, 50, 90 + 6 * 30+10, 100, 24);
	GuiImage* image1	= NewImage(window1, L"创建迷你小埋"					, 620, 90 + 7 * 30, 50, 50,L"C:\\小埋.png");
	GuiTextBox*TextBox1 = NewTextBox(window1, L"文文1"		, 20,  90 + 8 * 30, 250, 24);
}
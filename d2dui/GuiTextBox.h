#pragma once
#include "GuiBase.h"
#include "GuiWindow.h"
class GuiTextBox :public GuiBase
{
public:

	/// <summary>
	/// ��Ϣ��Ӧ����
	/// </summary>
	/// <param name="hwnd">���.</param>
	/// <param name="message">��Ϣ.</param>
	/// <param name="wparam">wparam.</param>
	/// <param name="lparam">lparam.</param>
	/// <returns>����1�������������Ϣ</returns>
	int WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam);
	void Refresh();

	D2D_POINT_2F DrawCaret(int pos = 0);
	static void ReDrawCaret();
	static bool CaretVisibleState;
	static GuiTextBox* ActivatedTextBox;
	static unsigned int CaretTimer;
	int CaretPos = 0;
};
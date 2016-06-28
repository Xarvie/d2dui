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

	int DrawCaret(int pos = 0);
	static void ReDrawCaret();
	void MouseIn();
	void MouseOut();
	void DrawSelectRect(float x1, float x2);
	static bool CaretVisibleState;
	static GuiTextBox* ActivatedTextBox;
	static unsigned int CaretTimer;
	int CaretPos = 0;
	float selectBegin = 0;
	float selectEnd = 0;
	float clickpos = 0;
	int selectBeginCount = 0;
	int selectEndCount = 0;
	int clickposCount = 0;
};
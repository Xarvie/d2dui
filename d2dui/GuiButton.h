#pragma once
#include "GuiBase.h"
#include "GuiWindow.h"
class GuiButton :public GuiBase
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
	void MouseIn();
	void MouseOut();
	void MouseLBDown();
};
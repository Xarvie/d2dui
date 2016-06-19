#pragma once
#include "GuiBase.h"
#include "GuiWindow.h"
class GuiButton :public GuiBase
{
public:

	/// <summary>
	/// 消息响应函数
	/// </summary>
	/// <param name="hwnd">句柄.</param>
	/// <param name="message">消息.</param>
	/// <param name="wparam">wparam.</param>
	/// <param name="lparam">lparam.</param>
	/// <returns>返回1跳过处理后续消息</returns>
	int WndProc(HWND &hwnd, UINT &message, WPARAM &wparam, LPARAM &lparam);
	void Refresh();
	void MouseIn();
	void MouseOut();
	void MouseLBDown();
};
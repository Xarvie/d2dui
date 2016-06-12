#pragma once
#include <d2d1.h>
#include <Windows.h>
#include <dwrite.h>
class GuiBase;
struct GuiElement
{
	int id;
	RECT* rc;
	LPCWSTR text;
	int *image;
	bool through;
	bool visible;
	GuiBase* vfunc;
	GuiElement* last;
	GuiElement* next;
	GuiElement* parent;
	GuiElement* child;
};
class GuiBase
{
	
	virtual void WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) = 0;
};

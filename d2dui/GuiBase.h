#pragma once
#include <d2d1.h>
#include <Windows.h>
#include <dwrite.h>
struct GuiElement
{
	int id;
	RECT* rc;
	LPCWSTR text;
	bool through;
	bool visible;
	int image;
	GuiElement* last;
	GuiElement* next;
	GuiElement* parent;
	GuiElement* child;
};

class GuiBase
{
public:
	virtual void WndProc(UINT message, WPARAM wparam, LPARAM lparam)=0;
	GuiElement e;
};

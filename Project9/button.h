#pragma once
#include "Clickable.h"
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>

class Button : public Clickable
{
public:
	Button(int x, int y, int height, int width);
	void draw() override;
};

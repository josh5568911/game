#pragma once
#include "Clickable.h"
#include <graphics.h>		// ����ͼ�ο�ͷ�ļ�
#include <conio.h>

class Button : public Clickable
{
public:
	Button(int x, int y, int height, int width);
	void draw() override;
};

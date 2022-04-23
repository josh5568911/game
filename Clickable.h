#pragma once
class Clickable
{
public:
	int x;
	int y;
	int height;
	int width;
	void(*clickAction)();

	void receiveEvent(int cx, int cy);
	virtual void draw() = 0;
private:
	bool isClickMe(int cx, int cy);
};
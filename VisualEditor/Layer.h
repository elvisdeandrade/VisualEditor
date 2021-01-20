#pragma once
#include <d2d1.h>
#include <string>

class Layer
{
public:
	virtual HRESULT Draw(ID2D1HwndRenderTarget* renderTarget) = 0;

	virtual HRESULT Initialize(ID2D1HwndRenderTarget* renderTarget) = 0;

	virtual void SetBounds(float x, float y, float width, float height) = 0;

	void SetBackColor(D2D1_COLOR_F color);

protected:
	D2D1_RECT_F bounds;
	D2D_POINT_2F centerPoint;
	D2D1_COLOR_F forecolor;
	D2D1_COLOR_F backColor;
	std::string name;

};


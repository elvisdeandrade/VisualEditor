#pragma once
#include "Layer.h"
class RectangleLayer :
    public Layer
{
public:
	RectangleLayer();
	~RectangleLayer();
	HRESULT Draw(ID2D1HwndRenderTarget* renderTarget);
	HRESULT Initialize(ID2D1HwndRenderTarget* renderTarget);
	void SetBounds(float x, float y, float width, float height);
private:
	ID2D1SolidColorBrush* brush;
};


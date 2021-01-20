#include "RectangleLayer.h"

RectangleLayer::RectangleLayer()
	:brush(NULL)
{
}

RectangleLayer::~RectangleLayer()
{
	if (brush) {
		brush->Release();
	}
}

HRESULT RectangleLayer::Draw(ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr = S_OK;

	renderTarget->FillRectangle(&bounds, brush);

	return  hr;
}

HRESULT RectangleLayer::Initialize(ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr = S_OK;

	hr = renderTarget->CreateSolidColorBrush(
		backColor,//D2D1::ColorF(D2D1::ColorF::ColorF(1.0f,0,0,.8f)),
		&brush
	);

	return  hr;
}

void RectangleLayer::SetBounds(float x, float y, float width, float height)
{
	bounds = D2D1::RectF(x, y, x + width, y + height);
}


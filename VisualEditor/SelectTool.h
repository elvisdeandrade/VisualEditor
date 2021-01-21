#pragma once
#include <memory>
#include "SelectToolNode.h"
#include <vector>

using namespace std;

class SelectTool
{
public:
	SelectTool();
	~SelectTool();

	HRESULT Draw(ID2D1HwndRenderTarget* renderTarget);
	HRESULT Initialize(ID2D1HwndRenderTarget* renderTarget);

	void SetBounds(D2D_POINT_2F start, D2D_POINT_2F end);
private:
	vector<shared_ptr<SelectToolNode>> nodes;

	ID2D1SolidColorBrush* brush;

	float x;
	float y;
	float width;
	float height;

	int selectedNodeIndex;
};


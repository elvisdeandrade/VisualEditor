#pragma once
#include <d2d1.h>

enum class NodeType {
	ALL				= 0,
	TOP_LEFT_1		= 1,
	TOP_RIGHT_1		= 2,
	BOTTOM_LEFT_1	= 3,
	BOTTOM_RIGHT_1	= 4,
	TOP_LEFT_2		= 21,
	TOP_RIGHT_2		= 22,
	BOTTOM_LEFT_2	= 23,
	BOTTOM_RIGHT_2	= 24,
	MIDDLE_LEFT		= 5,
	MIDDLE_RIGHT	= 6,
	CENTER_TOP		= 27,
	CENTER_BOTTOM	= 28
};

class SelectToolNode
{
public:
	SelectToolNode(NodeType type);

	~SelectToolNode();

	HRESULT Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush);

	void Update(D2D_POINT_2F start, D2D_POINT_2F end);
private:
	int width;
	int height;
	int x;
	int y;
	NodeType type;
};


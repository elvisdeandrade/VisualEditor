#include "SelectToolNode.h"

SelectToolNode::SelectToolNode(NodeType node)
{
	type = node;

	if (((int)type) > 20) {
		width = 15;
		height = 5;
	}
	else
	{
		width = 5;
		height = 15;
	}

	x = 0;
	y = 0;
}

SelectToolNode::~SelectToolNode()
{
}

HRESULT SelectToolNode::Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush)
{
	HRESULT hr = S_OK;
	D2D1_RECT_F bounds = D2D1::RectF(x, y, x + width, y + height);

	if (type != NodeType::ALL) {
		renderTarget->FillRectangle(&bounds, brush);
	}

	return  hr;
}

void SelectToolNode::Update(D2D_POINT_2F start, D2D_POINT_2F end)
{
	switch (type) {
	case NodeType::TOP_LEFT_1:
		x = start.x - 2;
		y = start.y - 2;
		break;
	case NodeType::TOP_LEFT_2:
		x = start.x - 2;
		y = start.y - 2;
		break;
	case NodeType::TOP_RIGHT_1:
		x = end.x - 2;
		y = start.y - 2;
		break;
	case NodeType::TOP_RIGHT_2:
		x = end.x - width + 2;
		y = start.y - 2;
		break;
	case NodeType::BOTTOM_LEFT_1:
		x = start.x - 2;
		y = end.y - height + 2;
		break;
	case NodeType::BOTTOM_LEFT_2:
		x = start.x - 2;
		y = end.y - 2;
		break;
	case NodeType::BOTTOM_RIGHT_1:
		x = end.x - 2;
		y = end.y - height + 2;
		break;
	case NodeType::BOTTOM_RIGHT_2:
		x = end.x - width + 2;
		y = end.y - 2;
		break;
	case NodeType::MIDDLE_LEFT:
		x = start.x - 2;
		y = start.y + ((end.y - start.y) / 2) - (height / 2);
		break;
	case NodeType::MIDDLE_RIGHT:
		x = end.x - 2;
		y = start.y + ((end.y - start.y) / 2) - (height / 2);
		break;
	case NodeType::CENTER_TOP:
		x = start.x + ((end.x - start.x) / 2) - (width / 2);
		y = start.y - 2;
		break;
	case NodeType::CENTER_BOTTOM:
		x = start.x + ((end.x - start.x) / 2) - (width / 2);
		y = end.y - 2;
		break;
	default:
		x = start.x;
		y = start.y;
		width = end.x - start.x;
		height = end.y - start.y;
		break;
	}
}

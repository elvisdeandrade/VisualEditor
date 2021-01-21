#include "SelectTool.h"
#include <memory>

using namespace std;

SelectTool::SelectTool()
{
	SelectToolNode* node_all			= new SelectToolNode(NodeType::ALL);
	SelectToolNode* node_top_left_1		= new SelectToolNode(NodeType::TOP_LEFT_1);
	SelectToolNode* node_top_right_1	= new SelectToolNode(NodeType::TOP_RIGHT_1);
	SelectToolNode* node_bottom_left_1	= new SelectToolNode(NodeType::BOTTOM_LEFT_1);
	SelectToolNode* node_bottom_right_1 = new SelectToolNode(NodeType::BOTTOM_RIGHT_1);
	SelectToolNode* node_top_left_2		= new SelectToolNode(NodeType::TOP_LEFT_2);
	SelectToolNode* node_top_right_2	= new SelectToolNode(NodeType::TOP_RIGHT_2);
	SelectToolNode* node_bottom_left_2	= new SelectToolNode(NodeType::BOTTOM_LEFT_2);
	SelectToolNode* node_bottom_right_2	= new SelectToolNode(NodeType::BOTTOM_RIGHT_2);
	SelectToolNode* node_middle_left	= new SelectToolNode(NodeType::MIDDLE_LEFT);
	SelectToolNode* node_middle_right	= new SelectToolNode(NodeType::MIDDLE_RIGHT);
	SelectToolNode* node_center_top		= new SelectToolNode(NodeType::CENTER_TOP);
	SelectToolNode* node_center_bottom	= new SelectToolNode(NodeType::CENTER_BOTTOM);

	nodes.push_back(shared_ptr<SelectToolNode>(node_all));
	nodes.push_back(shared_ptr<SelectToolNode>(node_top_left_1));
	nodes.push_back(shared_ptr<SelectToolNode>(node_top_right_1));
	nodes.push_back(shared_ptr<SelectToolNode>(node_bottom_left_1));
	nodes.push_back(shared_ptr<SelectToolNode>(node_bottom_right_1));
	nodes.push_back(shared_ptr<SelectToolNode>(node_top_left_2));
	nodes.push_back(shared_ptr<SelectToolNode>(node_top_right_2));
	nodes.push_back(shared_ptr<SelectToolNode>(node_bottom_left_2));
	nodes.push_back(shared_ptr<SelectToolNode>(node_bottom_right_2));
	nodes.push_back(shared_ptr<SelectToolNode>(node_middle_left));
	nodes.push_back(shared_ptr<SelectToolNode>(node_middle_right));
	nodes.push_back(shared_ptr<SelectToolNode>(node_center_top));
	nodes.push_back(shared_ptr<SelectToolNode>(node_center_bottom));
	
}

SelectTool::~SelectTool()
{
}

HRESULT SelectTool::Draw(ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr = S_OK;

	D2D1_RECT_F bounds = D2D1::RectF(x, y, x + width, y + height);

	renderTarget->DrawRectangle(&bounds, brush);

	for (auto& n : nodes)
	{
		n.get()->Draw(renderTarget, brush);
	}

	return  hr;
}

HRESULT SelectTool::Initialize(ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr = S_OK;

	hr = renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&brush
	);

	return  hr;
}

void SelectTool::SetBounds(D2D_POINT_2F start, D2D_POINT_2F end)
{
	x = start.x;
	y = start.y;
	width = end.x - start.x;
	height = end.y - start.y;

	for (auto& n : nodes)
	{
		n.get()->Update(start, end);
	}
}

#include "VisualEditor.h"
#include <windowsx.h>
#include <direct.h>
#include "RectangleLayer.h"
#include "SelectToolNode.h"

int WINAPI WinMain(
	_In_ HINSTANCE,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int
)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			VisualEditor editor;

			if (SUCCEEDED(editor.Initialize()))
			{
				editor.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

VisualEditor::VisualEditor() {
	m_hwnd = nullptr;
	m_pDirect2DFactory = nullptr;
	m_pRenderTarget = nullptr;

	RectangleLayer* rec = new RectangleLayer();
	rec->SetBounds(50, 50, 200, 300);
	rec->SetBackColor(HexToColor("ffc0cbFF"));

	layers.push_back(std::shared_ptr<Layer>(rec));

	rec = new RectangleLayer();
	rec->SetBounds(150, 150, 200, 300);
	rec->SetBackColor(HexToColor("C8FDC5FF"));

	layers.push_back(std::shared_ptr<Layer>(rec));

	backColor = HexToColor("e73895ff");

	D2D_POINT_2F start;
	D2D_POINT_2F end;

	start.x = 10;
	start.y = 10;
	end.x = 110;
	end.y = 110;

	selectTool = new SelectTool();
	
	selectTool->SetBounds(start, end);
}

VisualEditor::~VisualEditor() {
	SafeRelease(&m_pDirect2DFactory);
	SafeRelease(&m_pRenderTarget);

	layers.clear();
	layers.shrink_to_fit();
}

HRESULT VisualEditor::Initialize() {
	HRESULT hr;

	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		WNDCLASSEX wcx = { sizeof(WNDCLASSEX) };

		wcx.style = CS_HREDRAW | CS_VREDRAW;
		wcx.lpfnWndProc = VisualEditor::WndProc;
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = sizeof(LONG_PTR);
		wcx.hInstance = HINST_THISCOMPONENT;
		wcx.hbrBackground = NULL;
		wcx.lpszMenuName = NULL;
		wcx.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcx.lpszClassName = L"vsEditor";

		RegisterClassEx(&wcx);

		m_hwnd = CreateWindow(
			L"vsEditor",
			L"Visual Editor",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			1280,
			720,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);

		hr = m_hwnd ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}

void VisualEditor::RunMessageLoop() {
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK VisualEditor::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		VisualEditor* vsEditor = (VisualEditor*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(vsEditor)
		);

		result = 1;
	}
	else
	{
		VisualEditor* vsEditor = reinterpret_cast<VisualEditor*>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (vsEditor)
		{
			switch (message)
			{
				case WM_DESTROY:
					{
						PostQuitMessage(0);
					}
					result = 1;
					wasHandled = true;
					break;
				case WM_PAINT:
					{
						vsEditor->OnRender(0, 0);
						ValidateRect(hwnd, NULL);
					}
					result = 0;
					wasHandled = true;
					break;
				case WM_SIZE:
					{
						UINT width = LOWORD(lParam);
						UINT height = HIWORD(lParam);
						vsEditor->OnResize(width, height);
					}
					result = 0;
					wasHandled = true;
					break;
				case WM_DISPLAYCHANGE:
					{
						InvalidateRect(hwnd, NULL, FALSE);
					}
					result = 0;
					wasHandled = true;
					break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

HRESULT VisualEditor::CreateDeviceIndependentResources() {
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2DFactory);

	return hr;
}

HRESULT VisualEditor::OnRender(float x, float y)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->Clear(backColor);

		for (auto& n : layers)
		{
			n.get()->Draw(m_pRenderTarget);
		}

		selectTool->Draw(m_pRenderTarget);

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

HRESULT VisualEditor::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		hr = m_pDirect2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		for (auto& n : layers)
		{
			n.get()->Initialize(m_pRenderTarget);
		}

		selectTool->Initialize(m_pRenderTarget);
	}

	return hr;
}

void VisualEditor::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
}

void VisualEditor::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

D2D1_COLOR_F VisualEditor::HexToColor(std::string hexColor)
{
	int red = 0;
	int green = 0;
	int blue = 0;
	int alpha = 0;

	red = std::stoi(hexColor.substr(0, 2), 0, 16);
	green = std::stoi(hexColor.substr(2, 2), 0, 16);
	blue = std::stoi(hexColor.substr(4, 2), 0, 16);
	alpha = std::stoi(hexColor.substr(6, 2), 0, 16);

	return D2D1::ColorF::ColorF(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

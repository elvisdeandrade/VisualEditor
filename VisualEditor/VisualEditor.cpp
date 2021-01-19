#include "VisualEditor.h"
#include <windowsx.h>
#include <direct.h>

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
}

VisualEditor::~VisualEditor() {

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
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::YellowGreen));


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
	}

	return hr;
}

void VisualEditor::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
}

void VisualEditor::OnResize(UINT width, UINT height)
{
}

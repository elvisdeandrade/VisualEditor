#include "VisualEditor.h"
#include <windowsx.h>

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
}

VisualEditor::~VisualEditor() {

}

HRESULT VisualEditor::Initialize() {
	HRESULT hr;

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
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

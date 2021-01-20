#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <vector>
#include <memory>
#include "Layer.h"

using namespace std;

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

template<class Interface> inline void SafeRelease(Interface * *ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

class VisualEditor
{
public:
	VisualEditor();
	~VisualEditor();
	HRESULT Initialize();

	void RunMessageLoop();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND m_hwnd;

	vector<shared_ptr<Layer>> layers;

	ID2D1Factory* m_pDirect2DFactory;

	ID2D1HwndRenderTarget* m_pRenderTarget;

	HRESULT CreateDeviceIndependentResources();

	HRESULT OnRender(float x, float y);

	HRESULT CreateDeviceResources();

	void DiscardDeviceResources();

	void OnResize(UINT width, UINT height);

	D2D1_COLOR_F HexToColor(std::string hexColor);

	D2D1_COLOR_F backColor;
};


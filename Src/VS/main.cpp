#if defined BUILD_WITH_NUKLEAR
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>


#include <d3d11.h>
extern "C" {
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#pragma comment(lib,"DXGI.lib")
#pragma comment(lib,"D3D11.lib")
#pragma comment(lib,"dxguid.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_INDEX_BUFFER 128 * 1024



#include <MyStdTypes.h>
#include <MyFramework_Selection.h>

#define NK_INCLUDE_FIXED_TYPES
//#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include <nuklear.h>
#include "nuk_controls.h"
#define NK_D3D11_IMPLEMENTATION
#include <nuklear_d3d11.h>

//#include <MyForm.h> //->MyFramework_Selection
#include "NukForm.h" //->TProcess, ->nuklear
#include "MyStream.h"

/* ===============================================================
 *
 *                          DEMO
 * (nuklear/demo/d3d11/)
 * ===============================================================*/
static IDXGISwapChain *swap_chain;
static ID3D11Device *device;
static ID3D11DeviceContext *context;
static ID3D11RenderTargetView* rt_view;

static void
set_swap_chain_size(int width, int height)
{
	ID3D11Texture2D *back_buffer;
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	HRESULT hr;

	if (rt_view)
		rt_view->Release();

	context->OMSetRenderTargets( 0, NULL, NULL);

	hr = swap_chain->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET || hr == DXGI_ERROR_DRIVER_INTERNAL_ERROR)
	{
		/* to recover from this, you'll need to recreate device and all the resources */
		MessageBoxW(NULL, L"DXGI device is removed or reset!", L"Error", 0);
		exit(0);
	}
	assert(SUCCEEDED(hr));

	memset(&desc, 0, sizeof(desc));
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = swap_chain->GetBuffer( 0, IID_ID3D11Texture2D, (void **)&back_buffer);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView( (ID3D11Resource *)back_buffer, &desc, &rt_view);
	assert(SUCCEEDED(hr));

	back_buffer->Release();
}

static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (swap_chain)
		{
			int width = LOWORD(lparam);
			int height = HIWORD(lparam);
			set_swap_chain_size(width, height);
			nk_d3d11_resize(context, width, height);
		}
		break;
	}

	if (nk_d3d11_handle_event(wnd, msg, wparam, lparam))
		return 0;

	return DefWindowProcW(wnd, msg, wparam, lparam);
}
#ifdef NDEBUG
int WINAPI _tWinMain( _In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ int)
#else
int main(int argc, char** argv)
#endif
{

	struct nk_context *ctx;
	struct nk_colorf bg;

	WNDCLASSW wc;
	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD exstyle = WS_EX_APPWINDOW;
	HWND wnd;
	int running = 1;
	HRESULT hr;
	D3D_FEATURE_LEVEL feature_level;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;

	/* Win32 */
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandleW(0);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"NuklearWindowClass";
	RegisterClassW(&wc);

	AdjustWindowRectEx(&rect, style, FALSE, exstyle);

	wnd = CreateWindowExW(exstyle, wc.lpszClassName, L"Nuklear Demo",
		style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, wc.hInstance, NULL);

	/* D3D11 setup */
	memset(&swap_chain_desc, 0, sizeof(swap_chain_desc));
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = wnd;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		&swap_chain, &device, &feature_level, &context)))
	{
		/* if hardware device fails, then try WARP high-performance
		   software rasterizer, this is useful for RDP sessions */
		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP,
			NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swap_chain_desc,
			&swap_chain, &device, &feature_level, &context);
		assert(SUCCEEDED(hr));
	}
	set_swap_chain_size(WINDOW_WIDTH, WINDOW_HEIGHT);

	/* GUI */
	ctx = nk_d3d11_init(device, WINDOW_WIDTH, WINDOW_HEIGHT, MAX_VERTEX_BUFFER, MAX_INDEX_BUFFER);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	/* Load Cursor: if you uncomment cursor loading please hide the cursor */
	{
		struct nk_font_atlas *atlas;
		nk_d3d11_font_stash_begin(&atlas);
		//  ->Get Windows default Application-Font?
		/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../extra_font/DroidSans.ttf", 14, 0);*/
		nk_d3d11_font_stash_end();
		/*nk_style_load_all_cursors(ctx, atlas->cursors);*/
		/*nk_style_set_font(ctx, &droid->handle)*/;
	}

	
	bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
	while (running)
	{
		/* Input */
		MSG msg;
		nk_input_begin(ctx);
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = 0;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		nk_input_end(ctx);

		/* GUI */
		if (nk_begin(ctx, "Demo", nk_rectf(0, 0, d3d11.viewport.Width*0.8f, d3d11.viewport.Height*0.8f),
			NK_WINDOW_BORDER | NK_WINDOW_TITLE
			//| NK_WINDOW_SCALABLE | NK_WINDOW_MOVABLE //<- updates bounds.. fullscreenwindow
			| NK_WINDOW_MINIMIZABLE
			 ))
		{
			
			enum { EASY, HARD };
			static int op = EASY;
			static int property = 20;

			nk_layout_row_static(ctx, 30, 80, 1);
			if (nk_button_label(ctx, "button"))
				fprintf(stdout, "button pressed\n");
			nk_layout_row_dynamic(ctx, 30, 2);
			if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
			if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
			nk_layout_row_dynamic(ctx, 22, 1);
			nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

			nk_layout_row_dynamic(ctx, 20, 1);
			nk_label(ctx, "background:", NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 25, 1);
			if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
				nk_layout_row_dynamic(ctx, 120, 1);
				bg = nk_color_picker(ctx, bg, NK_RGBA);
				nk_layout_row_dynamic(ctx, 25, 1);
				bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
				bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
				bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
				bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
				nk_combo_end(ctx);
			}
		}
		nk_end(ctx);

		//TStatusBar
		if (nk_begin(ctx, "StatusBar", nk_rectf(0, d3d11.viewport.Height - 30, d3d11.viewport.Width, 30),
			NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR
			))
		{
			nk_layout_row_dynamic(ctx, ctx->current->bounds.h, 1);
			nk_label(ctx, "statustext", NK_TEXT_CENTERED);
		}
		nk_end(ctx);
		/* Draw */
		context->ClearRenderTargetView( rt_view, &bg.r);
		context->OMSetRenderTargets( 1, &rt_view, NULL);
		nk_d3d11_render(context, NK_ANTI_ALIASING_ON);
		hr = swap_chain->Present( 1, 0 );
		if (hr == DXGI_ERROR_DEVICE_RESET || hr == DXGI_ERROR_DEVICE_REMOVED) {
			/* to recover from this, you'll need to recreate device and all the resources */
			MessageBoxW(NULL, L"D3D11 device is lost or removed!", L"Error", 0);
			break;
		}
		else if (hr == DXGI_STATUS_OCCLUDED) {
			/* window is not visible, so vsync won't work. Let's sleep a bit to reduce CPU usage */
			Sleep(50);
		}
		assert(SUCCEEDED(hr));
	}

	context->ClearState();
	nk_d3d11_shutdown();
	rt_view->Release();
	context->Release();
	device->Release();
	swap_chain->Release();
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
	return 0;
}

#endif
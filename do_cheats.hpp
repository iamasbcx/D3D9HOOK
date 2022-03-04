#pragma once

#include "help_func.hpp"

#include <stdio.h>
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "MinHook.h"
#include <process.h>
#include "vehHook.h"
#include <string>
#define  绿色 -16711936

#ifdef _WIN64
#define  调用约定 __fastcall
#else
#define  调用约定 __stdcall
#endif

static ImDrawList* drawList = NULL;

WNDPROC g_original_proc = nullptr;
HWND g_hwnd = 0;
UINT g_count = 0;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT  (调用约定 *pfn_org_Reset)(IDirect3DDevice9* direct3ddevice9, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT (调用约定 *pfn_org_EndScene)(IDirect3DDevice9* direct3ddevice9);
typedef HRESULT (调用约定 *pfn_org_DrawIndexedPrimitive)(IDirect3DDevice9* direct3ddevice9, D3DPRIMITIVETYPE type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
pfn_org_Reset org_Reset = NULL;
pfn_org_EndScene org_EndScene = NULL;
pfn_org_DrawIndexedPrimitive org_DrawIndexedPrimitive = NULL;
void initialize_imgui(HWND hwnd, IDirect3DDevice9* direct3ddevice9);
void draw_line(float X1, float Y1, float X2, float Y2, float Width, ImColor Color);
void draw_rect(float X, float Y, float W, float H, float Width, ImColor Color);
void draw_text(float X, float Y, float fontSize, std::string Str, ImColor Color);
void draw_circle(float X, float Y, float radius/*半径*/, ImColor Color);
void draw_spot(float X, float Y, float radius/*半径*/, ImColor Color);
bool key_status(ULONG key);




unsigned int __stdcall doSomeThing(void* data)
{
	VEH_Uninitialize();
	return 0;
}

LRESULT CALLBACK self_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	////处理 Imgui界面窗口信息  
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_UP) {
			g_count += 100;

			OutputDebugPrintf("hzw:%d\n", g_count);
		}
		if (wParam == VK_DOWN) {
			g_count -= 100;

			OutputDebugPrintf("hzw:%d\n", g_count);
		}
		if (wParam == VK_LEFT) {
			g_count--;

			OutputDebugPrintf("hzw:%d\n", g_count);
		}
		if (wParam == VK_RIGHT) {
			g_count++;

			OutputDebugPrintf("hzw:%d\n", g_count);
		}
		break;
	}
	case WM_CLOSE:
	{
		//解决 关闭窗口的时候  弹出 The Direct3D device has a non-zero refernce count 的问题
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		break;
	}
	}


	return CallWindowProcW(g_original_proc, hWnd, uMsg, wParam, lParam);
}



HRESULT 调用约定 self_Reset(IDirect3DDevice9* direct3ddevice9, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	HRESULT hResult = S_FALSE;

	ImGui_ImplDX9_InvalidateDeviceObjects();
	hResult = org_Reset(direct3ddevice9, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();

	return hResult;
}


HRESULT 调用约定 self_EndScene(IDirect3DDevice9* direct3ddevice9)
{
	static bool first_call = true;
	if (first_call)
	{
		first_call = false;
		initialize_imgui(g_hwnd,direct3ddevice9);
		g_original_proc = (WNDPROC)SetWindowLongPtrW(g_hwnd, GWLP_WNDPROC, (ULONG_PTR)self_proc);
	}
	///////////////// 开始//////////////////////////////////////
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	///////////////////////////////////////////////////////


	//绘制 界面窗口
	ImGui::Begin("new window");
	ImGui::Text("this test windows");
	ImGui::End();

	//绘制
	drawList = ImGui::GetForegroundDrawList();
	draw_text(100, 14, 2,u8"你是我的小苹果", 绿色);
	draw_rect(150, 200, 60, 70, 1, 绿色);
	draw_line(250, 100, 50, 60, 1, 绿色);
	draw_circle(50, 50, 40, 绿色);
	draw_spot(111, 222, 50, 绿色);
	//////////////////// 结束///////////////////////////////////
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	///////////////////////////////////////////////////////

	if (key_status(VK_F7))
	{
		OutputDebugPrintf("hzw:self_EndScene %x %x\n", GetCurrentThreadId(), self_EndScene);
	}
	return org_EndScene(direct3ddevice9);
}

 HRESULT 调用约定 self_DrawIndexedPrimitive(IDirect3DDevice9* direct3ddevice9, D3DPRIMITIVETYPE type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	
	 //CS起源 人物模型
	 //bool state = (NumVertices == 499 || NumVertices == 811 || NumVertices == 929 || NumVertices == 1404 || NumVertices == 1424 || NumVertices == 1498);
	 //if (state) {
		// direct3ddevice9->SetRenderState(D3DRS_ZENABLE, FALSE);
		// org_DrawIndexedPrimitive(direct3ddevice9, type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	 //}
	 //if (state)
	 //{
		// direct3ddevice9->SetRenderState(D3DRS_ZENABLE, TRUE);
	 //}
	 //return org_DrawIndexedPrimitive(direct3ddevice9, type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	 if (key_status(VK_F6))
	 {
		 OutputDebugPrintf("hzw:self_DrawIndexedPrimitive %x %x\n", GetCurrentThreadId(), self_DrawIndexedPrimitive);
	 }
	  return org_DrawIndexedPrimitive(direct3ddevice9, type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	 
}




unsigned int __stdcall initialize_d3d9(void* data)
{
	IDirect3D9* pDirect3d9 = nullptr;
	IDirect3DDevice9* pDirect3ddevice9 = nullptr;
	D3DPRESENT_PARAMETERS dwPresent;

	g_hwnd = FindWindowW(L"Direct3DWindowClass", nullptr);

	// 只是为了获取 三个函数的地址而已
	pDirect3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	check_error(pDirect3d9, "Direct3DCreate9失败");
	memset(&dwPresent, 0, sizeof(dwPresent));
	dwPresent.Windowed = TRUE;
	dwPresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dwPresent.BackBufferFormat = D3DFMT_UNKNOWN;
	dwPresent.EnableAutoDepthStencil = TRUE;
	dwPresent.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = pDirect3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dwPresent, &pDirect3ddevice9);
	check_error(result == 0, "CreateDevice失败");


	ULONG_PTR* direct3d9_table = (ULONG_PTR*)*(ULONG_PTR*)pDirect3d9;
	ULONG_PTR* direct3ddevice9_table = (ULONG_PTR*)*(ULONG_PTR*)pDirect3ddevice9;


	/*

	// 使用 硬件断点挂钩 但是做多只能挂 四次
	VEH_Initialize();
	VEH_CreateHook(PVOID(direct3ddevice9_table[16]), PVOID(&self_Reset), reinterpret_cast<PVOID*>(&org_Reset));
	VEH_CreateHook(PVOID(direct3ddevice9_table[42]), PVOID(&self_EndScene), reinterpret_cast<PVOID*>(&org_EndScene));
	VEH_CreateHook(PVOID(direct3ddevice9_table[82]), PVOID(&self_DrawIndexedPrimitive), reinterpret_cast<PVOID*>(&org_DrawIndexedPrimitive));
	VEH_EnableHook();

	*/
	//使用 正常 hook
	
	MH_Initialize();
	MH_CreateHook(PVOID(direct3ddevice9_table[16]), PVOID(&self_Reset), reinterpret_cast<PVOID*>(&org_Reset));
	MH_CreateHook(PVOID(direct3ddevice9_table[42]), PVOID(&self_EndScene), reinterpret_cast<PVOID*>(&org_EndScene));
	MH_CreateHook(PVOID(direct3ddevice9_table[82]), PVOID(&self_DrawIndexedPrimitive), reinterpret_cast<PVOID*>(&org_DrawIndexedPrimitive));
	MH_EnableHook(MH_ALL_HOOKS);
	

	//
	if (pDirect3ddevice9) { pDirect3ddevice9->Release(); pDirect3ddevice9 = NULL; }
	if (pDirect3d9) { pDirect3d9->Release(); pDirect3d9 = NULL; }

	return 0;
}



void un_load()
{


}




void initialize_imgui(HWND hwnd, IDirect3DDevice9* direct3ddevice9)
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsLight();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	ImGui::StyleColorsDark();
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simhei.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	// Setup Dear ImGui style
    //ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	 ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(direct3ddevice9);

}


void draw_line(float X1, float Y1, float X2, float Y2, float Width, ImColor Color)
{
	drawList->AddLine({ X1,Y1 }, { X2,Y2 }, Color, Width);
}

void draw_rect(float X, float Y, float W, float H, float Width, ImColor Color)
{
	drawList->AddRect({ X,Y }, { W + X,H + Y }, Color, 0, 0, Width);
}

void draw_text(float X, float Y, float fontSize, std::string Str, ImColor Color)
{

	drawList->AddText(NULL, fontSize, { X,Y }, Color, Str.c_str());
}

void draw_circle(float X, float Y, float radius/*半径*/, ImColor Color)
{

	drawList->AddCircle({ X,Y }, radius, Color);
}
void draw_spot(float X, float Y, float radius/*半径*/, ImColor Color)
{
	drawList->AddCircleFilled({ X,Y }, radius, Color);
}

bool key_status(ULONG key)
{
	if (GetAsyncKeyState(key) & 1)
	{
		return true;
	}
	return false;
}
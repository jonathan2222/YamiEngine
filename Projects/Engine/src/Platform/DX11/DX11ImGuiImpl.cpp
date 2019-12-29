#include "stdafx.h"
#include "DX11ImGuiImpl.h"

#include "DX11API.h"
#include "../../Utils/Imgui/imgui_impl_win32.h"
#include "../../Utils/Imgui/imgui_impl_dx11.h"
#include "../../Engine/Core/Display.h"
#include "DX11Renderer.h"

ym::DX11ImGuiImpl::DX11ImGuiImpl()
{
}

ym::DX11ImGuiImpl::~DX11ImGuiImpl()
{
}

void ym::DX11ImGuiImpl::setUp()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
#if 0
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI
#endif

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	HWND hwnd = (HWND)Display::get()->getNativeDisplay();
	ImGui_ImplWin32_Init(hwnd);
	ID3D11Device* device = DX11API::get()->getDevice();
	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	ImGui_ImplDX11_Init(device, context);

	activate();
}

void ym::DX11ImGuiImpl::startFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ym::DX11ImGuiImpl::endFrame()
{
	ImGui::Render();

	ID3D11DeviceContext* context = DX11API::get()->getDeviceContext();
	DX11Renderer* renderer = dynamic_cast<DX11Renderer*>(Renderer::get());
	ID3D11RenderTargetView* rtv = renderer->getRenderTarget();
	context->OMSetRenderTargets(1, &rtv, NULL);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ym::DX11ImGuiImpl::cleanUp()
{
	deactivate();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

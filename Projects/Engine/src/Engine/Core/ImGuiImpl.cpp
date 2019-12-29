#include "stdafx.h"
#include "ImGuiImpl.h"

#include "../../Platform/GL/GLImGuiImpl.h"
#ifdef YAMI_PLATFORM_WINDOWS
#include "../../Platform/DX11/DX11ImGuiImpl.h"
#endif

ym::ImGuiImpl* ym::ImGuiImpl::create()
{
	static std::string type = Config::get()->fetch<std::string>("API/type");
	if (type == YM_API_GL) return new GLImGuiImpl();
	else if (type == YM_API_DX11) return new DX11ImGuiImpl();
	YM_ASSERT(false, "Could not create ImGuiImpl: API not supported!");
	return nullptr;
}

bool ym::ImGuiImpl::isActive()
{
	return m_isActive;
}

void ym::ImGuiImpl::activate()
{
	m_isActive = true;
}

void ym::ImGuiImpl::deactivate()
{
	m_isActive = false;
}

bool ym::ImGuiImpl::needInput()
{
	if (isActive())
		return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
	else return false;
}

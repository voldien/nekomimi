#include "UIStatusBar.h"
#include "imgui_internal.h"

using namespace nekomimi;

UIStatusBar::UIStatusBar() {
	this->window_flags = window_flags =
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking;

	this->setName("##StatusBar");
}

void UIStatusBar::begin() {
	ImGuiViewport *viewport = ImGui::GetMainViewport();

	if (ImGui::BeginViewportSideBar(this->getName().c_str(), viewport, dir, statusBarHeight, this->window_flags)) {

		this->m_show = true;
		ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
	} else {
		this->m_show = false;
	}
}

void UIStatusBar::end() {
	if (this->m_show) {
		ImGui::End();
	}
}

#include "UIComponent.h"

using namespace MIMIIMGUI;

UIComponent::UIComponent() : window_flags(ImGuiWindowFlags_None) {
	this->m_show = true;
	this->setName("Default");
}

void UIComponent::begin() {

	if (this->isVisible()) {
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
								ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.25f, ImGui::GetIO().DisplaySize.y * 0.25f),
								 ImGuiCond_FirstUseEver);

		this->isContentVisible = ImGui::Begin(getName().c_str(), &this->m_show);
	}
}

void UIComponent::drawUI() {
	if (this->isVisible()) {
		this->begin();
		if (this->isContentVisible)
			this->draw();
		this->end();
	}
}

void UIComponent::end() { ImGui::End(); }

void UIComponent::show(bool state) noexcept { this->m_show = state; }
void UIComponent::showTitle(bool state) {
	if (state)
		this->window_flags |= ImGuiWindowFlags_NoTitleBar;
	else
		this->window_flags &= ~ImGuiWindowFlags_NoTitleBar;
}

bool UIComponent::isVisible() const noexcept { return this->m_show; }

void UIComponent::setHelperInformation(const std::string &info) { this->helperInformation = info; }
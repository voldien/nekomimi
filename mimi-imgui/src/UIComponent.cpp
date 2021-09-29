#include "UIComponents/UIComponent.h"

UIComponent::UIComponent(void) : window_flags(ImGuiWindowFlags_None) { this->m_show = true; }

void UIComponent::begin(void) {

	if (isVisible()) {
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
								ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.25f, ImGui::GetIO().DisplaySize.y * 0.25f),
								 ImGuiCond_FirstUseEver);

		this->isContentVisible = ImGui::Begin(Translation::getInstance()->getText(getName().c_str()), &this->m_show);
	}
}

void UIComponent::drawUI(void) {
	if (isVisible()) {
		this->begin();
		if (this->isContentVisible)
			draw();
		ImGui::End();
	}
}

void UIComponent::end(void) {
	if (isVisible())
		ImGui::End();
}

void UIComponent::show(bool state) noexcept { this->m_show = state; }
void UIComponent::showTitle(bool state) {
	if (state)
		this->window_flags |= ImGuiWindowFlags_NoTitleBar;
	else
		this->window_flags &= ~ImGuiWindowFlags_NoTitleBar;
}

bool UIComponent::isVisible(void) const noexcept { return this->m_show; }

void UIComponent::setHelperInformation(const std::string &info) { this->helperInformation = info; }
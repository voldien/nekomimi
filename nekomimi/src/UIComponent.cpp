#include "UIComponent.h"
#include "imgui.h"

using namespace nekomimi;

UIComponent::UIComponent() : m_show(true), window_flags(ImGuiWindowFlags_None) { this->setName("Default"); }

void UIComponent::begin() {

	if (this->isVisible()) {
		/*	*/
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
								ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.25f, ImGui::GetIO().DisplaySize.y * 0.25f),
								 ImGuiCond_FirstUseEver);

		//						 BeginPopup
		switch (this->windowMode) {
		case WindowMode::Normal:
			this->isContentVisible = ImGui::Begin(this->getName().c_str(), &this->m_show, this->window_flags);
			break;
		case WindowMode::PopUp:
			this->isContentVisible = ImGui::BeginPopup(this->getName().c_str(), this->window_flags);
			this->m_show = true;
			break;
		default:
			break;
		}
	}
}

void UIComponent::drawUI() {
	if (this->isVisible()) {
		this->begin();
		/*	*/
		if (this->isContentVisible) {
			this->draw();
		}
				/*	*/
		this->end();
	}
}

void UIComponent::end() {
	switch (this->windowMode) {
	case WindowMode::Normal:
		ImGui::End();
		break;
	case WindowMode::PopUp:
		ImGui::EndPopup();
		break;
	default:
		break;
	}
}

void UIComponent::show(bool state) noexcept { this->m_show = state; }
void UIComponent::showTitle(bool state) {
	if (state) {
		this->window_flags |= ImGuiWindowFlags_NoTitleBar;
	} else {
		this->window_flags &= ~ImGuiWindowFlags_NoTitleBar;
	}
}

bool UIComponent::isVisible() const noexcept { return this->m_show; }

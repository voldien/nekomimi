#pragma once
#include "UIComponent.h"

namespace nekomimi {

	class UIStatusBar : public UIComponent {
	  public:
		UIStatusBar();

		void begin() override;
		void end() override;

	  protected:
		const float statusBarHeight = 25.0f;
		const ImGuiDir dir = ImGuiDir_Down;
	};
} // namespace nekomimi

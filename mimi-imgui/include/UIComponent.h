#ifndef _MIMI_IMGUI_UICOMPONENT_H_
#define _MIMI_IMGUI_UICOMPONENT_H_ 1
#include <Core/UIDObject.h>
#include <Core/dataStructure/ITree.h>
#include <imgui/imgui.h>
#include <string>

namespace MIMIIMGUI {
	/**
	 * @brief
	 *
	 */
	class UIComponent : public fragcore::UIDObject {
	  public:
		UIComponent();
		virtual ~UIComponent() = default;
		virtual void begin();

		virtual void drawUI();

		virtual void end();

		virtual void show(bool state) noexcept;
		virtual void showTitle(bool state);

		virtual bool isVisible() const noexcept;

		//	virtual const std::string &getName() const { return name; }
		virtual const std::string &getHelperInformation() const { return this->helperInformation; }

		void setName(const std::string &name) { this->name = name; }
		const std::string &getName() const { return this->name; }

		//TODO get size

	  protected:
		virtual void draw() = 0;

		void setHelperInformation(const std::string &info);

	  private:
		bool m_show;
		ImGuiWindowFlags window_flags;
		std::string helperInformation;
		bool isContentVisible;
		std::string name;
	};
} // namespace MIMIIMGUI

#endif

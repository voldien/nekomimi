#ifndef _MIMI_IMGUI_UICOMPONENT_H_
#define _MIMI_IMGUI_UICOMPONENT_H_ 1
#include "Translation.h"
#include <ITree.h>
#include <NoCopyable.h>
#include <imgui/imgui.h>

/**
 * @brief
 *
 */
class UIComponent {
  public:
	UIComponent(void);
	virtual ~UIComponent(void) = default;
	virtual void begin(void);

	virtual void drawUI(void);

	virtual void end(void);

	virtual void show(bool state) noexcept;
	virtual void showTitle(bool state);

	virtual bool isVisible(void) const noexcept;

	//	virtual const std::string &getName(void) const { return name; }
	virtual const std::string &getHelperInformation(void) const { return this->helperInformation; }

  protected:
	virtual void draw() = 0;

	void setHelperInformation(const std::string &info);

  private:
	bool m_show;
	ImGuiWindowFlags window_flags;
	std::string helperInformation;
	bool isContentVisible;
};

#endif
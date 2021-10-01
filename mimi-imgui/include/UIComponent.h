#ifndef _MIMI_IMGUI_UICOMPONENT_H_
#define _MIMI_IMGUI_UICOMPONENT_H_ 1
//#include <ITree.h>
#include <imgui/imgui.h>
#include <string>

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

	void setName(const std::string &name) { this->name = name; }
	const std::string &getName() const { return this->name; }

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

#endif
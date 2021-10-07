#ifndef _MIMI_MIMI_WINDOW_H_
#define _MIMI_MIMI_WINDOW_H_ 1
#include "GraphicBackend/WindowBackend.h"
#include "UIComponent.h"
#include "util/imgui-ext.h"
#include <memory>

namespace MIMIIMGUI {
	/**
	 * @brief
	 *
	 */
	class MIMIWindow : public WindowBackend {
	  public:
		MIMIWindow(GfxBackEnd backend);

	  public:
		void run();

		void quit();

	  protected:
		virtual void displayMenuBar() = 0;
		virtual void renderUI() = 0;

		virtual void addUIComponent(const std::shared_ptr<UIComponent> &component);
		virtual void removeUIComponent(const std::shared_ptr<UIComponent> &component);
		unsigned int getNrUIComponents() const;
		std::shared_ptr<UIComponent> &getComponent(unsigned int index);
		// std::shared_ptr<UIComponent> &getComponent(unsigned int index);

	  private:
		void removeUIComponent(uint32_t uid);

		std::vector<std::shared_ptr<UIComponent>> components;
		bool requestQuit = false;
	};
} // namespace MIMIIMGUI

#endif

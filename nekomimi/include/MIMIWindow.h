/*
 *	NekoMimi - Window ImGUI Framework
 *	Copyright (C) 2021  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _MIMI_MIMI_WINDOW_H_
#define _MIMI_MIMI_WINDOW_H_ 1
#include "GraphicBackend/WindowBackend.h"
#include "MIMIDef.h"
#include "UIComponent.h"
#include "Util/imgui-ext.h"
#include <memory>

namespace nekomimi {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC MIMIWindow : public WindowBackend {
	  public:
		MIMIWindow(GfxBackEnd backend);

	  public:
		void run();

		/**
		 * @brief Request for the application
		 * to quit.
		 *
		 */
		void quit();

	  public:
		virtual void addUIComponent(const std::shared_ptr<UIComponent> &component);
		virtual void removeUIComponent(const std::shared_ptr<UIComponent> &component);
		unsigned int getNrUIComponents() const;
		std::shared_ptr<UIComponent> &getComponent(unsigned int index);

		void setStatusBar(bool enabled);
		void setUIComponentEnabled(bool enabled);

	  protected:
		virtual void displayMenuBar() = 0;
		virtual void renderUI() = 0;

		// std::shared_ptr<UIComponent> &getComponent(unsigned int index);

	  private:
		void removeUIComponent(uint32_t uid);

		std::vector<std::shared_ptr<UIComponent>> components;

	};
} // namespace nekomimi

#endif

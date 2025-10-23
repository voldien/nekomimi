/*
 *	NekoMimi - Window ImGUI Framework
 *	Copyright (c) 2025  Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#pragma once
#include "GraphicBackend/WindowBackend.h"
#include "UIComponent.h"
#include <memory>

namespace nekomimi {

	/**
	 * @brief
	 */
	class FVDECLSPEC MIMIWindow : public WindowBackend {
	  public:
		MIMIWindow(const GfxBackEnd backend,
				   const WindowLibBackend window_backend = WindowLibBackend::WindowBackendSDL2);
		// TODO: based on existing data.
	  public:
		void run();

		/**
		 * @brief Request for the application
		 * to quit.
		 */
		void quit();

	  public:
		virtual void addUIComponent(const std::shared_ptr<UIComponent> &component);
		virtual void removeUIComponent(const std::shared_ptr<UIComponent> &component);
		size_t getNrUIComponents() const noexcept;
		std::shared_ptr<UIComponent> &getComponent(const size_t index);

		void setMenuBarVisable(bool enabled);
		bool getMenuBarVisable() const noexcept;
		void setUIComponentEnabled(bool enabled);

	  protected:
		virtual void displayMenuBar() = 0;
		virtual void renderUI() = 0;

	  private:
		void removeUIComponent(uint32_t uid);

		std::vector<std::shared_ptr<UIComponent>> components;
		bool showMenuBar = false;
	};
} // namespace nekomimi

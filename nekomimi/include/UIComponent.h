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
#ifndef _NEKO_MIMI_UICOMPONENT_H_
#define _NEKO_MIMI_UICOMPONENT_H_ 1
#include <Core/UIDObject.h>
#include <Core/dataStructure/ITree.h>
#include <imgui/imgui.h>
#include <string>

namespace nekomimi {
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

		// TODO get size
		// TODO add get parent

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
} // namespace nekomimi

#endif

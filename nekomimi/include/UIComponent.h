/*
 * Copyright (c) 2021 Valdemar Lindberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

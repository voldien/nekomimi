
#ifndef _MIMI_IMGUI_GLFW_WINDOW_H_
#define _MIMI_IMGUI_GLFW_WINDOW_H_ 1
#include "Window/Window.h"
#include <GLFW/glfw3.h>
#include <string>

namespace MIMIIMGUI {

	class GLFWWindow : public fragcore::Window {
	  public:
		GLFWWindow();

		virtual void show() override;

		virtual void hide() override;

		virtual void close() override;

		virtual void focus() override;

		virtual void restore() override;

		virtual void maximize() override;

		virtual void minimize() override;

		virtual void setTitle(const std::string &title) override;

		virtual std::string getTitle() const override;

		// virtual int x() const noexcept override;
		// virtual int y() const noexcept override;

		// virtual int width() const noexcept override;
		// virtual int height() const noexcept override;

		virtual void getPosition(int *x, int *y) const override;

		virtual void setPosition(int x, int y) noexcept override;

		virtual void setSize(int width, int height) noexcept override;

		virtual void getSize(int *width, int *height) const override;

		virtual void resizable(bool resizable) noexcept;

		virtual void setFullScreen(bool fullscreen);

		virtual bool isFullScreen() const;

		virtual void setBordered(bool borded);

		virtual float getGamma() const override;

		virtual void setGamma(float gamma) override;

		virtual void setMinimumSize(int width, int height) override;
		virtual void getMinimumSize(int *width, int *height) override;
		virtual void setMaximumSize(int width, int height) override;
		virtual void getMaximumSize(int *width, int *height) override;

		virtual intptr_t getNativePtr() const override; /*  Get native window reference object. */

	  protected:
		std::string title;
		GLFWwindow *window;
	};
} // namespace MIMIIMGUI

#endif

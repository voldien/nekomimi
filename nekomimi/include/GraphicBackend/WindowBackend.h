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
#ifndef _NEKO_MIMI_UI_WINDOW_BACKEND_H_
#define _NEKO_MIMI_UI_WINDOW_BACKEND_H_ 1
#define IMTERM_USE_FMT 1
#include "UIViewModel.h"
#include <CommandList.h>
#include <IRenderer.h>
#include <VKDevice.h>
#include <VulkanCore.h>
#include <Window.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ImTui {
	struct TScreen;
} // namespace ImTui

namespace nekomimi {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC WindowBackend : public fragcore::Window {
	  public:
		enum class GfxBackEnd : size_t {
			/**
			 * @brief
			 *
			 */
			ImGUI_Terminal,
			/**
			 * @brief
			 *
			 */
			ImGUI_OpenGL,
			/**
			 * @brief
			 *
			 */
			ImGUI_Vulkan,
			/**
			 * @brief
			 *
			 */
			ImGUI_DirectX9,
			/**
			 * @brief
			 *
			 */
			ImGUI_DirectX10,
			/**
			 * @brief
			 *
			 */
			ImGUI_DirectX11,
			/**
			 * @brief
			 *
			 */
			ImGUI_DirectX12,
			ImGUI_Default,
		};

		/**
		 * @brief
		 *
		 */
		enum class WindowLibBackend {
			/**
			 * @brief
			 *
			 */
			WindowBackendSDL2,
			/**
			 * @brief
			 *
			 */
			WindowBackendGLFW3,
			/**
			 * @brief
			 *
			 */
			WindowBackendWindows,
			/**
			 * @brief
			 *
			 */
			WindowBaclendDefault,
		};

		static const char *getGfxBackEndSymbol(GfxBackEnd v) noexcept;
		static const char *getWindowBackEndSymbol(WindowLibBackend v) noexcept;

		/**
		 * @brief
		 *
		 * @param gfxBackend
		 * @return true
		 * @return false
		 */
		static bool isGfxBackendSupported(GfxBackEnd gfxBackend);
		/**
		 * @brief
		 *
		 * @param windowBackend
		 * @return true
		 * @return false
		 */
		static bool isWindowBackendSupported(WindowLibBackend windowBackend);

	  public:
		WindowBackend(WindowLibBackend windowBackend, GfxBackEnd backend);
		// TODO add support to provide renderinterface.
		// WindowBackend()
		// WindowBackend(std::shared_ptr<fragcore::IRenderer> &render);
		virtual ~WindowBackend();

		/**
		 * @brief
		 *
		 * @param backend
		 */
		virtual void initGfx(GfxBackEnd backend);

		/**
		 * @brief
		 *
		 * @param windowBackend
		 */
		virtual void initWindow(WindowLibBackend windowBackend);

		void initTerminal();
		void initVulkan();
		void initOpenGL();
		void initDX9();
		void initDX10();
		void initDX11();
		void initDX12();

		virtual void beginRender();
		virtual void endRender();

		virtual void beginRenderVulkan();
		virtual void beginRenderOpenGL();
		virtual void beginRenderTerminal();
		virtual void beginRenderDX9();
		virtual void beginRenderDX10();
		virtual void beginRenderDX11();
		virtual void beginRenderDX12();

		virtual void endRenderVulkan();
		virtual void endRenderOpenGL();
		virtual void endRenderTerminal();
		virtual void endRenderDX9();
		virtual void endRenderDX10();
		virtual void endRenderDX11();
		virtual void endRenderDX12();

		virtual void releaseRender();

	  public: /*	*/
		virtual void loadFont(const std::string &path);

		virtual void enableImGUI(bool enabled);
		virtual void enableDocking(bool enabled);
		virtual void enableViewPorts(bool enabled);
		virtual void showDockSpace(bool *open);
		virtual void showViewPorts(bool *open);

	  public:
		GfxBackEnd getBackendRenderer() const noexcept { return this->gfxBackend; }

		WindowLibBackend getBackendWindowManager() const noexcept { return this->windowBackend; }

		const std::shared_ptr<fragcore::IRenderer> &getRenderInterface() const noexcept { return this->renderer; }
		std::shared_ptr<fragcore::IRenderer> &getRenderInterface() noexcept { return this->renderer; }

		const std::shared_ptr<fragcore::CommandList> &getRenderCommandBuffer() const noexcept {
			return this->commandList;
		}
		std::shared_ptr<fragcore::CommandList> &getRenderCommandBuffer() noexcept { return this->commandList; }

	  public:
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

		virtual int width() const noexcept override;
		virtual int height() const noexcept override;

		virtual void getPosition(int *x, int *y) const override;

		virtual void setPosition(int x, int y) override;
		virtual void setSize(int width, int height) override;

		virtual void getSize(int *width, int *height) const override;

		virtual void resizable(bool resizable) noexcept override;

		virtual void setFullScreen(bool fullscreen) override;

		virtual bool isFullScreen() const override;

		virtual void setBordered(bool borded) override;

		virtual float getGamma() const override;

		virtual void setGamma(float gamma) override;

		virtual void setMinimumSize(int width, int height) override;
		virtual void getMinimumSize(int *width, int *height) override;
		virtual void setMaximumSize(int width, int height) override;
		virtual void getMaximumSize(int *width, int *height) override;

		// TODO change the type to image.
		virtual void setIcon(void *) override{};
		//		virtual void setIcon(Image* image) = 0;

		virtual void *getIcon() const override { return nullptr; }
		virtual fragcore::Display *getCurrentDisplay() const override { return this->proxyWindow->getCurrentDisplay(); }
		virtual void setFullScreen(fragcore::Display &display) override {}

		//		virtual Image* setIcon(Image* image) = 0;

		virtual intptr_t getNativePtr() const override; /*  Get native window reference object. */
	  protected:
		bool useImGUI = true;
		bool useDocking = true;
		bool requestQuit = false;

	  private:
		bool visible;

		GfxBackEnd gfxBackend;
		WindowLibBackend windowBackend;

		/*	*/
		std::shared_ptr<fragcore::IRenderer> renderer;
		std::shared_ptr<fragcore::CommandList> commandList;
		/*	*/

		ImTui::TScreen *imtuiScreen;
		/*	*/
		fragcore::Window *proxyWindow;

		bool requestResize{false};
	};
} // namespace nekomimi

#endif

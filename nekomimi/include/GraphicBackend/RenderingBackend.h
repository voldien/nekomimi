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
#ifndef _NEKO_MIMI_UI_RENDERING_BACKEND_H_
#define _NEKO_MIMI_UI_RENDERING_BACKEND_H_ 1
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

namespace nekomimi {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC RenderBackend {
	  public:
		enum class GfxBackEnd {
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
		RenderBackend(WindowLibBackend windowBackend, GfxBackEnd backend);
		// TODO add support to provide renderinterface.
		// WindowBackend()
		// WindowBackend(std::shared_ptr<fragcore::IRenderer> &render);
		virtual ~RenderBackend();

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

	  private:
		bool visible;

		GfxBackEnd gfxBackend;
		WindowLibBackend windowBackend;

		/*	*/
		std::shared_ptr<fragcore::IRenderer> renderer;
		std::shared_ptr<fragcore::CommandList> commandList;
		/*	*/
	};

} // namespace nekomimi

#endif
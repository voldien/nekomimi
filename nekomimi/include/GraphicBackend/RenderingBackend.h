/*
 *	NekoMimi - Window ImGUI Framework
 *	Copyright (C) 2021  Valdemar Lindberg
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
#ifndef _NEKO_MIMI_UI_RENDERING_BACKEND_H_
#define _NEKO_MIMI_UI_RENDERING_BACKEND_H_ 1
#define IMTERM_USE_FMT 1
#include "GraphicBackendDef.h"
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
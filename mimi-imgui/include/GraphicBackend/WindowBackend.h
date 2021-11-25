#ifndef _MIMI_IMGUI_UI_WINDOW_BACKEND_H_
#define _MIMI_IMGUI_UI_WINDOW_BACKEND_H_ 1
#define IMTERM_USE_FMT 1
#include "Window/Window.h"
//#include <SDL2/SDL.h>
#include <IRenderer.h>
#include <imgui/backends/imgui_impl_vulkan.h>
//#include <imtui/imtui.h>
#include <VKDevice.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ImTui {
	struct TScreen;
} // namespace ImTui

namespace MIMIIMGUI {

	class WindowBackend : public fragcore::Window {
	  public:
		enum class GfxBackEnd {
			ImGUI_Terminal,
			ImGUI_OpenGL,
			ImGUI_Vulkan,
			ImGUI_DirectX9,
			ImGUI_DirectX10,
			ImGUI_DirectX11,
			ImGUI_DirectX12
		};

		enum class WindowLibBackend { WindowBackendSDL2, WindowBackendGLFW3, WindowBackendWindows };

		static const char *getGfxBackEndSymbol(GfxBackEnd v) noexcept;
		static const char *getWindowBackEndSymbol(WindowLibBackend v) noexcept;
		static bool isGfxBackendSupported(GfxBackEnd gfxBackend);
		static bool isWindowBackendSupported(WindowLibBackend windowBackend);

		WindowBackend(WindowLibBackend windowBackend, GfxBackEnd backend);
		virtual ~WindowBackend();

		virtual void initGfx(GfxBackEnd backend);
		virtual void initWindow(WindowLibBackend windowBackend);

		void initTerminal();
		void initVulkan();
		void initOpenGL();
		void initDX9();
		void initDX10();
		void initDX11();
		void initDX12();

		virtual void showDockSpace(bool *open);

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

		GfxBackEnd getBackendRenderer() const noexcept { return this->gfxBackend; }

		WindowLibBackend getBackendWindowManager() const noexcept { return this->windowBackend; }

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

		// virtual int width() const noexcept override;
		// virtual int height() const noexcept override;

		virtual void getPosition(int *x, int *y) const override;

		virtual void setPosition(int x, int y) override;

		virtual void setSize(int width, int height) override;

		virtual void getSize(int *width, int *height) const;

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

		// TODO change the type to image.
		virtual void setIcon(void *) override{};
		//		virtual void setIcon(Image* image) = 0;

		virtual void *getIcon() const override{};
		virtual fragcore::Display *getCurrentDisplay() const override {}
		virtual void setFullScreen(fragcore::Display &display) override {}

		//		virtual Image* setIcon(Image* image) = 0;

		virtual intptr_t getNativePtr() const override; /*  Get native window reference object. */

	  private:
		GfxBackEnd gfxBackend;
		WindowLibBackend windowBackend;

		/*	*/
		// SDL_Window *gfxWindow;
		std::shared_ptr<fragcore::IRenderer> renderer;

		// TODO can be relocated.
		std::shared_ptr<VulkanCore> vkCore;
		std::vector<std::shared_ptr<PhysicalDevice>> vkPhysicalDevices;
		std::shared_ptr<VKDevice> vkDevice;

		/*	*/
		ImGui_ImplVulkanH_Window wd;
		ImTui::TScreen *imtuiScreen;
		/*	*/
		fragcore::Window *proxyWindow;
	};
} // namespace MIMIIMGUI

#endif

#ifndef _MIMI_IMGUI_UI_WINDOW_BACKEND_H_
#define _MIMI_IMGUI_UI_WINDOW_BACKEND_H_ 1
#define IMTERM_USE_FMT 1
#include "Window.h"
//#include <SDL2/SDL.h>
#include <imgui/backends/imgui_impl_vulkan.h>
//#include <imtui/imtui.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "SDL_video.h"
#include "VKDevice.h"
#include "VulkanCore.h"

class PhysicalDevice;
class VKDevice;
class VulkanCore;
namespace ImTui {
	struct TScreen;
} // namespace ImTui

class WindowBackend : public Window {
  public:
	enum GfxBackEnd {
		ImGUI_Terminal,
		ImGUI_OpenGL,
		ImGUI_Vulkan,
		ImGUI_DirectX9,
		ImGUI_DirectX10,
		ImGUI_DirectX11,
		ImGUI_DirectX12
	};

	enum WindowBackend {
		WindowBackendSDL2,
		WindowBackendGLFW3,
		WindowBackendWindows
	};

	static const char *getGfxBackEndSymbol(GfxBackEnd v) noexcept;
	static const char *getWindowBackEndSymbol(GfxBackEnd v) noexcept;
	WindowBackend(GfxBackEnd backend);
	~WindowBackend();

	virtual void initGfx(GfxBackEnd backend);

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
	virtual void releaseRender();

	GfxBackEnd getBackendRenderer() const noexcept { return this->gfxBackend; }

  public:
	virtual void show() noexcept;

	virtual void hide() noexcept;

	virtual void close() noexcept;

	virtual void focus();

	virtual void restore();

	virtual void maximize();

	virtual void minimize();

	virtual void setTitle(const std::string &title);

	virtual std::string getTitle();

	virtual int x() const noexcept;
	virtual int y() const noexcept;

	virtual int width() const noexcept;
	virtual int height() const noexcept;

	virtual void getPosition(int *x, int *y) const;

	virtual void setPosition(int x, int y) noexcept;

	virtual void setSize(int width, int height) noexcept;

	virtual void getSize(int *width, int *height) const;

	virtual void resizable(bool resizable) noexcept;

	virtual void setFullScreen(bool fullscreen);

	virtual bool isFullScreen() const;

	virtual void setBordered(bool borded);

	virtual void setMinimumSize(int width, int height);
	virtual void getMinimumSize(int *width, int *height);
	virtual void setMaximumSize(int width, int height);
	virtual void getMaximumSize(int *width, int *height);

  private:
	GfxBackEnd gfxBackend;
	SDL_Window *gfxWindow;
	SDL_GLContext gl_context;
	std::shared_ptr<VulkanCore> vkCore;
	std::vector<std::shared_ptr<PhysicalDevice>> vkPhysicalDevices;
	std::shared_ptr<VKDevice> vkDevice;
	ImGui_ImplVulkanH_Window wd;

	ImTui::TScreen *imtuiScreen;
};

#endif

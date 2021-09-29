#ifndef _MIMI_IMGUI_UI_WINDOW_BACKEND_H_
#define _MIMI_IMGUI_UI_WINDOW_BACKEND_H_ 1
#define IMTERM_USE_FMT 1
#include <SDL2/SDL.h>
#include <chrono>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imtui/imtui.h>
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

class WindowBackend {
  public:
	enum GfxBackEnd { ImGUI_Terminal, ImGUI_OpenGL, ImGUI_Vulkan };
	static const char *getGfxBackEndSymbol(GfxBackEnd v) noexcept;
	WindowBackend(GfxBackEnd backend);
	~WindowBackend(void);

	virtual void initGfx(GfxBackEnd backend);

	void initTerminal(void);
	void initVulkan(void);
	void initOpenGL(void);

	virtual void showDockSpace(bool *open);

	virtual void beginRender(void);
	virtual void endRender(void);
	virtual void releaseRender(void);

	GfxBackEnd getBackendRenderer(void) const noexcept { return this->gfxBackend; }

  public:
	virtual void show(void) noexcept;

	virtual void hide(void) noexcept;

	virtual void close(void) noexcept;

	virtual void focus(void);

	virtual void restore(void);

	virtual void maximize(void);

	virtual void minimize(void);

	virtual void setTitle(const std::string &title);

	virtual std::string getTitle(void);

	virtual int x(void) const noexcept;
	virtual int y(void) const noexcept;

	virtual int width(void) const noexcept;
	virtual int height(void) const noexcept;

	virtual void getPosition(int *x, int *y) const;

	virtual void setPosition(int x, int y) noexcept;

	virtual void setSize(int width, int height) noexcept;

	virtual void getSize(int *width, int *height) const;

	virtual void resizable(bool resizable) noexcept;

	virtual void setFullScreen(bool fullscreen);

	virtual bool isFullScreen(void) const;

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
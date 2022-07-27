#include "GraphicBackend/WindowBackend.h"
#include <GL/glew.h>
#include <RendererFactory.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <VKDevice.h>
#include <WindowManager.h>
#include <algorithm>
#include <fmt/format.h>
#include <imgui/backends/imgui_impl_dx9.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <opengl/GLRenderWindow.h>
#include <opengl/GLRendererInterface.h>
#include <vulkan/VKRenderInterface.h>
#include <vulkan/VKRenderWindow.h>
/*	TODO include directX if supported.	*/
#include <cstdint>
#include <imgui/imgui.h>
#include <memory>

using namespace nekomimi;
using namespace fragcore;

const char *WindowBackend::getGfxBackEndSymbol(GfxBackEnd v) noexcept {
	switch (v) {
	case GfxBackEnd::ImGUI_OpenGL:
		return "OpenGL";
	case GfxBackEnd::ImGUI_Vulkan:
		return "Vulkan";
	case GfxBackEnd::ImGUI_Terminal:
		return "Terminal";
	case GfxBackEnd::ImGUI_DirectX9:
		return "DirectX9";
	case GfxBackEnd::ImGUI_DirectX10:
		return "DirectX10";
	case GfxBackEnd::ImGUI_DirectX11:
		return "DirectX11";
	case GfxBackEnd::ImGUI_DirectX12:
		return "DirectX12";
	default:
		assert(0);
		return "";
	}
}

const char *WindowBackend::getWindowBackEndSymbol(WindowLibBackend v) noexcept {
	switch (v) {
	case WindowLibBackend::WindowBackendSDL2:
		return "SDL2";
	case WindowLibBackend::WindowBackendGLFW3:
		return "GLFW3";
	case WindowLibBackend::WindowBackendWindows:
		return "Windows";
	default:
		assert(0);
		return "";
	}
}

bool WindowBackend::isGfxBackendSupported(GfxBackEnd gfxBackend) {
	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_OpenGL:
	case GfxBackEnd::ImGUI_Vulkan:
		return true;
	case GfxBackEnd::ImGUI_Terminal:
	case GfxBackEnd::ImGUI_DirectX9:
	case GfxBackEnd::ImGUI_DirectX10:
	case GfxBackEnd::ImGUI_DirectX11:
	case GfxBackEnd::ImGUI_DirectX12:
	default:
		return false;
	}
}
bool WindowBackend::isWindowBackendSupported(WindowLibBackend windowBackend) {
	switch (windowBackend) {
	default:
		return true;
	}
}

WindowBackend::WindowBackend(WindowLibBackend windowBackend, GfxBackEnd gfxBackend) {
	if (!isWindowBackendSupported(windowBackend)) {
		throw RuntimeException("Window Backed not Supported {}", getWindowBackEndSymbol(windowBackend));
	}
	if (!isGfxBackendSupported(gfxBackend)) {
		throw RuntimeException("Graphic Backed not Supported {}", getGfxBackEndSymbol(gfxBackend));
	}
	/*	*/
	this->initWindow(windowBackend);
	this->initGfx(gfxBackend);
}

WindowBackend::~WindowBackend() { releaseRender(); }

void WindowBackend::releaseRender() {
	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Terminal:

		// ImTui_ImplText_Shutdown();
		// ImTui_ImplNcurses_Shutdown();
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		ImGui_ImplOpenGL3_Shutdown();
		break;
	case GfxBackEnd::ImGUI_Vulkan:
		ImGui_ImplVulkan_Shutdown();
		break;
	default:
		break;
	}
	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Vulkan:
	case GfxBackEnd::ImGUI_OpenGL:
		break;
	default:
		break;
	}
	delete this->proxyWindow;

	ImGui::DestroyContext();
}

void WindowBackend::initGfx(GfxBackEnd backend) {
	this->gfxBackend = backend;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	if (backend == GfxBackEnd::ImGUI_OpenGL || backend == GfxBackEnd::ImGUI_Vulkan) {
		// TODO relocate
		// SDL_Init(SDL_INIT_EVERYTHING);

		// setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGuiStyle &style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(8, 6);
		style.WindowRounding = 0.0f;
		style.FramePadding = ImVec2(5, 7);
		style.ItemSpacing = ImVec2(5, 5);
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	}

	/*	*/
	switch (this->getBackendRenderer()) {
	case GfxBackEnd::ImGUI_Terminal:
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		this->initOpenGL();
		break;
	case GfxBackEnd::ImGUI_Vulkan:
		this->initVulkan();
		break;
	case GfxBackEnd::ImGUI_DirectX9:
		break;
	case GfxBackEnd::ImGUI_DirectX10:
		break;
	case GfxBackEnd::ImGUI_DirectX11:
		break;
	case GfxBackEnd::ImGUI_DirectX12:
		break;

	default:
		break;
	}
}

void WindowBackend::initWindow(WindowLibBackend windowBackend) {
	this->windowBackend = windowBackend;
	switch (getBackendWindowManager()) {
	case WindowLibBackend::WindowBackendSDL2:
		// this->proxyWindow = new SDLWindow();
		break;
	case WindowLibBackend::WindowBackendGLFW3:
		break;
	case WindowLibBackend::WindowBackendWindows:
		break;
	default:
		assert(0);
		break;
	}
}

void WindowBackend::initTerminal() {
	// this->imtuiScreen = ImTui_ImplNcurses_Init(true);
	// ImTui_ImplText_Init();
}

void WindowBackend::initVulkan() {

	int width = 800;
	int height = 600;

	/*	Create render interface.	*/
	this->renderer = std::shared_ptr<VKRenderInterface>(new VKRenderInterface(nullptr));
	VKRenderInterface *vkRenderer = (VKRenderInterface *)this->renderer.get();

	fragcore::Display *mainDisplay;

	/*	*/
	this->proxyWindow = renderer->createWindow(0, 0, 2560, 1440);

	/*	Create command buffer.	*/
	VKRenderWindow *renderWindow = static_cast<VKRenderWindow *>(this->proxyWindow);
	this->commandList = std::shared_ptr<CommandList>(renderer->createCommandBuffer());

	std::unordered_map<const char *, bool> required_instance_extensions = {{VK_KHR_SURFACE_EXTENSION_NAME, true},
																		   {"VK_KHR_xlib_surface", true}};

	std::unordered_map<const char *, bool> required_instance_layer = {{"VK_LAYER_LUNARG_standard_validation", false}};
	std::unordered_map<const char *, bool> required_device_extensions = {{VK_KHR_SWAPCHAIN_EXTENSION_NAME, true}};

	// ImGui_ImplVulkanH_Window wd = {};
	// renderWindow->getSize(&wd.Width, &wd.Height);
	// wd.Swapchain = renderWindow->getSwapChain().swapchain;
	// wd.Surface = renderWindow->getSurface();
	// wd.SurfaceFormat = renderWindow->getSurfaceFormat();
	// wd.PresentMode = renderWindow->getPresentMode();
	// wd.RenderPass = renderWindow->getDefaultRenderPass();

	/*	Check if any of the gpu devices and which of their queue support present.	*/
	// int gpu_index = -1;
	// for (unsigned int i = 0; i < renderWindow->getVKDevice()->getNrPhysicalDevices(); i++) {
	// 	for (unsigned int j = 0; j < renderWindow->getVKDevice()->getPhysicalDevice(i)->getNrQueueFamilyProperties();
	// 		 j++) {

	// 		if (renderWindow->getVKDevice()->getPhysicalDevice(i)->isPresentable(wd.Surface, j)) {
	// 			gpu_index = i;
	// 		}
	// 	}
	// }
	// assert(gpu_index < (int)renderWindow->getVKDevice()->getNrPhysicalDevices());

	const std::shared_ptr<fvkcore::PhysicalDevice> &gpuDevice = renderWindow->getVKDevice()->getPhysicalDevice(0);

	VkDescriptorPool desc_pool;
	VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
										 {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
										 {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
										 {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
										 {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
										 {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
										 {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	VKS_VALIDATE(vkCreateDescriptorPool(renderWindow->getVKDevice()->getHandle(), &pool_info, nullptr, &desc_pool));

	VkPipelineCache cache = fvkcore::VKHelper::createPipelineCache(vkRenderer->getDevice()->getHandle(), 0, nullptr);

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = vkRenderer->getInstance()->getHandle();
	init_info.PhysicalDevice = gpuDevice->getHandle();
	init_info.Device = renderWindow->getVKDevice()->getHandle();
	init_info.QueueFamily = renderWindow->getVKDevice()->getDefaultGraphicQueueIndex();
	init_info.Queue = renderWindow->getVKDevice()->getDefaultGraphicQueue();
	init_info.PipelineCache = cache;
	init_info.DescriptorPool = desc_pool;
	init_info.Subpass = 0;
	init_info.MinImageCount = 2;
	init_info.ImageCount = renderWindow->getNrCommandBuffers();
	init_info.CheckVkResultFn = nullptr;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = nullptr;

	/*	*/
	SDL_Window *window = SDL_CreateWindowFrom((const void *)renderWindow->getNativePtr());

	/*	*/
	if (!ImGui_ImplSDL2_InitForVulkan(window)) {
		throw fragcore::RuntimeException("Failed init SDL2 ImGUI Vulkan");
	}

	/*	*/
	if (!ImGui_ImplVulkan_Init(&init_info, renderWindow->getDefaultRenderPass())) {
		throw fragcore::RuntimeException("Failed init ImGUI Vulkan");
	}

	/*  Create command pool.    */
	VkCommandPool command_pool;
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.queueFamilyIndex = renderWindow->getVKDevice()->getDefaultGraphicQueueIndex();
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VKS_VALIDATE(
		vkCreateCommandPool(renderWindow->getVKDevice()->getHandle(), &cmdPoolCreateInfo, NULL, &command_pool));

	{
		// TODO relocate.
		VkCommandBuffer command_buffer =
			fvkcore::VKHelper::beginSingleTimeCommands(renderWindow->getVKDevice()->getHandle(), command_pool);

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		VKS_VALIDATE(vkBeginCommandBuffer(command_buffer, &begin_info));

		/*	*/
		if (!ImGui_ImplVulkan_CreateFontsTexture(command_buffer)) {
			throw fragcore::RuntimeException("Failed to generate font ImGUI Vulkan");
		}

		VkSubmitInfo end_info = {};
		end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		end_info.commandBufferCount = 1;
		end_info.pCommandBuffers = &command_buffer;
		VKS_VALIDATE(vkEndCommandBuffer(command_buffer));

		// VKHelper::endSingleTimeCommands
		VKS_VALIDATE(
			vkQueueSubmit(renderWindow->getVKDevice()->getDefaultGraphicQueue(), 1, &end_info, VK_NULL_HANDLE));

		VKS_VALIDATE(vkDeviceWaitIdle(renderWindow->getVKDevice()->getHandle()));

		VKS_VALIDATE(vkResetCommandBuffer(command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));

		vkFreeCommandBuffers(renderWindow->getVKDevice()->getHandle(), command_pool, 1, &command_buffer);

		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}
}

void WindowBackend::initOpenGL() {

	/*	*/
	fragcore::GLRendererInterface *openGLRenderer = new fragcore::GLRendererInterface(nullptr);
	this->renderer = std::shared_ptr<fragcore::IRenderer>(openGLRenderer);

	/*	*/
	void *gl_context = openGLRenderer->getOpenGLContext();
	this->proxyWindow = (fragcore::Window *)openGLRenderer->createWindow(1, 1, 800, 600);
	/*	*/
	this->commandList = std::shared_ptr<fragcore::CommandList>(openGLRenderer->createCommandBuffer());

	std::string glsl_version = "";
#ifdef __APPLE__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 150";
#elif __linux__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 330";
#elif _WIN32
	// GL 3.0 + GLSL 130
	glsl_version = "#version 130";
#endif

	// enable VSync
	// TODO replace with the renderwindow.
	int rc = SDL_GL_SetSwapInterval(1);

	/*	*/
	if (!ImGui_ImplSDL2_InitForOpenGL((SDL_Window *)this->getNativePtr(), gl_context)) {
		throw fragcore::RuntimeException("Failed to Init ImGUI SDL2 OpenGL");
	}
	/*	*/
	if (!ImGui_ImplOpenGL3_Init(glsl_version.c_str())) {
		throw fragcore::RuntimeException("Failed to Init ImGUI OpenGL3");
	}
}

void WindowBackend::loadFont(const std::string &path) {
	// TODO add support for multiple
	ImFontConfig config;
	ImGuiIO &io = ImGui::GetIO();
	config.MergeMode = true;
	io.Fonts->AddFontFromFileTTF(path.c_str(), 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());
	io.Fonts->Build();
}

void WindowBackend::enableImGUI(bool enabled) { useImGUI = enabled; }
void WindowBackend::enableDocking(bool enabled) { this->useDocking = enabled; }
void WindowBackend::enableViewPorts(bool enabled) {}

void WindowBackend::showDockSpace(bool *open) {
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen) {
		ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("###DockSpace", open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	} else {
		// ShowDockingDisabledMessage();
	}

	ImGui::End();
}
void WindowBackend::showViewPorts(bool *open) {}

void WindowBackend::beginRenderVulkan() {
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
}
void WindowBackend::beginRenderOpenGL() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
}
void WindowBackend::beginRenderTerminal() {
	// ImTui_ImplNcurses_NewFrame();
	// ImTui_ImplText_NewFrame();
}
void WindowBackend::beginRenderDX9() {
	// ImGui_ImplDX9_NewFrame();
	ImGui_ImplSDL2_NewFrame();
}
void WindowBackend::beginRenderDX10() {
	// ImGui_ImplDX10_NewFrame();
	ImGui_ImplSDL2_NewFrame();
}
void WindowBackend::beginRenderDX11() {
	// ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame();
}
void WindowBackend::beginRenderDX12() {
	// ImGui_ImplDX12_NewFrame();
	ImGui_ImplSDL2_NewFrame();
}

void WindowBackend::endRenderVulkan() {
	VKRenderWindow *renderWindow = static_cast<VKRenderWindow *>(this->proxyWindow);

	VkCommandBuffer currentCmd = renderWindow->getCurrentCommandBuffer();

	int width, height;
	renderWindow->getSize(&width, &height);

	VKS_VALIDATE(vkQueueWaitIdle(renderWindow->getDefaultGraphicQueue()));

	VKS_VALIDATE(
		vkResetCommandPool(renderWindow->getVKDevice()->getHandle(), renderWindow->getGraphicCommandPool(), 0));

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VKS_VALIDATE(vkBeginCommandBuffer(currentCmd, &beginInfo));

	// TOOD execute the command line.

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderWindow->getDefaultRenderPass();
	renderPassInfo.framebuffer = renderWindow->getFrameBuffer(renderWindow->getCurrentFrameIndex());
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent.width = width;
	renderPassInfo.renderArea.extent.height = height;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(currentCmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderWindow->getCurrentCommandBuffer());

	vkCmdEndRenderPass(currentCmd);

	VKS_VALIDATE(vkEndCommandBuffer(currentCmd));

	renderWindow->swapBuffer();
}

void WindowBackend::endRenderOpenGL() {
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiIO &io = ImGui::GetIO();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow((SDL_Window *)getNativePtr());
	// TODO check if can be removed or configured with flag.
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
				 clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void WindowBackend::endRenderTerminal() {
	// ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), this->imtuiScreen);
	// ImTui_ImplNcurses_DrawScreen();
}
void WindowBackend::endRenderDX9() {}
void WindowBackend::endRenderDX10() {}
void WindowBackend::endRenderDX11() {}
void WindowBackend::endRenderDX12() {}

void WindowBackend::beginRender() {

	this->commandList->begin();

	if (this->gfxBackend == WindowBackend::GfxBackEnd::ImGUI_Vulkan ||
		this->gfxBackend == WindowBackend::GfxBackEnd::ImGUI_OpenGL ||
		this->gfxBackend == WindowBackend::GfxBackEnd::ImGUI_DirectX9) {

		int windowWidth;
		int windowHeight;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// without it you won't have keyboard input and other things
			if (ImGui_ImplSDL2_ProcessEvent(&event)) {
				/*	*/
			}
			// you might also want to check io.WantCaptureMouse and io.WantCaptureKeyboard
			// before processing events

			switch (event.type) {
			case SDL_QUIT:
				this->requestQuit = true;
				break;
			case SDL_KEYDOWN:
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					windowWidth = event.window.data1;
					windowHeight = event.window.data2;
					// this->commandList->setViewport(0, 0, windowWidth, windowHeight);
					this->requestResize = true;
					break;
				case SDL_WINDOWEVENT_CLOSE:
					this->requestQuit = true;
					break;
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
					this->visible = false;
					break;
				case SDL_WINDOWEVENT_EXPOSED:
				case SDL_WINDOWEVENT_SHOWN:
					this->visible = true;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}

	this->commandList->end();

	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Terminal:
		beginRenderTerminal();
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		beginRenderOpenGL();
		break;
	case GfxBackEnd::ImGUI_Vulkan:
		beginRenderVulkan();
		break;
	default:
		break;
	}
}

void WindowBackend::endRender() {

	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Terminal:
		endRenderTerminal();
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		endRenderOpenGL();
		break;
	case GfxBackEnd::ImGUI_Vulkan: {
		endRenderVulkan();
	} break;
	case GfxBackEnd::ImGUI_DirectX9:
	case GfxBackEnd::ImGUI_DirectX10:
	case GfxBackEnd::ImGUI_DirectX11:
	case GfxBackEnd::ImGUI_DirectX12:
	default:
		break;
	}
	/*	Finalize and execute.	*/

	// this->renderer->execute(this->commandList.get());
}
void WindowBackend::show() { this->proxyWindow->show(); }

void WindowBackend::hide() { this->proxyWindow->hide(); }

void WindowBackend::close() { this->hide(); }

void WindowBackend::setPosition(int x, int y) { this->proxyWindow->setPosition(x, y); }

void WindowBackend::setSize(int width, int height) { /*	TODO determine if it shall update framebuffera as well.	*/
	this->proxyWindow->setSize(width, height);
	//						recreateSwapChain();
}

void WindowBackend::getPosition(int *x, int *y) const { this->proxyWindow->getPosition(x, y); }

void WindowBackend::getSize(int *width, int *height) const { this->proxyWindow->getSize(width, height); }
void WindowBackend::setTitle(const std::string &title) { this->proxyWindow->setTitle(title); }

std::string WindowBackend::getTitle() const { return this->proxyWindow->getTitle(); }

// int WindowBackend::x() const noexcept {
// 	int x, y;

// 	return x;
// }
// int WindowBackend::y() const noexcept {
// 	int x, y;

// 	return y;
// }
int WindowBackend::width() const noexcept { return this->proxyWindow->width(); }
int WindowBackend::height() const noexcept { return this->proxyWindow->height(); }

void WindowBackend::resizable(bool resizable) noexcept {}

void WindowBackend::setFullScreen(bool fullscreen) {}

bool WindowBackend::isFullScreen() const { return false; }

void WindowBackend::setBordered(bool bordered) {}

float WindowBackend::getGamma() const { return this->proxyWindow->getGamma(); }

void WindowBackend::setGamma(float gamma) { return this->proxyWindow->setGamma(gamma); }

void WindowBackend::setMinimumSize(int width, int height) { this->proxyWindow->setMinimumSize(width, height); }
void WindowBackend::getMinimumSize(int *width, int *height) { this->proxyWindow->getMinimumSize(width, height); }

void WindowBackend::setMaximumSize(int width, int height) { this->proxyWindow->setMaximumSize(width, height); }
void WindowBackend::getMaximumSize(int *width, int *height) { this->proxyWindow->getMaximumSize(width, height); }

void WindowBackend::focus() { this->proxyWindow->focus(); }

void WindowBackend::restore() { this->proxyWindow->restore(); }

void WindowBackend::maximize() { this->proxyWindow->maximize(); }

void WindowBackend::minimize() { this->proxyWindow->minimize(); }

intptr_t WindowBackend::getNativePtr() const { return this->proxyWindow->getNativePtr(); }
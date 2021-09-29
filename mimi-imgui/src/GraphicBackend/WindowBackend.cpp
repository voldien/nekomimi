#include "WindowBackend.h"

#include <GL/glew.h>
#include <Logging.h>
#include <SDL2/SDL_vulkan.h>
#include <algorithm>
#include <bits/exception.h>
#include <ext/alloc_traits.h>
#include <fmt/format.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/imgui.h>
#include <implot.h>
#include <spdlog/logger.h>
#include <stdexcept>
#include <stdint.h>
#include <vulkan/vulkan_core.h>

#include "Exception/IException.h"
#include "Exception/RuntimeException.h"
#include "ResourceManager.h"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_stdinc.h"
#include "VKDevice.h"
#include "VKUtil.h"
#include "VkPhysicalDevice.h"
#include "VulkanCore.h"
#include "imtui/imtui-impl-ncurses.h"
#include "imtui/imtui-impl-text.h"


const char *WindowBackend::getGfxBackEndSymbol(GfxBackEnd v) noexcept {
	switch (v) {
	case GfxBackEnd::ImGUI_OpenGL:
		return "OpenGL";
	case GfxBackEnd::ImGUI_Vulkan:
		return "Vulkan";
	case GfxBackEnd::ImGUI_Terminal:
		return "Terminal";
	default:
		assert(0);
		return "";
	}
}

WindowBackend::WindowBackend(GfxBackEnd backend) { initGfx(backend); }

WindowBackend::~WindowBackend(void) { releaseRender(); }

void WindowBackend::releaseRender(void) {
	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Terminal:
		ImTui_ImplText_Shutdown();
		ImTui_ImplNcurses_Shutdown();
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		ImGui_ImplOpenGL3_Shutdown();
		SDL_GL_DeleteContext(gl_context);
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
		SDL_DestroyWindow(gfxWindow);
		ImGui_ImplSDL2_Shutdown();
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
		break;
	default:
		break;
	}

	ImGui::DestroyContext();
}

void WindowBackend::initGfx(GfxBackEnd backend) {

	this->gfxBackend = backend;
	Logger::log()->info("Initilize UI Graphic with Backend Graphic: {}", backend);

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
		SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
	}

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

		ImFontConfig config;
		config.MergeMode = true;
		// io.Fonts->AddFontFromFileTTF("Verdana.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese()));
		// TODO relocate
		std::string fontPath = ResourceManager::getResourcePath("NotoSansCJKjp-Medium.otf");
		io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
		io.Fonts->Build();
	}

	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Terminal:
		initTerminal();
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		initOpenGL();
		break;
	case GfxBackEnd::ImGUI_Vulkan:
		initVulkan();
		break;
	default:
		break;
	}

	// TODO relocate to the inherit class
	ImPlot::CreateContext();
}

void WindowBackend::initTerminal(void) {
	this->imtuiScreen = ImTui_ImplNcurses_Init(true);
	ImTui_ImplText_Init();
}

void WindowBackend::initVulkan(void) {

	int width = 800;
	int height = 600;

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
													 SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_INPUT_FOCUS);
	gfxWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

	SDL_SetWindowMinimumSize(gfxWindow, 800, 600);

	this->vkCore = std::make_shared<VulkanCore>();
	this->vkPhysicalDevices = this->vkCore->createPhysicalDevices();
	this->vkDevice = std::make_shared<VKDevice>(vkPhysicalDevices);

	/*  Create surface. */
	bool surfaceResult = SDL_Vulkan_CreateSurface(this->gfxWindow, vkCore->getHandle(), &wd.Surface);
	if (surfaceResult == SDL_FALSE) {
		throw RuntimeException("failed create vulkan surface - {}", SDL_GetError());
	}

	/*	Check if any of the gpu devices and which of their queue support present.	*/
	int gpu_index = -1;
	for (unsigned int i = 0; i < this->vkDevice->getNrPhysicalDevices(); i++) {
		for (unsigned int j = 0; j < this->vkDevice->getPhysicalDevice(i)->getNrQueueFamilyProperties(); j++) {

			if (this->vkDevice->getPhysicalDevice(i)->isPresentable(wd.Surface, j)) {
				gpu_index = i;
			}
		}
	}
	assert(gpu_index < (int)this->vkDevice->getNrPhysicalDevices());

	if (gpu_index == -1)
		throw RuntimeException("No Supported GPU device that are presentable");

	const std::shared_ptr<PhysicalDevice> &gpuDevice = this->vkDevice->getPhysicalDevice(gpu_index);

	const std::vector<VkFormat> requestFormat = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SRGB};
	const std::vector<VkPresentModeKHR> requestPresent = {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_KHR,
														  VK_PRESENT_MODE_FIFO_RELAXED_KHR,
														  VK_PRESENT_MODE_IMMEDIATE_KHR};

	wd.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(gpuDevice->getHandle(), wd.Surface, requestFormat.data(),
															 requestFormat.size(), VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
	wd.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(gpuDevice->getHandle(), wd.Surface, requestPresent.data(),
														 requestPresent.size());

	ImGui_ImplVulkanH_CreateOrResizeWindow(this->vkCore->getHandle(), gpuDevice->getHandle(), vkDevice->getHandle(),
										   &wd, vkDevice->getDefaultGraphicQueueIndex(), nullptr, width, height, 2);

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
	vkCreateDescriptorPool(vkDevice->getHandle(), &pool_info, nullptr, &desc_pool);

	// Setup Platform/Renderer backends
	if (!ImGui_ImplSDL2_InitForVulkan(gfxWindow)) {
		throw RuntimeException("Failed init ImGUI SDL - Vulkan");
	}
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = this->vkCore->getHandle();
	init_info.PhysicalDevice = gpuDevice->getHandle();
	init_info.Device = vkDevice->getHandle();
	init_info.QueueFamily = vkDevice->getDefaultGraphicQueueIndex();
	init_info.Queue = vkDevice->getDefaultGraphicQueue();
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = desc_pool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = wd.ImageCount;
	init_info.CheckVkResultFn = nullptr;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	if (!ImGui_ImplVulkan_Init(&init_info, wd.RenderPass)) {
		throw RuntimeException("Failed init ImGUI Vulkan");
	}

	VkCommandPool command_pool = wd.Frames[wd.FrameIndex].CommandPool;
	VkCommandBuffer command_buffer = wd.Frames[wd.FrameIndex].CommandBuffer;

	VKS_VALIDATE(vkResetCommandPool(vkDevice->getHandle(), command_pool, 0));

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	VKS_VALIDATE(vkBeginCommandBuffer(command_buffer, &begin_info));

	if (!ImGui_ImplVulkan_CreateFontsTexture(command_buffer)) {
		throw RuntimeException("Failed to generate font ImGUI Vulkan");
	}

	VkSubmitInfo end_info = {};
	end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	end_info.commandBufferCount = 1;
	end_info.pCommandBuffers = &command_buffer;
	VKS_VALIDATE(vkEndCommandBuffer(command_buffer));

	VKS_VALIDATE(vkQueueSubmit(vkDevice->getDefaultGraphicQueue(), 1, &end_info, VK_NULL_HANDLE));

	VKS_VALIDATE(vkDeviceWaitIdle(vkDevice->getHandle()));

	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void WindowBackend::initOpenGL(void) {
	// set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	std::string glsl_version = "";
#ifdef __APPLE__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 150";
	SDL_GL_SetAttribute( // required on Mac OS
		SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#elif __linux__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 330";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
	// GL 3.0 + GLSL 130
	glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

	int width = 800;
	int height = 600;

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
													 SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_INPUT_FOCUS);
	gfxWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

	SDL_SetWindowMinimumSize(gfxWindow, 800, 600);

	gl_context = SDL_GL_CreateContext(gfxWindow);
	SDL_GL_MakeCurrent(gfxWindow, gl_context);

	// enable VSync
	SDL_GL_SetSwapInterval(1);

	glViewport(0, 0, width, height);

	if (glewInit() != GLEW_OK) {
		throw RuntimeException("GLEW Failed");
	}

	ImGui_ImplSDL2_InitForOpenGL(gfxWindow, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

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

void WindowBackend::beginRender(void) {

	if (gfxBackend == ImGUI_Vulkan || gfxBackend == ImGUI_OpenGL) {

		int windowWidth;
		int windowHeight;

		bool requestQuit = false;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// without it you won't have keyboard input and other things
			ImGui_ImplSDL2_ProcessEvent(&event);
			// you might also want to check io.WantCaptureMouse and io.WantCaptureKeyboard
			// before processing events

			switch (event.type) {
			case SDL_QUIT:
				requestQuit = false;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					windowWidth = event.window.data1;
					windowHeight = event.window.data2;
					// std::cout << "[INFO] Window size: "
					//           << windowWidth
					//           << "x"
					//           << windowHeight
					//           << std::endl;
					glViewport(0, 0, windowWidth, windowHeight);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					requestQuit = true;
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

	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Terminal:
		ImTui_ImplNcurses_NewFrame();
		ImTui_ImplText_NewFrame();
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(gfxWindow);
		break;
	case GfxBackEnd::ImGUI_Vulkan:
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplSDL2_NewFrame(gfxWindow);
		break;
	default:
		break;
	}
}
void WindowBackend::endRender(void) {
	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	switch (gfxBackend) {
	case GfxBackEnd::ImGUI_Terminal:
		ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), this->imtuiScreen);
		ImTui_ImplNcurses_DrawScreen();
		break;
	case GfxBackEnd::ImGUI_OpenGL:
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(gfxWindow);
		break;
	case GfxBackEnd::ImGUI_Vulkan: {
		VkResult err;

		VkSemaphore image_acquired_semaphore = wd.FrameSemaphores[wd.SemaphoreIndex].ImageAcquiredSemaphore;
		VkSemaphore render_complete_semaphore = wd.FrameSemaphores[wd.SemaphoreIndex].RenderCompleteSemaphore;
		err = vkAcquireNextImageKHR(vkDevice->getHandle(), wd.Swapchain, UINT64_MAX, image_acquired_semaphore,
									VK_NULL_HANDLE, &wd.FrameIndex);
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
			// g_SwapChainRebuild = true;
			return;
		}
		VKS_VALIDATE(err);

		ImGui_ImplVulkanH_Frame *fd = &wd.Frames[wd.FrameIndex];
		{
			VKS_VALIDATE(vkWaitForFences(vkDevice->getHandle(), 1, &fd->Fence, VK_TRUE,
										 UINT64_MAX)); // wait indefinitely instead of periodically checking

			VKS_VALIDATE(vkResetFences(vkDevice->getHandle(), 1, &fd->Fence));
		}
		{
			VKS_VALIDATE(vkResetCommandPool(vkDevice->getHandle(), fd->CommandPool, 0));

			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			VKS_VALIDATE(vkBeginCommandBuffer(fd->CommandBuffer, &info));
		}
		{
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = wd.RenderPass;
			info.framebuffer = fd->Framebuffer;
			info.renderArea.extent.width = wd.Width;
			info.renderArea.extent.height = wd.Height;
			info.clearValueCount = 1;
			info.pClearValues = &wd.ClearValue;
			vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}

		// Record dear imgui primitives into command buffer
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);

		// Submit command buffer
		vkCmdEndRenderPass(fd->CommandBuffer);
		{
			VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			VkSubmitInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &image_acquired_semaphore;
			info.pWaitDstStageMask = &wait_stage;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &fd->CommandBuffer;
			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = &render_complete_semaphore;

			err = vkEndCommandBuffer(fd->CommandBuffer);

			err = vkQueueSubmit(vkDevice->getDefaultGraphicQueue(), 1, &info, fd->Fence);
		}

		render_complete_semaphore = wd.FrameSemaphores[wd.SemaphoreIndex].RenderCompleteSemaphore;
		VkPresentInfoKHR info = {};
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &render_complete_semaphore;
		info.swapchainCount = 1;
		info.pSwapchains = &wd.Swapchain;
		info.pImageIndices = &wd.FrameIndex;
		err = vkQueuePresentKHR(vkDevice->getDefaultGraphicQueue(), &info);
		if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
			// g_SwapChainRebuild = true;
			return;
		}
		VKS_VALIDATE(err);

		wd.SemaphoreIndex = (wd.SemaphoreIndex + 1) % wd.ImageCount; // Now we can use the next set of semaphores
	} break;
	default:
		break;
	}
}
void WindowBackend::show() noexcept { SDL_ShowWindow(this->gfxWindow); }

void WindowBackend::hide() noexcept { SDL_HideWindow(this->gfxWindow); }

void WindowBackend::close(void) noexcept {
	this->hide();
	SDL_DestroyWindow(this->gfxWindow);
}

void WindowBackend::setPosition(int x, int y) noexcept { SDL_SetWindowPosition(this->gfxWindow, x, y); }

void WindowBackend::setSize(int width, int height) noexcept {
	/*	TODO determine if it shall update framebuffera as well.	*/
	SDL_SetWindowSize(this->gfxWindow, width, height);
}

void WindowBackend::getPosition(int *x, int *y) const { SDL_GetWindowPosition(this->gfxWindow, x, y); }

void WindowBackend::getSize(int *width, int *height) const { SDL_GetWindowSize(this->gfxWindow, width, height); }
void WindowBackend::setTitle(const std::string &title) { SDL_SetWindowTitle(gfxWindow, title.c_str()); }

std::string WindowBackend::getTitle(void) { return SDL_GetWindowTitle(gfxWindow); }

int WindowBackend::x(void) const noexcept {
	int x, y;
	SDL_GetWindowPosition(this->gfxWindow, &x, &y);
	return x;
}
int WindowBackend::y(void) const noexcept {
	int x, y;
	SDL_GetWindowPosition(this->gfxWindow, &x, &y);
	return y;
}
void WindowBackend::resizable(bool resizable) noexcept { SDL_SetWindowResizable(this->gfxWindow, (SDL_bool)resizable); }

void WindowBackend::setFullScreen(bool fullscreen) {
	// TODO add option for using either of the modes.
	if (fullscreen)
		SDL_SetWindowFullscreen(this->gfxWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(this->gfxWindow, 0);
}

bool WindowBackend::isFullScreen(void) const { return false; }

void WindowBackend::setBordered(bool bordered) { SDL_SetWindowBordered(this->gfxWindow, (SDL_bool)bordered); }

int WindowBackend::width(void) const noexcept {
	int w, h;
	getSize(&w, &h);
	return w;
}
int WindowBackend::height(void) const noexcept {
	int w, h;
	getSize(&w, &h);
	return h;
}

void WindowBackend::setMinimumSize(int width, int height) { SDL_SetWindowMinimumSize(this->gfxWindow, width, height); }
void WindowBackend::getMinimumSize(int *width, int *height) {
	SDL_GetWindowMinimumSize(this->gfxWindow, width, height);
}

void WindowBackend::setMaximumSize(int width, int height) { SDL_SetWindowMaximumSize(this->gfxWindow, width, height); }
void WindowBackend::getMaximumSize(int *width, int *height) {
	SDL_GetWindowMaximumSize(this->gfxWindow, width, height);
}

void WindowBackend::focus(void) { SDL_SetWindowInputFocus(this->gfxWindow); }

void WindowBackend::restore(void) { SDL_RestoreWindow(this->gfxWindow); }

void WindowBackend::maximize(void) { SDL_MaximizeWindow(this->gfxWindow); }

void WindowBackend::minimize(void) { SDL_MinimizeWindow(this->gfxWindow); }
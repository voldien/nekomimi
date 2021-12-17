#pragma once
#include <Exception.hpp>
#include <observable/observable.hpp>

namespace MIMIIMGUI {


	/**
	 * @brief
	 *
	 */
	enum class GfxBackEnd {
		ImGUI_Terminal,
		ImGUI_OpenGL,
		ImGUI_Vulkan,
		ImGUI_DirectX9,
		ImGUI_DirectX10,
		ImGUI_DirectX11,
		ImGUI_DirectX12
	};

	/**
	 * @brief
	 *
	 */
	enum class WindowLibBackend { WindowBackendSDL2, WindowBackendGLFW3, WindowBackendWindows };

}

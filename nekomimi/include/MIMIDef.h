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
#pragma once
#include <Exception.hpp>
#include <observable/observable.hpp>

namespace nekomimi {


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

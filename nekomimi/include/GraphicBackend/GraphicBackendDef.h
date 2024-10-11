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
#pragma once
#include <Exception.hpp>
#include <observable/observable.hpp>

namespace nekomimi {
	/**
	 * @brief
	 *
	 */
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

} // namespace nekomimi

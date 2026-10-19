/*
 *	NekoMimi - Window ImGUI Framework
 *	Copyright (c) 2025  Valdemar Lindberg
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
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Core/Property.h"
#include "magic_enum/magic_enum.hpp"
#include <ctime>
#include <functional>
#include <imgui.h>
#include <string>

namespace nekomimi {
	/**
	 * @brief
	 */
	class ImGuiUtil {
	  public:
		template <typename T> static void draw_property_override(fragcore::Property<T> &property) noexcept {
			bool override = property.is_override();
			ImGui::PushID(property.get_id());

			if (ImGui::Checkbox("Override", &override)) {
				property.override(override);
			}

			ImGui::PopID();
		}

		template <typename T>
		static void enumComboBox(const char *label, const T &currentSelected, const size_t maxEnums,
								 std::function<void(const T selected)> onSelected, const ImGuiComboFlags flags = 0) {

			const size_t item_selected_idx = static_cast<size_t>(currentSelected);

			const std::string combo_preview_value = std::string(magic_enum::enum_name(currentSelected));

			if (ImGui::BeginCombo(label, combo_preview_value.c_str(), flags)) {
				for (size_t nth_enum = 0; nth_enum < maxEnums; nth_enum++) {
					const bool is_selected = (item_selected_idx == nth_enum);

					const T curr_enum = static_cast<T>(nth_enum);
					if (ImGui::Selectable(magic_enum::enum_name(curr_enum).data(), is_selected)) {
						onSelected(curr_enum);
					}

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}

		template <typename T = const char *>
		static void ComboBoxStr(const char *label, const T *selectables, const size_t nrElements,
								const size_t currentSelected, std::function<void(const T *selected)> onSelected,
								const ImGuiComboFlags flags = 0) {

			// const size_t item_selected_idx = static_cast<size_t>(currentSelected);
			const T *combo_preview_value = selectables[currentSelected];

			if (ImGui::BeginCombo(label, combo_preview_value, flags)) {
				for (size_t nth_enum = 0; nth_enum < nrElements; nth_enum++) {
					/*  */
					const bool is_selected = (currentSelected == selectables[nth_enum]);

					if (ImGui::Selectable(selectables[nth_enum], is_selected)) {
						onSelected(selectables[nth_enum]);
					}

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}

		template <typename T>
		static void ListComboBox(const char *label, const T *selectables, const size_t nrElements,
								 const size_t currentSelected, std::function<void(const T *selected)> onSelected) {

			// const size_t item_selected_idx = static_cast<size_t>(currentSelected);
			const T *combo_preview_value = selectables[currentSelected];

			const ImGuiComboFlags flags = 0;
			ImGui::SetNextItemWidth(256);
			if (ImGui::BeginCombo(label, combo_preview_value, flags)) {
				for (size_t index = 0; index < nrElements; index++) {
					const bool is_selected = (currentSelected == selectables[index]);

					if (ImGui::Selectable(selectables[index], is_selected)) {
						onSelected(selectables[index]);
					}

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}

		template <typename T>
		static void enumPopUp(const char *label, const T &currentSelected, const size_t maxEnums,
							  std::function<void(const T selected)> onSelected, const ImGuiPopupFlags flags = 0,
							  std::function<void()> onPopUp = {}) noexcept {

			/*	*/
			if (ImGui::BeginPopup(label)) {
				for (size_t nth_enum = 0; nth_enum < maxEnums; nth_enum++) {
					const bool is_selected = (currentSelected == nth_enum);

					const T curr_enum = static_cast<T>(nth_enum);

					if (ImGui::Selectable(magic_enum::enum_name(curr_enum).data(), is_selected)) {
						onSelected(curr_enum);
					}
				}
				ImGui::EndPopup();
			}
		}

		template <typename T>
		static void arrayPopUp(const char *label, const T &currentSelected, const T *selectables,
							   const size_t nrElements, std::function<void(const T &selected)> onSelected,
							   const ImGuiPopupFlags flags = 0) noexcept {

			if (ImGui::BeginPopup(label)) {
				for (size_t index = 0; index < nrElements; index++) {

					const bool is_selected = (currentSelected == selectables[index]);

					if (ImGui::Selectable(selectables[selectables], is_selected)) {
						onSelected(index);
					}
				}
				ImGui::EndPopup();
			}
		}
	};

} // namespace nekomimi

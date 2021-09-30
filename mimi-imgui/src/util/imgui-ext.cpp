#include "imgui-ext.h"

#include <imgui_internal.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "imgui.h"

using namespace ImGui;

void UIUtilHelper::HelpMarker(const char *desc) {
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

// https://github.com/ocornut/imgui/issues/1901
bool UIUtilHelper::BufferingBar(const char *label, float value, const ImVec2 &size_arg, const ImU32 &bg_col,
								const ImU32 &fg_col) {
	ImGuiWindow *window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext &g = *GImGui;
	const ImGuiStyle &style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = size_arg;
	size.x -= style.FramePadding.x * 2;

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	// Render
	const float circleStart = size.x * 0.7f;
	const float circleEnd = size.x;
	const float circleWidth = circleEnd - circleStart;

	window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
	window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

	const float t = g.Time;
	const float r = size.y / 2;
	const float speed = 1.5f;

	const float a = speed * 0;
	const float b = speed * 0.333f;
	const float c = speed * 0.666f;

	const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
	const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
	const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

	window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
	window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
	window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

	return true;
}

// https://github.com/ocornut/imgui/issues/1901
bool UIUtilHelper::Spinner(const char *label, float radius, int thickness, const ImU32 &color) {
	ImGuiWindow *window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext &g = *GImGui;
	const ImGuiStyle &style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size((radius)*2, (radius + style.FramePadding.y) * 2);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	// Render
	window->DrawList->PathClear();

	int num_segments = 30;
	int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

	const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

	const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

	for (int i = 0; i < num_segments; i++) {
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		window->DrawList->PathLineTo(
			ImVec2(centre.x + ImCos(a + g.Time * 8) * radius, centre.y + ImSin(a + g.Time * 8) * radius));
	}

	window->DrawList->PathStroke(color, false, thickness);

	return true;
}

void UIUtilHelper::ToggleButton(const char *str_id, bool *v) {
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList *draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * 1.55f;
	float radius = height * 0.50f;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	if (ImGui::IsItemClicked())
		*v = !*v;

	float t = *v ? 1.0f : 0.0f;

	ImGuiContext &g = *GImGui;
	float ANIM_SPEED = 0.08f;
	if (g.LastActiveId == g.CurrentWindow->GetID(str_id)) // && g.LastActiveIdTimer < ANIM_SPEED)
	{
		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = *v ? (t_anim) : (1.0f - t_anim);
	}

	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
	else
		col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f,
							   IM_COL32(255, 255, 255, 255));
}

// https://github.com/ocornut/imgui/issues/1658
bool UIUtilHelper::ComboFilter__DrawPopup(ComboFilterState &state, int START, const char **ENTRIES, int ENTRY_COUNT) {
	using namespace ImGui;
	bool clicked = 0;

	// Grab the position for the popup
	ImVec2 pos = GetItemRectMin();
	pos.y += GetItemRectSize().y;
	ImVec2 size = ImVec2(GetItemRectSize().x - 60, ImGui::GetFrameHeightWithSpacing() * 4);

	PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
							 ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings |
							 0; // ImGuiWindowFlags_ShowBorders;

	SetNextWindowFocus();

	SetNextWindowPos(pos);
	SetNextWindowSize(size);
	Begin("##combo_filter", nullptr, flags);

	PushAllowKeyboardFocus(false);

	for (int i = 0; i < ENTRY_COUNT; i++) {
		// Track if we're drawing the active index so we
		// can scroll to it if it has changed
		bool isIndexActive = state.activeIdx == i;

		if (isIndexActive) {
			// Draw the currently 'active' item differently
			// ( used appropriate colors for your own style )
			PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 0, 1));
		}

		PushID(i);
		if (Selectable(ENTRIES[i], isIndexActive)) {
			// And item was clicked, notify the input
			// callback so that it can modify the input buffer
			state.activeIdx = i;
			clicked = 1;
		}
		if (IsItemFocused() && IsKeyPressed(GetIO().KeyMap[ImGuiKey_Enter])) {
			// Allow ENTER key to select current highlighted item (w/ keyboard navigation)
			state.activeIdx = i;
			clicked = 1;
		}
		PopID();

		if (isIndexActive) {
			if (state.selectionChanged) {
				// Make sure we bring the currently 'active' item into view.
				// SetScrollHere();
				state.selectionChanged = false;
			}

			PopStyleColor(1);
		}
	}

	PopAllowKeyboardFocus();
	End();
	PopStyleVar(1);

	return clicked;
}

bool UIUtilHelper::ComboFilter(const char *id, char *buffer, int bufferlen, const char **hints, int num_hints,
							   ComboFilterState &s) {
	struct fuzzy {
		static int score(const char *str1, const char *str2) {
			int score = 0, consecutive = 0, maxerrors = 0;
			while (*str1 && *str2) {
				int is_leading = (*str1 & 64) && !(str1[1] & 64);
				if ((*str1 & ~32) == (*str2 & ~32)) {
					int had_separator = (str1[-1] <= 32);
					int x = had_separator || is_leading ? 10 : consecutive * 5;
					consecutive = 1;
					score += x;
					++str2;
				} else {
					int x = -1, y = is_leading * -3;
					consecutive = 0;
					score += x;
					maxerrors += y;
				}
				++str1;
			}
			return score + (maxerrors < -9 ? -9 : maxerrors);
		}
		static int search(const char *str, int num, const char *words[]) {
			int scoremax = 0;
			int best = -1;
			for (int i = 0; i < num; ++i) {
				int score = fuzzy::score(words[i], str);
				int record = (score >= scoremax);
				int draw = (score == scoremax);
				if (record) {
					scoremax = score;
					if (!draw)
						best = i;
					else
						best = best >= 0 && strlen(words[best]) < strlen(words[i]) ? best : i;
				}
			}
			return best;
		}
	};
	using namespace ImGui;
	bool done =
		InputText(id, buffer, bufferlen, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
	bool hot = s.activeIdx >= 0 && strcmp(buffer, hints[s.activeIdx]);
	if (hot) {
		int new_idx = fuzzy::search(buffer, num_hints, hints);
		int idx = new_idx >= 0 ? new_idx : s.activeIdx;
		s.selectionChanged = s.activeIdx != idx;
		s.activeIdx = idx;
		if (done || ComboFilter__DrawPopup(s, idx, hints, num_hints)) {
			int i = s.activeIdx;
			if (i >= 0) {
				strcpy(buffer, hints[i]);
				done = true;
			}
		}
	}
	return done;
}

bool UIUtilHelper::MyKnob(const char *label, float *p_value, float v_min, float v_max) {
	ImGuiIO &io = ImGui::GetIO();
	ImGuiStyle &style = ImGui::GetStyle();

	float radius_outer = 20.0f;
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
	float line_height = ImGui::GetTextLineHeight();
	ImDrawList *draw_list = ImGui::GetWindowDrawList();

	float ANGLE_MIN = 3.141592f * 0.75f;
	float ANGLE_MAX = 3.141592f * 2.25f;

	ImGui::InvisibleButton(label, ImVec2(radius_outer * 2, radius_outer * 2 + line_height + style.ItemInnerSpacing.y));
	bool value_changed = false;
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemActive();
	if (is_active && io.MouseDelta.x != 0.0f) {
		float step = (v_max - v_min) / 200.0f;
		*p_value += io.MouseDelta.x * step;
		if (*p_value < v_min)
			*p_value = v_min;
		if (*p_value > v_max)
			*p_value = v_max;
		value_changed = true;
	}

	float t = (*p_value - v_min) / (v_max - v_min);
	float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t;
	float angle_cos = cosf(angle), angle_sin = sinf(angle);
	float radius_inner = radius_outer * 0.40f;
	draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
	draw_list->AddLine(ImVec2(center.x + angle_cos * radius_inner, center.y + angle_sin * radius_inner),
					   ImVec2(center.x + angle_cos * (radius_outer - 2), center.y + angle_sin * (radius_outer - 2)),
					   ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
	draw_list->AddCircleFilled(center, radius_inner,
							   ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive
															: is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg),
							   16);
	draw_list->AddText(ImVec2(pos.x, pos.y + radius_outer * 2 + style.ItemInnerSpacing.y),
					   ImGui::GetColorU32(ImGuiCol_Text), label);

	if (is_active || is_hovered) {
		ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x,
									   pos.y - line_height - style.ItemInnerSpacing.y - style.WindowPadding.y));
		ImGui::BeginTooltip();
		ImGui::Text("%.3f", *p_value);
		ImGui::EndTooltip();
	}

	return value_changed;
}

// // https://github.com/ocornut/imgui/issues/942
// // Implementing a simple custom widget using the public API.
// // You may also use the <imgui_internal.h> API to get raw access to more data/helpers, however the internal API
// // isn't guaranteed to be forward compatible.
// // FIXME: Need at least proper label centering + clipping (internal functions RenderTextClipped provides both but
// // api is flaky/temporary)
// bool ImGui::MyKnob(const char *label, float *p_value, float v_min, float v_max) {
// 	ImGuiIO &io = ImGui::GetIO();
// 	ImGuiStyle &style = ImGui::GetStyle();

// 	float radius_outer = 20.0f;
// 	ImVec2 pos = ImGui::GetCursorScreenPos();
// 	ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
// 	float line_height = ImGui::GetTextLineHeight();
// 	ImDrawList *draw_list = ImGui::GetWindowDrawList();

// 	float ANGLE_MIN = 3.141592f * 0.75f;
// 	float ANGLE_MAX = 3.141592f * 2.25f;

// 	ImGui::InvisibleButton(label, ImVec2(radius_outer * 2, radius_outer * 2 + line_height + style.ItemInnerSpacing.y));
// 	bool value_changed = false;
// 	bool is_active = ImGui::IsItemActive();
// 	bool is_hovered = ImGui::IsItemActive();
// 	if (is_active && io.MouseDelta.x != 0.0f) {
// 		float step = (v_max - v_min) / 200.0f;
// 		*p_value += io.MouseDelta.x * step;
// 		if (*p_value < v_min)
// 			*p_value = v_min;
// 		if (*p_value > v_max)
// 			*p_value = v_max;
// 		value_changed = true;
// 	}

// 	float t = (*p_value - v_min) / (v_max - v_min);
// 	float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t;
// 	float angle_cos = cosf(angle), angle_sin = sinf(angle);
// 	float radius_inner = radius_outer * 0.40f;
// 	draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
// 	draw_list->AddLine(ImVec2(center.x + angle_cos * radius_inner, center.y + angle_sin * radius_inner),
// 					   ImVec2(center.x + angle_cos * (radius_outer - 2), center.y + angle_sin * (radius_outer - 2)),
// 					   ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
// 	draw_list->AddCircleFilled(center, radius_inner,
// 							   ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive
// 															: is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg),
// 							   16);
// 	draw_list->AddText(ImVec2(pos.x, pos.y + radius_outer * 2 + style.ItemInnerSpacing.y),
// 					   ImGui::GetColorU32(ImGuiCol_Text), label);

// 	if (is_active || is_hovered) {
// 		ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x,
// 									   pos.y - line_height - style.ItemInnerSpacing.y - style.WindowPadding.y));
// 		ImGui::BeginTooltip();
// 		ImGui::Text("%.3f", *p_value);
// 		ImGui::EndTooltip();
// 	}

// 	return value_changed;
// }

float UIUtilHelper::BezierValue(float dt01, float P[4]) {
	enum { STEPS = 256 };
	ImVec2 Q[4] = {{0, 0}, {P[0], P[1]}, {P[2], P[3]}, {1, 1}};
	ImVec2 results[STEPS + 1];
	bezier_table<STEPS>(Q, results);
	return results[(int)((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
}

int UIUtilHelper::Bezier(const char *label, float P[5]) {
	// visuals
	enum { SMOOTHNESS = 64 };		  // curve smoothness: the higher number of segments, the smoother curve
	enum { CURVE_WIDTH = 4 };		  // main curved line width
	enum { LINE_WIDTH = 1 };		  // handlers: small lines width
	enum { GRAB_RADIUS = 8 };		  // handlers: circle radius
	enum { GRAB_BORDER = 2 };		  // handlers: circle border width
	enum { AREA_CONSTRAINED = true }; // should grabbers be constrained to grid area?
	enum { AREA_WIDTH = 128 };		  // area width in pixels. 0 for adaptive size (will use max avail width)

	// curve presets
	static struct {
		const char *name;
		float points[4];
	} presets[] = {
		{"Linear", 0.000f, 0.000f, 1.000f, 1.000f},

		{"In Sine", 0.470f, 0.000f, 0.745f, 0.715f},	 {"In Quad", 0.550f, 0.085f, 0.680f, 0.530f},
		{"In Cubic", 0.550f, 0.055f, 0.675f, 0.190f},	 {"In Quart", 0.895f, 0.030f, 0.685f, 0.220f},
		{"In Quint", 0.755f, 0.050f, 0.855f, 0.060f},	 {"In Expo", 0.950f, 0.050f, 0.795f, 0.035f},
		{"In Circ", 0.600f, 0.040f, 0.980f, 0.335f},	 {"In Back", 0.600f, -0.28f, 0.735f, 0.045f},

		{"Out Sine", 0.390f, 0.575f, 0.565f, 1.000f},	 {"Out Quad", 0.250f, 0.460f, 0.450f, 0.940f},
		{"Out Cubic", 0.215f, 0.610f, 0.355f, 1.000f},	 {"Out Quart", 0.165f, 0.840f, 0.440f, 1.000f},
		{"Out Quint", 0.230f, 1.000f, 0.320f, 1.000f},	 {"Out Expo", 0.190f, 1.000f, 0.220f, 1.000f},
		{"Out Circ", 0.075f, 0.820f, 0.165f, 1.000f},	 {"Out Back", 0.175f, 0.885f, 0.320f, 1.275f},

		{"InOut Sine", 0.445f, 0.050f, 0.550f, 0.950f},	 {"InOut Quad", 0.455f, 0.030f, 0.515f, 0.955f},
		{"InOut Cubic", 0.645f, 0.045f, 0.355f, 1.000f}, {"InOut Quart", 0.770f, 0.000f, 0.175f, 1.000f},
		{"InOut Quint", 0.860f, 0.000f, 0.070f, 1.000f}, {"InOut Expo", 1.000f, 0.000f, 0.000f, 1.000f},
		{"InOut Circ", 0.785f, 0.135f, 0.150f, 0.860f},	 {"InOut Back", 0.680f, -0.55f, 0.265f, 1.550f},

		// easeInElastic: not a bezier
		// easeOutElastic: not a bezier
		// easeInOutElastic: not a bezier
		// easeInBounce: not a bezier
		// easeOutBounce: not a bezier
		// easeInOutBounce: not a bezier
	};

	// preset selector

	bool reload = 0;
	ImGui::PushID(label);
	if (ImGui::ArrowButton("##lt", ImGuiDir_Left)) { // ImGui::ArrowButton(ImGui::GetCurrentWindow()->GetID("##lt"),
													 // ImGuiDir_Left, ImVec2(0, 0), 0)
		if (--P[4] >= 0)
			reload = 1;
		else
			++P[4];
	}
	ImGui::SameLine();

	if (ImGui::Button("Presets")) {
		ImGui::OpenPopup("!Presets");
	}
	if (ImGui::BeginPopup("!Presets")) {
		for (int i = 0; i < IM_ARRAYSIZE(presets); ++i) {
			if (i == 1 || i == 9 || i == 17)
				ImGui::Separator();
			if (ImGui::MenuItem(presets[i].name, nullptr, P[4] == i)) {
				P[4] = i;
				reload = 1;
			}
		}
		ImGui::EndPopup();
	}
	ImGui::SameLine();

	if (ImGui::ArrowButton("##rt",
						   ImGuiDir_Right)) { // ImGui::ArrowButton(ImGui::GetCurrentWindow()->GetID("##rt"),
											  // ImGuiDir_Right, ImVec2(0, 0), 0)
		if (++P[4] < IM_ARRAYSIZE(presets))
			reload = 1;
		else
			--P[4];
	}
	ImGui::SameLine();
	ImGui::PopID();

	if (reload) {
		memcpy(P, presets[(int)P[4]].points, sizeof(float) * 4);
	}

	// bezier widget

	const ImGuiStyle &Style = GetStyle();
	ImDrawList *DrawList = GetWindowDrawList();
	ImGuiWindow *Window = GetCurrentWindow();
	if (Window->SkipItems)
		return false;

	// header and spacing
	int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
	int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?
	Dummy(ImVec2(0, 3));

	// prepare canvas
	const float avail = GetContentRegionAvailWidth();
	const float dim = AREA_WIDTH > 0 ? AREA_WIDTH : avail;
	ImVec2 Canvas(dim, dim);

	ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
	ItemSize(bb);
	if (!ItemAdd(bb, NULL))
		return changed;

	const ImGuiID id = Window->GetID(label);
	hovered |= 0 != ItemHoverable(ImRect(bb.Min, bb.Min + ImVec2(avail, dim)), id);

	RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);

	// background grid
	for (int i = 0; i <= Canvas.x; i += (Canvas.x / 4)) {
		DrawList->AddLine(ImVec2(bb.Min.x + i, bb.Min.y), ImVec2(bb.Min.x + i, bb.Max.y),
						  GetColorU32(ImGuiCol_TextDisabled));
	}
	for (int i = 0; i <= Canvas.y; i += (Canvas.y / 4)) {
		DrawList->AddLine(ImVec2(bb.Min.x, bb.Min.y + i), ImVec2(bb.Max.x, bb.Min.y + i),
						  GetColorU32(ImGuiCol_TextDisabled));
	}

	// eval curve
	ImVec2 Q[4] = {{0, 0}, {P[0], P[1]}, {P[2], P[3]}, {1, 1}};
	ImVec2 results[SMOOTHNESS + 1];
	bezier_table<SMOOTHNESS>(Q, results);

	// control points: 2 lines and 2 circles
	{
		// handle grabbers
		ImVec2 mouse = GetIO().MousePos, pos[2];
		float distance[2];

		for (int i = 0; i < 2; ++i) {
			pos[i] = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
			distance[i] = (pos[i].x - mouse.x) * (pos[i].x - mouse.x) + (pos[i].y - mouse.y) * (pos[i].y - mouse.y);
		}

		int selected = distance[0] < distance[1] ? 0 : 1;
		if (distance[selected] < (4 * GRAB_RADIUS * 4 * GRAB_RADIUS)) {
			SetTooltip("(%4.3f, %4.3f)", P[selected * 2 + 0], P[selected * 2 + 1]);

			if (/*hovered &&*/ (IsMouseClicked(0) || IsMouseDragging(0))) {
				float &px = (P[selected * 2 + 0] += GetIO().MouseDelta.x / Canvas.x);
				float &py = (P[selected * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y);

				if (AREA_CONSTRAINED) {
					px = (px < 0 ? 0 : (px > 1 ? 1 : px));
					py = (py < 0 ? 0 : (py > 1 ? 1 : py));
				}

				changed = true;
			}
		}
	}

	// if (hovered || changed) DrawList->PushClipRectFullScreen();

	// draw curve
	{
		ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
		for (int i = 0; i < SMOOTHNESS; ++i) {
			ImVec2 p = {results[i + 0].x, 1 - results[i + 0].y};
			ImVec2 q = {results[i + 1].x, 1 - results[i + 1].y};
			ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
			ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
			DrawList->AddLine(r, s, color, CURVE_WIDTH);
		}
	}

	// draw preview (cycles every 1s)
	static clock_t epoch = clock();
	ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
	for (int i = 0; i < 3; ++i) {
		double now = ((clock() - epoch) / (double)CLOCKS_PER_SEC);
		float delta = ((int)(now * 1000) % 1000) / 1000.f;
		delta += i / 3.f;
		if (delta > 1)
			delta -= 1;
		int idx = (int)(delta * SMOOTHNESS);
		float evalx = results[idx].x; //
		float evaly = results[idx].y; // ImGui::BezierValue( delta, P );
		ImVec2 p0 = ImVec2(evalx, 1 - 0) * (bb.Max - bb.Min) + bb.Min;
		ImVec2 p1 = ImVec2(0, 1 - evaly) * (bb.Max - bb.Min) + bb.Min;
		ImVec2 p2 = ImVec2(evalx, 1 - evaly) * (bb.Max - bb.Min) + bb.Min;
		DrawList->AddCircleFilled(p0, GRAB_RADIUS / 2, ImColor(white));
		DrawList->AddCircleFilled(p1, GRAB_RADIUS / 2, ImColor(white));
		DrawList->AddCircleFilled(p2, GRAB_RADIUS / 2, ImColor(white));
	}

	// draw lines and grabbers
	float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
	ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
	ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
	ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
	DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y), p1, ImColor(white), LINE_WIDTH);
	DrawList->AddLine(ImVec2(bb.Max.x, bb.Min.y), p2, ImColor(white), LINE_WIDTH);
	DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
	DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
	DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
	DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));

	// if (hovered || changed) DrawList->PopClipRect();

	return changed;
}

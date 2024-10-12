#ifndef _NEKO_MIMI_IMGUI_UI_UTIL_HELPER_H_
#define _NEKO_MIMI_IMGUI_UI_UTIL_HELPER_H_ 1
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <time.h>

/**
 * @brief a helper class with
 * various additional imgui features.
 *
 */
class UIUtilHelper {
  public:
	// https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
	static void HelpMarker(const char *desc);

	// https://github.com/ocornut/imgui/issues/1901
	static bool BufferingBar(const char *label, float value, const ImVec2 &size_arg, const ImU32 &bg_col,
							 const ImU32 &fg_col);

	// https://github.com/ocornut/imgui/issues/1901
	static bool Spinner(const char *label, float radius, int thickness, const ImU32 &color);
	static void ToggleButton(const char *str_id, bool *v);

	// https://github.com/ocornut/imgui/issues/1658
	struct ComboFilterState {
		int activeIdx;		   // Index of currently 'active' item by use of up/down keys
		bool selectionChanged; // Flag to help focus the correct item when selecting active item
	};

	static bool ComboFilter__DrawPopup(ComboFilterState &state, int START, const char **ENTRIES, int ENTRY_COUNT);

	static bool ComboFilter(const char *id, char *buffer, int bufferlen, const char **hints, int num_hints,
							ComboFilterState &s);

	// https://github.com/ocornut/imgui/issues/942
	// Implementing a simple custom widget using the public API.
	// You may also use the <imgui_internal.h> API to get raw access to more data/helpers, however the internal API
	// isn't guaranteed to be forward compatible.
	// FIXME: Need at least proper label centering + clipping (internal functions RenderTextClipped provides both but
	// api is flaky/temporary)
	static bool MyKnob(const char *label, float *p_value, float v_min, float v_max);
	// https://github.com/ocornut/imgui/issues/786
	template <int steps> static void bezier_table(ImVec2 P[4], ImVec2 results[steps + 1]) {
		static float C[(steps + 1) * 4], *K = 0;
		if (!K) {
			K = C;
			for (unsigned step = 0; step <= steps; ++step) {
				float t = (float)step / (float)steps;
				C[step * 4 + 0] = (1 - t) * (1 - t) * (1 - t); // * P0
				C[step * 4 + 1] = 3 * (1 - t) * (1 - t) * t;   // * P1
				C[step * 4 + 2] = 3 * (1 - t) * t * t;		   // * P2
				C[step * 4 + 3] = t * t * t;				   // * P3
			}
		}
		for (unsigned step = 0; step <= steps; ++step) {
			ImVec2 point = {K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x +
								K[step * 4 + 3] * P[3].x,
							K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y +
								K[step * 4 + 3] * P[3].y};
			results[step] = point;
		}
	}

	static float BezierValue(float dt01, float P[4]);
	static int Bezier(const char *label, float P[5]);
};

#endif
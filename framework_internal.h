#pragma once
#include <renderlib/imgui/imgui.h>

namespace ui
{
	class window;

	inline ImVec2 get_cursor()
	{
		return ImGui::GetMousePos();
	}
	inline bool key_pressed(int key)
	{
		return ImGui::IsKeyPressed(key);
	}
	inline bool key_down(int key)
	{
		return ImGui::IsKeyDown(key);
	}
	inline bool key_released(int key)
	{
		return ImGui::IsKeyReleased(key);
	}

	inline ImFont* font_title;
	inline ImFont* font_widgets;
	inline ImFont* font_tab;
	inline ImFont* font_icons;
	inline ImFont* font_menu_icons;
	inline ImFont* menuicons;
	inline ImFont* weapon_font;
	inline ImFont* weapon_icons;
	inline ImFont* test;
	inline void* radar_window_extern;
	inline void* window_device;
	inline window* main_window;
	inline int focused_item = -1;


}
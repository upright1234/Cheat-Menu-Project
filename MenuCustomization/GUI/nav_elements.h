#pragma once
#include <windows.h>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
using namespace ImGui;

namespace elements {
    bool tab(ImFont* tab_icons_font, bool selected, const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
    bool checkbox(const char* label, bool* v);
    bool beginchildex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool beginchild(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    void endchild();
    bool beginchildexTab(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool beginchildTab(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags);
    bool selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); 
    bool selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));     
    bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);    
    bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);
    void multi_combo(const char* label, bool combos[], std::vector<std::string> list);
    bool sliderint(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool sliderfloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    bool sliderscalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags = 0);
    bool coloredit4(ImFont* icon, const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    bool button(const char* label, const ImVec2& size = ImVec2(0, 0), bool border = true);
    bool buttonex(const char* label, const ImVec2& size_arg, bool border, ImGuiButtonFlags flags = 0);
    bool HotKey(const char* label, int* k, const ImVec2& size_arg);
    bool colorbutton(ImFont* icon, const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0, 0));
    bool begincombo(const char* label, const char* preview_value, int items_count, ImGuiComboFlags flags = 0);
    bool input_text_ex(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    bool input_text(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    ImVec2 CalcWindowExpectedSize(ImGuiWindow* window);
    ImVec2 CalcWindowContentSize(ImGuiWindow* window);
    ImRect GetWindowAllowedExtentRect(ImGuiWindow* window);
    ImVec2 CalcWindowSizeAfterConstraint(ImGuiWindow* window, ImVec2 new_size);
    ImVec2 CalcWindowAutoFitSize(ImGuiWindow* window, const ImVec2& size_contents);
    ImRect GetViewportRect();
    void Theme();
}





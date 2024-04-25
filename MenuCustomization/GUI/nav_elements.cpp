#include "nav_elements.h"
#include <unordered_map>
#include <imgui_impl_win32.h>

namespace colors {

    namespace utility {

        inline ImColor circle_user = ImColor(128, 0, 128, 150);
        inline ImColor logo_color = ImColor(128, 0, 128, 150);
    }

    namespace background {
        inline ImColor background_image = ImColor(14, 14, 14, 255);
        inline ImColor background = ImColor(15, 15, 15, 255);
        inline ImColor shadow = ImColor(50, 50, 50, 255);
        inline ImColor border = ImColor(20, 20, 20, 255);

    }

    namespace child {

        inline ImColor text = ImColor(140, 141, 145, 255);
        inline ImColor child = ImColor(0, 0, 0, 0);
        inline ImColor border = ImColor(20, 20, 20, 255);

    }

    namespace scroll {

        inline ImColor background = ImColor(0, 0, 0, 0);
        inline ImColor scroll = ImColor(20, 20, 20, 255);

    }

    namespace button {

        inline ImColor background_active = ImColor(20, 20, 20, 255);
        inline ImColor background_hov = ImColor(20, 25, 29, 0);
        inline ImColor background = ImColor(20, 25, 29, 0);

        inline ImColor text_active = ImColor(128, 0, 128, 255);
        inline ImColor text_hov = ImColor(71, 71, 73, 255);
        inline ImColor text = ImColor(71, 71, 73, 255);

    }

    namespace checkbox {

        inline ImColor background = ImColor(23, 24, 29, 255);

        inline ImColor circle_active = ImColor(128, 0, 128, 255);
        inline ImColor circle_inactive = ImColor(52, 58, 57, 255);

        inline ImColor text_active = ImColor(140, 141, 145, 255);
        inline ImColor text_hov = ImColor(75, 75, 77, 255);
        inline ImColor text = ImColor(65, 65, 67, 255);

    }

    namespace slider {

        inline ImColor background = ImColor(23, 24, 29, 255);

        inline ImColor gradient_start = ImColor(255, 0, 255, 255);
        inline ImColor gradient_end = ImColor(158, 0, 158, 255);

        inline ImColor circle_active = ImColor(128, 0, 128, 255);
        inline ImColor circle_inactive = ImColor(52, 58, 57, 255);

        inline ImColor text_active = ImColor(140, 141, 145, 255);
        inline ImColor text_hov = ImColor(75, 75, 77, 255);
        inline ImColor text = ImColor(65, 65, 67, 255);

    }

    namespace picker {

        inline ImColor background = ImColor(20, 20, 20, 255);

        inline ImColor shadow = ImColor(8, 8, 8, 255);
        inline ImColor border = ImColor(5, 5, 5, 255);

        inline ImColor text_active = ImColor(140, 141, 145, 255);
        inline ImColor text_hov = ImColor(75, 75, 77, 255);
        inline ImColor text = ImColor(65, 65, 67, 255);

    }

    namespace combo {

        inline ImColor background = ImColor(20, 20, 20, 255);
        inline ImColor circle = ImColor(128, 0, 128, 255);

        inline ImColor text_active = ImColor(140, 141, 145, 255);
        inline ImColor text_hov = ImColor(75, 75, 77, 255);
        inline ImColor text = ImColor(65, 65, 67, 255);

        namespace selectable {

            inline ImColor mark_active = ImColor(128, 0, 128, 255);

            inline ImColor text_active = ImColor(140, 141, 145, 255);
            inline ImColor text_hov = ImColor(75, 75, 77, 255);
            inline ImColor text = ImColor(65, 65, 67, 255);

        }

    }

    namespace keybind {

        inline ImColor key_text = ImColor(107, 107, 107, 255);
        inline ImColor color_rect = ImColor(24, 25, 27, 255);
        inline ImColor color_outline = ImColor(24, 25, 27, 255);

        inline ImColor key_rect = ImColor(24, 25, 27, 255);
        inline ImColor key_outline = ImColor(24, 25, 27, 255);

    }

}

bool             InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data, ImGuiInputSource input_source);
int              InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end);
ImVec2           InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining = NULL, ImVec2* out_offset = NULL, bool stop_on_new_line = false);

using namespace ImGui;
struct tab_state {

    ImVec4 background, text;
    float inflating;
};

std::map<ImGuiID, tab_state> anim_1;
bool elements::tab(ImFont* tab_icons_font, bool selected, const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);


    auto it_anim = anim_1.find(id);

    if (it_anim == anim_1.end())
    {
        anim_1.insert({ id, tab_state() });
        it_anim = anim_1.find(id);
    }
    it_anim->second.inflating = ImLerp(it_anim->second.inflating, selected ? 0.f : 20.f, g.IO.DeltaTime * 6.f);

    it_anim->second.background = ImLerp(it_anim->second.background, selected ? colors::button::background_active : hovered ? colors::button::background_hov : colors::button::background, g.IO.DeltaTime * 6.f);
    it_anim->second.text = ImLerp(it_anim->second.text, selected ? colors::button::text_active : hovered ? colors::button::text_hov : colors::button::text, g.IO.DeltaTime * 6.f);

    RenderFrame(bb.Min + ImVec2(it_anim->second.inflating, it_anim->second.inflating), bb.Max - ImVec2(it_anim->second.inflating, it_anim->second.inflating), GetColorU32(it_anim->second.background), true, 4.f);

    PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text));
    PushFont(tab_icons_font);
    RenderTextClipped(bb.Min - ImVec2(13, 7), bb.Max, label, NULL, &label_size, style.ButtonTextAlign, &bb);
    PopFont();
    PopStyleColor();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

struct checkbox_state {

    ImVec4 background, circle, text;
    float move_circle;

};
std::map<ImGuiID, checkbox_state> anim_2;
bool elements::checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    float size = GetWindowContentRegionMax().x - 50;

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? label_size.x + GetWindowContentRegionMax().x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (IsItemClicked())
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    auto it_anim = anim_2.find(id);

    if (it_anim == anim_2.end())
    {
        anim_2.insert({ id, checkbox_state() });
        it_anim = anim_2.find(id);
    }

    it_anim->second.text = ImLerp(it_anim->second.text, *v ? colors::checkbox::text_active : hovered ? colors::checkbox::text_hov : colors::checkbox::text, g.IO.DeltaTime * 6.f);
    it_anim->second.circle = ImLerp(it_anim->second.circle, *v ? colors::checkbox::circle_active : colors::checkbox::circle_inactive, g.IO.DeltaTime * 6.f);
    it_anim->second.move_circle = ImLerp(it_anim->second.move_circle, *v ? 17.f : 0.f, g.IO.DeltaTime * 12.f);

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));

    window->DrawList->AddRectFilled(check_bb.Min + ImVec2(size, 1), check_bb.Max + ImVec2(GetWindowContentRegionMax().x - 35, -1), GetColorU32(ImVec4(colors::checkbox::background)), 100);
    window->DrawList->AddCircleFilled(check_bb.Min + ImVec2(size + 9 + it_anim->second.move_circle, 10), 6.f, GetColorU32(it_anim->second.circle), 100);

    ImVec2 label_pos = ImVec2(check_bb.Max.x - 20, check_bb.Min.y + style.FramePadding.y);

    PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text));
    if (label_size.x > 0.0f) RenderText(label_pos, label);
    PopStyleColor();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

////////////
//BeginChild
///////////
bool elements::beginchildex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;

    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

    // Size
    const ImVec2 content_avail = GetContentRegionAvail();
    ImVec2 size = ImFloor(size_arg);
    const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
    if (size.x <= 0.0f)
        size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too many issues)
    if (size.y <= 0.0f)
        size.y = ImMax(content_avail.y + size.y, 4.0f);
    SetNextWindowPos(ImVec2(parent_window->DC.CursorPos + ImVec2(0, 39)));
    SetNextWindowSize(size - ImVec2(0, 39));

    ImGui::GetWindowDrawList()->AddRect(parent_window->DC.CursorPos + ImVec2(0.f, 0.f), parent_window->DC.CursorPos + ImVec2(size_arg.x, size_arg.y), GetColorU32(ImVec4(colors::child::border)), g.Style.ChildRounding, ImDrawListFlags_None, 2.f);
    ImGui::GetWindowDrawList()->AddLine(parent_window->DC.CursorPos + ImVec2(0.f, 40.f), parent_window->DC.CursorPos + ImVec2(size_arg.x, 40.f), GetColorU32(ImVec4(colors::child::border)), 2.f);
    ImGui::GetWindowDrawList()->AddText(parent_window->DC.CursorPos + ImVec2(12.f, 13.f), GetColorU32(ImVec4(colors::child::text)), name);

    char title[256];
    if (name)
        ImFormatString(title, IM_ARRAYSIZE(title), "%s/%s_%08X", parent_window->Name, name, id);
    else
        ImFormatString(title, IM_ARRAYSIZE(title), "%s/%08X", parent_window->Name, id);

    PushStyleColor(ImGuiCol_ChildBg, GetColorU32(ImVec4(colors::child::child)));
    bool ret = Begin(title, NULL, flags);
    PopStyleColor();


    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;
    child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

    // Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
    // While this is not really documented/defined, it seems that the expected thing to do.
    if (child_window->BeginCount == 1)
        parent_window->DC.CursorPos = child_window->Pos;

    // Process navigation-in immediately so NavInit can run on first frame
    if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
    {
        FocusWindow(child_window);
        NavInitWindow(child_window, false);
        SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
        g.ActiveIdSource = ImGuiInputSource_Nav;
    }
    return ret;
}
bool elements::beginchild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();

    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 13));
    return beginchildex(str_id, window->GetID(str_id), size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding);
}
void elements::endchild()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    PopStyleVar(2);

    IM_ASSERT(g.WithinEndChild == false);
    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

    g.WithinEndChild = true;
    if (window->BeginCount > 1)
    {
        End();
    }
    else
    {
        ImVec2 sz = window->Size;
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
            sz.x = ImMax(4.0f, sz.x);
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
            sz.y = ImMax(4.0f, sz.y);
        End();

        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
        ItemSize(sz);
        if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
        {
            ItemAdd(bb, window->ChildId);
            RenderNavHighlight(bb, window->ChildId);

            // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
            if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
                RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
        }
        else
        {
            // Not navigable into
            ItemAdd(bb, 0);
        }
        if (g.HoveredWindow == window)
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
    }
    g.WithinEndChild = false;
    g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
}


////////////
//Combobox
///////////

struct selectable_state {

    ImVec4 text_color, mark_color;

};
std::map<ImGuiID, selectable_state> anim_3;
bool elements::selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
    const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb(min_x, pos.y, text_max.x, text_max.y);
    if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
        const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

    // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
    const float backup_clip_rect_min_x = window->ClipRect.Min.x;
    const float backup_clip_rect_max_x = window->ClipRect.Max.x;
    if (span_all_columns)
    {
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
    }

    const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
    const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
    if (span_all_columns)
    {
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }

    if (!item_add)
        return false;

    const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
    if (disabled_item && !disabled_global) // Only testing this as an optimization
        BeginDisabled();

    // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
    // which would be advantageous since most selectable are not selected.
    if (span_all_columns && window->DC.CurrentColumns)
        PushColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePushBackgroundChannel();

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
    if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
    if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
    if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

    const bool was_selected = selected;
    bool hovered = IsItemHovered();
    bool pressed = hovered && g.IO.MouseClicked[0];

    // Auto-select when moved into
    // - This will be more fully fleshed in the range-select branch
    // - This is not exposed as it won't nicely work with some user side handling of shift/control
    // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
    //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
    //   - (2) usage will fail with clipped items
    //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
    if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
        if (g.NavJustMovedToId == id)
            selected = pressed = true;

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
            g.NavDisableHighlight = true;
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (flags & ImGuiSelectableFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

    if (span_all_columns && window->DC.CurrentColumns)
        PopColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePopBackgroundChannel();

    auto it_anim = anim_3.find(id);

    if (it_anim == anim_3.end())
    {
        anim_3.insert({ id, selectable_state() });
        it_anim = anim_3.find(id);
    }

    it_anim->second.text_color = ImLerp(it_anim->second.text_color, selected ? colors::combo::selectable::text_active : hovered ? colors::combo::selectable::text_hov : colors::combo::selectable::text, g.IO.DeltaTime * 6.f);
    it_anim->second.mark_color = ImLerp(it_anim->second.mark_color, selected ? colors::combo::selectable::mark_active : ImColor(0, 0, 0, 0), g.IO.DeltaTime * 6.f);

    PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text_color));
    RenderTextClipped(text_min - ImVec2(0, 6), text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
    RenderCheckMark(GetWindowDrawList(), ImVec2(text_min.x + GetWindowContentRegionMax().x - 18, text_max.y - 18), GetColorU32(it_anim->second.mark_color), 10.f);
    PopStyleColor();

    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();

    if (disabled_item && !disabled_global)
        EndDisabled();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed; //-V1020
}

bool elements::selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    if (selectable(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}
bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}
bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
    // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    if (!*p)
        return false;
    if (out_text)
        *out_text = p;
    return true;
}
std::map<ImGuiID, float> hovered_anim;
bool elements::combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_value);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!begincombo(label, preview_value, items_count, ImGuiComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    SetCursorPos(GetCursorPos() + ImVec2(0, 26));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 8));
    for (int i = 0; i < items_count; i++)
    {
        PushID(i);
        const bool item_selected = (i == *current_item);
        const char* item_text;
        if (!items_getter(data, i, &item_text))
            item_text = "*Unknown item*";
        if (selectable(item_text, item_selected))
        {
            value_changed = true;
            *current_item = i;
        }
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }
    PopStyleVar(1);
    End();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}
bool elements::combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = elements::combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);

    return value_changed;
}
bool elements::combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = elements::combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}
////////////
//Slider
///////////
extern float colorUI[3];
const char* PatchFormatStringFloatToInt(const char* fmt)
{
    if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) // Fast legacy path for "%.0f" which is expected to be the most common case.
        return "%d";
    const char* fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
    const char* fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
    if (fmt_end > fmt_start && fmt_end[-1] == 'f')
    {
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        if (fmt_start == fmt && fmt_end[0] == 0)
            return "%d";
        ImGuiContext& g = *GImGui;
        ImFormatString(g.TempBuffer.Data, IM_ARRAYSIZE(g.TempBuffer.Data), "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
        return g.TempBuffer.Data;
#else
        IM_ASSERT(0 && "DragInt(): Invalid format string!"); // Old versions used a default parameter of "%.0f", please replace with e.g. "%d"
#endif
    }
    return fmt;
}
struct slider_state {

    ImVec4 background_color, background_slider, circle_color, text_color;
    float slow, hint_slider, size;
};
std::map<ImGuiID, slider_state> anim;
bool elements::sliderscalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    float size = GetWindowContentRegionMax().x - 173;


    SetCursorPos(GetCursorPos() + ImVec2(size, 0));

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(100, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
        const bool clicked = hovered && IsMouseClicked(0, id);
        const bool make_active = (input_requested_by_tabbing || clicked || g.NavActivateId == id || g.NavActivateInputId == id);

        if (make_active && temp_input_allowed)
            if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || g.NavActivateInputId == id)
                temp_input_is_active = true;

        if (make_active && !temp_input_is_active)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active);


    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, slider_state() });
        it_anim = anim.find(id);
    }

    it_anim->second.circle_color = ImLerp(it_anim->second.circle_color, IsItemActive() ? colors::slider::circle_active : colors::slider::circle_inactive, g.IO.DeltaTime * 6.f);
    it_anim->second.text_color = ImLerp(it_anim->second.text_color, IsItemActive() ? colors::slider::text_active : hovered ? colors::slider::text_hov : colors::slider::text, g.IO.DeltaTime * 6.f);

    window->DrawList->AddRectFilled(frame_bb.Min + ImVec2(0, 9), frame_bb.Max - ImVec2(0, 7), GetColorU32(ImVec4(colors::slider::background)), 100);

    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    it_anim->second.slow = ImLerp(it_anim->second.slow, grab_bb.Min.x - frame_bb.Min.x, g.IO.DeltaTime * 20.f);

    if (grab_bb.Max.x > grab_bb.Min.x) {
        const int vtx_idx_1 = ImGui::GetWindowDrawList()->VtxBuffer.Size;
        window->DrawList->AddRectFilled(frame_bb.Min + ImVec2(0, 10), frame_bb.Min + ImVec2(it_anim->second.slow, 13), GetColorU32(ImVec4(colors::slider::background)), 100);
        ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImGui::GetWindowDrawList(), vtx_idx_1, ImGui::GetWindowDrawList()->VtxBuffer.Size, frame_bb.Min, frame_bb.Max, GetColorU32(ImVec4(colors::slider::gradient_start)), GetColorU32(ImVec4(colors::slider::gradient_end)));

        window->DrawList->AddCircleFilled(ImVec2(it_anim->second.slow + frame_bb.Min.x, grab_bb.Max.y - 7), 7, GetColorU32(it_anim->second.circle_color), 100);
    }

    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    RenderTextClipped(frame_bb.Min + ImVec2(91, 0), frame_bb.Max + ImVec2(91, 0), value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    PushStyleColor(ImGuiCol_Text, GetColorU32(it_anim->second.text_color));
    if (label_size.x > 0.0f) RenderText(ImVec2(frame_bb.Max.x - size - 100, frame_bb.Min.y + style.FramePadding.y), label);
    PopStyleColor();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return value_changed;

}
bool elements::sliderint(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    return elements::sliderscalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
}
bool elements::sliderfloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
    return elements::sliderscalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
}

////////////
//ColorEdit4
///////////
void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
{
    // This check is optional. Suppose we have two color widgets side by side, both widgets display different colors, but both colors have hue and/or saturation undefined.
    // With color check: hue/saturation is preserved in one widget. Editing color in one widget would reset hue/saturation in another one.
    // Without color check: common hue/saturation would be displayed in all widgets that have hue/saturation undefined.
    // g.ColorEditLastColor is stored as ImU32 RGB value: this essentially gives us color equality check with reduced precision.
    // Tiny external color changes would not be detected and this check would still pass. This is OK, since we only restore hue/saturation _only_ if they are undefined,
    // therefore this change flipping hue/saturation from undefined to a very tiny value would still be represented in color picker.
    ImGuiContext& g = *GImGui;
    if (g.ColorEditLastColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
        return;

    // When S == 0, H is undefined.
    // When H == 1 it wraps around to 0.
    if (*S == 0.0f || (*H == 0.0f && g.ColorEditLastHue == 1))
        *H = g.ColorEditLastHue;

    // When V == 0, S is undefined.
    if (*V == 0.0f)
        *S = g.ColorEditLastSat;
}
std::map<ImGuiID, float> nameanim_animationX_6;
std::map<ImGuiID, float> nameanim_animationY_6;
bool elements::coloredit4(ImFont* icon, const char* label, float col[4], ImGuiColorEditFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const float square_sz = GetFrameHeight();
    const float w_full = CalcItemWidth();
    const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
    const float w_inputs = w_full - w_button;
    const char* label_display_end = FindRenderedTextEnd(label);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos);

    const ImRect total_bb(frame_bb.Min, frame_bb.Max);

    g.NextItemData.ClearFlags();
    const ImGuiID id = window->GetID(label);
    BeginGroup();
    PushID(label);

    // If we're not showing any slider there's no point in doing any HSV conversions
    const ImGuiColorEditFlags flags_untouched = flags;
    if (flags & ImGuiColorEditFlags_NoInputs)
        flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

    // Context menu: display and modify options (before defaults are applied)
    if (!(flags & ImGuiColorEditFlags_NoOptions))
        ColorEditOptionsPopup(col, flags);

    // Read stored options
    if (!(flags & ImGuiColorEditFlags_DisplayMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
    if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
    if (!(flags & ImGuiColorEditFlags_PickerMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
    if (!(flags & ImGuiColorEditFlags_InputMask_))
        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
    flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

    const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
    const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
    const int components = alpha ? 4 : 3;

    // Convert to the formats we need
    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
        ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
    else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
    {
        // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
        ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
        ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
    }
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos;
    const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
    window->DC.CursorPos.x = pos.x + inputs_offset_x;

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.10f, 0.10f, 0.10f, 0.30f));

    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.08f, 0.08f, 0.08f, 0.30f));

    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.08f, 0.08f, 0.08f, 0.30f));


    auto it_nameanimX = nameanim_animationX_6.find(id);
    if (it_nameanimX == nameanim_animationX_6.end())
    {
        nameanim_animationX_6.insert({ id, 0.f });
        it_nameanimX = nameanim_animationX_6.find(id);
    }

    it_nameanimX->second = ImClamp(it_nameanimX->second + (30.f * ImGui::GetIO().DeltaTime * (it_nameanimX->second ? 30.f : -30.0f)), 0.f, square_sz * 14 - 17);


    auto it_nameanimY = nameanim_animationY_6.find(id);
    if (it_nameanimY == nameanim_animationY_6.end())
    {
        nameanim_animationY_6.insert({ id, 0.f });
        it_nameanimY = nameanim_animationY_6.find(id);
    }
    it_nameanimY->second = ImClamp(it_nameanimY->second + (30.f * ImGui::GetIO().DeltaTime * (it_nameanimY->second ? 30.f : -30.0f)), 0.f, square_sz * 13 - 3);

    // Размер

    SetNextWindowSize(ImVec2((float)it_nameanimX->second, (float)it_nameanimY->second));

    if ((flags & (ImGuiColorEditFlags_DisplayRGB)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
    {
        // RGB/HSV 0..255 Sliders

        const float w_item_one = ImMax(0.4f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
        const float w_item_last = ImMax(0.4f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

        ImGui::Spacing();

        const bool hide_prefix = (w_item_one <= CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
        const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
        const char* fmt_table_int[3][4] =
        {
            {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
            { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
            { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
        };
        const char* fmt_table_float[3][4] =
        {
            {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
            { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
            { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
        };
        const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

        for (int n = 0; n < components; n++)
        {
            if (n > 0)
                SameLine(0, style.ItemInnerSpacing.x);
            SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

            // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
            if (flags & ImGuiColorEditFlags_Float)
            {
                value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                value_changed_as_float |= value_changed;
            }
            else
            {
                value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context");
        }
    }

    ImGui::PopStyleColor(3);

    ImGuiWindow* picker_active_window = NULL;
    if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
    {
        const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

        const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (colorbutton(icon, "##ColorButton", col_v4, flags))
        {
            it_nameanimX->second = true;
            it_nameanimY->second = true;
            if (!(flags & ImGuiColorEditFlags_NoPicker))
            {
                // Store current color and open a picker
                g.ColorPickerRef = col_v4;
                OpenPopup("picker");
                SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
            }
        }

        ImGui::SetCursorPos(ImVec2(5, 0));

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0, 0, 0, 0));

        ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));

        if (!(flags & ImGuiColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context");

        if (BeginPopup("picker"))
        {

            picker_active_window = g.CurrentWindow;
            if (label != label_display_end)
            {

            }
            ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
            ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
            SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
            value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
            EndPopup();
        }

        ImGui::PopStyleColor(1);

        ImGui::PopStyleVar(2);

    }

    if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
    {
        const float text_offset_x = (flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + text_offset_x - 30, pos.y + style.FramePadding.y);
        TextEx(label, label_display_end);
    }

    // Convert back
    if (value_changed && picker_active_window == NULL)
    {
        if (!value_changed_as_float)
            for (int n = 0; n < 4; n++)
                f[n] = i[n] / 255.0f;
        if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
        {
            g.ColorEditLastHue = f[0];
            g.ColorEditLastSat = f[1];
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
            g.ColorEditLastColor = ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
        }
        if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        col[0] = f[0];
        col[1] = f[1];
        col[2] = f[2];
        if (alpha)
            col[3] = f[3];
    }

    PopID();
    EndGroup();

    // Drag and Drop Target
    // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
    if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget())
    {
        bool accepted_drag_drop = false;
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512
            value_changed = accepted_drag_drop = true;
        }
        if (const ImGuiPayload* payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * components);
            value_changed = accepted_drag_drop = true;
        }

        // Drag-drop payloads are always RGB
        if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
        EndDragDropTarget();
    }

    // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
    if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
        g.LastItemData.ID = g.ActiveId;

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

////////////
//Button
///////////
std::map<ImGuiID, float> nameanim_animation_7;
std::map<ImGuiID, float> nameanim_opticaly_7;
bool elements::buttonex(const char* label, const ImVec2& size_arg, bool border, ImGuiButtonFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);


    auto it_nameanim = nameanim_animation_7.find(id);
    if (it_nameanim == nameanim_animation_7.end())
    {
        nameanim_animation_7.insert({ id, 0.f });
        it_nameanim = nameanim_animation_7.find(id);
    }

    it_nameanim->second = ImClamp(it_nameanim->second + (0.2f * ImGui::GetIO().DeltaTime * (hovered && !IsItemActive() ? 1.f : -1.0f)), 0.7f, 0.73f);
    it_nameanim->second *= style.Alpha;



    auto it_opt = nameanim_opticaly_7.find(id);
    if (it_opt == nameanim_opticaly_7.end())
    {
        nameanim_opticaly_7.insert({ id, 0.f });
        it_opt = nameanim_opticaly_7.find(id);
    }

    it_opt->second = ImClamp(it_opt->second + (1.5f * ImGui::GetIO().DeltaTime * (hovered && !IsItemActive() ? 1.f : -1.0f)), 0.0f, 0.3f);
    it_opt->second *= style.Alpha;


    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(bb, id);

    if (!border) RenderFrame(bb.Min, bb.Max, ImColor(0.80f - it_nameanim->second, 0.80f - it_nameanim->second, 0.80f - it_nameanim->second, 1.00f), true, style.FrameRounding);

    RenderFrame(bb.Min + ImVec2(1, 1), bb.Max + ImVec2(bb.Min.x - bb.Min.x - 1, bb.Min.y - bb.Min.y - 1), ImColor(0.03f, 0.03f, 0.03f, 1.00f - (float)it_opt->second), true, style.FrameRounding);

    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
    return pressed;
}

bool elements::button(const char* label, const ImVec2& size_arg, bool border)
{
    return elements::buttonex(label, size_arg, border, ImGuiButtonFlags_None);
}

std::unordered_map<int, std::string> key_names =
{
    { 0, "None" },
    { VK_LBUTTON, "MOUSE 1" },
    { VK_RBUTTON, "MOUSE 2" },
    { VK_MBUTTON, "MOUSE 3" },
    { VK_XBUTTON1, "MOUSE 4" },
    { VK_XBUTTON2, "MOUSE 5" },
    { VK_BACK, "BACK" },
    { VK_TAB, "TAB" },
    { VK_CLEAR, "CLEAR" },
    { VK_RETURN, "ENTER" },
    { VK_CONTROL, "Ctrl" },
    { VK_MENU, "ALT" },
    { VK_PAUSE, "PAUSE" },
    { VK_CAPITAL, "CAPS" },
    { VK_ESCAPE, "ESCAPE" },
    { VK_SPACE, "SPACE" },
    { VK_PRIOR, "PAGE UP" },
    { VK_NEXT, "PAGE DN" },
    { VK_END, "END" },
    { VK_HOME, "HOME" },
    { VK_LEFT, "LEFT KEY" },
    { VK_UP, "UP KEY" },
    { VK_RIGHT, "RIGHT KEY" },
    { VK_DOWN, "DOWN KEY" },
    { VK_SELECT, "SELECT" },
    { VK_PRINT, "SCREEN" },
    { VK_INSERT, "INSERT" },
    { VK_DELETE, "DELETE" },
    { VK_HELP, "HELP" },
    { VK_SLEEP, "SLEEP" },
    { VK_MULTIPLY, "*" },
    { VK_OEM_PLUS, "PLUS" },
    { VK_OEM_MINUS, "MINUS" },
    { VK_OEM_PERIOD, "POINT" },
    { VK_NUMLOCK, "NUM LOCK" },
    { VK_SCROLL, "SCROLL" },
    { VK_LSHIFT, "SHIFT" },
    { VK_RSHIFT, "RIGHT SHIFT" },
    { VK_LCONTROL, "LEFT CTRL" },
    { VK_RCONTROL, "RIGHT CTRL" },
    { VK_LMENU, "LEFT ALT" },
    { VK_RMENU, "RIGHT ALT" },
    { VK_OEM_2, "SLASH" },
    { VK_OEM_3, "TILDA" },
    { VK_F1, "F1" },
    { VK_F2, "F2" },
    { VK_F3, "F3" },
    { VK_F4, "F4" },
    { VK_F5, "F5" },
    { VK_F6, "F6" },
    { VK_F7, "F7" },
    { VK_F8, "F8" },
    { VK_F9, "F9" },
    { VK_F10, "F10" },
    { VK_F11, "F11" },
    { VK_F12, "F12" },
    { 0x31, "1" },
    { 0x32, "2" },
    { 0x33, "3" },
    { 0x34, "4" },
    { 0x35, "5" },
    { 0x36, "6" },
    { 0x37, "7" },
    { 0x38, "8" },
    { 0x39, "9" },
    { ImGuiKey_GamepadStart, "[Xbox] Menu" },
    { ImGuiKey_GamepadBack, "[Xbox] View" },
    { ImGuiKey_GamepadFaceLeft, "[Xbox] X" },
    { ImGuiKey_GamepadFaceRight, "[Xbox] Y" },
    { ImGuiKey_GamepadFaceUp, "[Xbox] A" },
    { ImGuiKey_GamepadFaceDown, "[Xbox] B" },
    { ImGuiKey_GamepadDpadLeft, "[Xbox] D-pad Left" },
    { ImGuiKey_GamepadDpadRight, "[Xbox] D-pad Right" },
    { ImGuiKey_GamepadDpadUp, "[Xbox] D-pad Up" },
    { ImGuiKey_GamepadDpadDown, "[Xbox] D-pad Down" },
    { ImGuiKey_GamepadL1, "[Xbox] L Bumper" },
    { ImGuiKey_GamepadR1, "[Xbox] R Bumper" },
    { ImGuiKey_GamepadL2, "[Xbox] L Trig" },
    { ImGuiKey_GamepadR2, "[Xbox] R Trig" },
    { ImGuiKey_GamepadL3, "[Xbox] L Stick" },
    { ImGuiKey_GamepadR3, "[Xbox] R Stick" },


};

std::string GetKeyName(const int id)
{

    if (id >= 0x30 && id <= 0x5A)
        return std::string(1, (char)id);

    return key_names[id];
}

DWORD astime2 = 0;
bool elements::HotKey(const char* label, int* k, const ImVec2& size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
    ImVec2 t = window->DC.CursorPos;
    t.x += label_size.x + style.ItemInnerSpacing.x;
    ImVec2 t2 = window->DC.CursorPos;
    t2.x += size.x + 370.f;
    t2.y += size.y;
    const ImRect frame_bb(t, t2);
    const ImRect total_bb({ window->DC.CursorPos.x - 100.f, window->DC.CursorPos.y }, frame_bb.Max);

    ImGui::ItemSize(frame_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;


    const bool hovered = ImGui::ItemHoverable(total_bb, id);

    if (hovered) {
        ImGui::SetHoveredID(id);
        g.MouseCursor = ImGuiMouseCursor_TextInput;
    }

    const bool user_clicked = hovered && io.MouseClicked[0];

    if (user_clicked) {
        if (g.ActiveId != id) {
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            *k = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (io.MouseClicked[0]) {
        if (g.ActiveId == id)
            ImGui::ClearActiveID();
    }

    bool value_changed = false;
    int key = *k;

    if (g.ActiveId == id && timeGetTime() - astime2 >= 200) {
        astime2 = timeGetTime();
        for (auto i = 0; i < 5; i++) {
            if (io.MouseDown[i]) {
                switch (i) {
                case 0:
                    key = VK_LBUTTON;
                    break;
                case 1:
                    key = VK_RBUTTON;
                    break;
                case 2:
                    key = VK_MBUTTON;
                    break;
                case 3:
                    key = VK_XBUTTON1;
                    break;
                case 4:
                    key = VK_XBUTTON2;
                    break;
                }
                value_changed = true;
                ImGui::ClearActiveID();
            }
        }
        if (!value_changed) {
            for (auto i = VK_BACK; i <= VK_RMENU; i++) {
                if (io.KeysDown[i]) {
                    key = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
            for (int i = ImGuiKey_GamepadStart; i < ImGuiKey_GamepadR3; i++) {
                if (ImGui::IsKeyDown(ImGui_ImplWin32_VirtualKeyToImGuiKey(i))) {
                    key = i;
                    value_changed = true;
                    ClearActiveID();
                }
            }
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            *k = 0;
            ImGui::ClearActiveID();
        }
        else {
            *k = key;
        }
    }

    char buf_display[64];

    strcpy_s(buf_display, (("NONE")));



    if (*k != 0 && g.ActiveId != id) {
        strcpy_s(buf_display, GetKeyName(key).c_str());
    }
    else if (g.ActiveId == id) {
        strcpy_s(buf_display, (("...")));
    }

    const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y);
    ImVec2 render_pos = frame_bb.Min;
    render_pos.x += style.FramePadding.x;
    render_pos.y += style.FramePadding.y;
    ImGui::RenderFrame(ImVec2(total_bb.Min.x + 280.f, total_bb.Min.y), { frame_bb.Max.x - 230.f, frame_bb.Max.y }, ImColor(0.03f, 0.03f, 0.03f, 1.00f), true, 10.f);
    ImGui::RenderTextClipped({ total_bb.Min.x + 280.f, frame_bb.Min.y + style.FramePadding.y }, { frame_bb.Max.x - 230.f, frame_bb.Max.y - style.FramePadding.y }, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
    if (label_size.x > 0)
        ImGui::RenderText(ImVec2(total_bb.Min.x + 100.f, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;

}

bool elements::colorbutton(ImFont* icon, const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, ImVec2 size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(desc_id);
    float default_size = GetFrameHeight();
    if (size.x == 0.0f)
        size.x = default_size;
    if (size.y == 0.0f)
        size.y = default_size;

    ImGui::SetCursorPosX(CalcItemWidth() + 25);

    const ImRect bb(window->DC.CursorPos + ImVec2(-CalcItemWidth(), 0), window->DC.CursorPos + size);
    ItemSize(bb, (size.y >= default_size) ? g.Style.FramePadding.y : 0.0f);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    if (flags & ImGuiColorEditFlags_NoAlpha)
        flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);

    ImVec4 col_rgb = col;
    if (flags & ImGuiColorEditFlags_InputHSV)
        ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

    ImVec4 col_rgb_without_alpha(col_rgb.x, col_rgb.y, col_rgb.z, 1.0f);
    float grid_step = ImMin(size.x, size.y) / 2.99f;
    float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
    ImRect bb_inner = bb;
    float off = 0.0f;
    if ((flags & ImGuiColorEditFlags_NoBorder) == 0)
    {
        off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
        bb_inner.Expand(off);
    }
    if ((flags & ImGuiColorEditFlags_AlphaPreviewHalf) && col_rgb.w < 1.0f)
    {
        float mid_x = IM_ROUND((bb_inner.Min.x + bb_inner.Max.x) * 0.5f);
        RenderColorRectWithAlphaCheckerboard(window->DrawList, ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, GetColorU32(col_rgb), grid_step, ImVec2(-grid_step + off, off), rounding, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);
        window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), GetColorU32(col_rgb_without_alpha), rounding, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft);
    }
    else
    {
        ImVec4 col_source = (flags & ImGuiColorEditFlags_AlphaPreview) ? col_rgb : col_rgb_without_alpha;

        ImGui::PushFont(icon);

        ImGui::PushStyleColor(ImGuiCol_Text, GetColorU32(col_source));

        RenderText(ImVec2(bb_inner.Max.x - 15, bb_inner.Min.y + 4), "Q");

        ImGui::PopStyleColor(1);

        ImGui::PopFont();

    }
    RenderNavHighlight(bb, id);

    return pressed;
}
struct combo_state {

    ImVec4 background_combo, text_color;

    bool opened_combo = false, hovered = false;
    float combo_size = 0.f;

};
std::map<ImGuiID, combo_state> anim_5;
bool elements::begincombo(const char* label, const char* preview_value, int items_count, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    if (window->SkipItems)
        return false;
    float size_x = GetWindowContentRegionMax().x - 173;
    ImGui::SetCursorPos(GetCursorPos() + ImVec2(size_x, 0));

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

    const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : CalcItemWidth();
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(155, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(bb.Min, bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &bb))
        return false;

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);

    auto it_anim = anim_5.find(id);

    if (it_anim == anim_5.end())
    {
        anim_5.insert({ id, combo_state() });
        it_anim = anim_5.find(id);
    }

    if (hovered && g.IO.MouseClicked[0] || it_anim->second.opened_combo && g.IO.MouseClicked[0] && !it_anim->second.hovered)
        it_anim->second.opened_combo = !it_anim->second.opened_combo;

    it_anim->second.combo_size = ImLerp(it_anim->second.combo_size, it_anim->second.opened_combo ? items_count * 23 : 0.f, g.IO.DeltaTime * 8.f);
    it_anim->second.text_color = ImLerp(it_anim->second.text_color, it_anim->second.opened_combo ? colors::combo::text_active : hovered ? colors::combo::text_hov : colors::combo::text, g.IO.DeltaTime * 6.f);

    const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);
    window->DrawList->AddRectFilled(bb.Min, ImVec2(value_x2 + 21, bb.Max.y), GetColorU32(ImVec4(colors::combo::background)), style.FrameRounding);
    window->DrawList->AddCircleFilled(ImVec2(value_x2 + 11, bb.Max.y - 10), 4.f, GetColorU32(ImVec4(colors::combo::circle)), 100);

    RenderTextClipped(bb.Min + style.FramePadding, ImVec2(value_x2, bb.Max.y), preview_value, NULL, NULL);
    window->DrawList->AddText(ImVec2(bb.Max.x - size_x - 155, bb.Min.y + 2), GetColorU32(it_anim->second.text_color), label);

    if (!it_anim->second.opened_combo && it_anim->second.combo_size < 2.f)
        return false;

    ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + 2));

    ImGui::SetNextWindowSize(ImVec2(bb.GetWidth(), it_anim->second.combo_size));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollWithMouse;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(g.Style.FramePadding.x, g.Style.WindowPadding.y - 30));
    PushStyleVar(ImGuiStyleVar_WindowRounding, 4);
    PushStyleColor(ImGuiCol_WindowBg, GetColorU32(ImVec4(colors::combo::background)));
    bool ret = Begin(label, NULL, window_flags);
    PopStyleVar(2);
    PopStyleColor(1);

    it_anim->second.hovered = IsWindowHovered();
    return true;
}

int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end)
{
    int line_count = 0;
    const char* s = text_begin;
    while (char c = *s++) // We are only matching for \n so we can ignore UTF-8 decoding
        if (c == '\n')
            line_count++;
    s--;
    if (s[0] != '\n' && s[0] != '\r')
        line_count++;
    *out_text_end = s;
    return line_count;
}

ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining, ImVec2* out_offset, bool stop_on_new_line)
{
    ImGuiContext& g = *GImGui;
    ImFont* font = g.Font;
    const float line_height = g.FontSize;
    const float scale = line_height / font->FontSize;

    ImVec2 text_size = ImVec2(0, 0);
    float line_width = 0.0f;

    const ImWchar* s = text_begin;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)(*s++);
        if (c == '\n')
        {
            text_size.x = ImMax(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0.0f;
            if (stop_on_new_line)
                break;
            continue;
        }
        if (c == '\r')
            continue;

        const float char_width = font->GetCharAdvance((ImWchar)c) * scale;
        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (out_offset)
        *out_offset = ImVec2(line_width, text_size.y + line_height);  // offset allow for the possibility of sitting after a trailing \n

    if (line_width > 0 || text_size.y == 0.0f)                        // whereas size.y will ignore the trailing \n
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

namespace ImStb
{

    int     STB_TEXTEDIT_STRINGLEN(const ImGuiInputTextState* obj) { return obj->CurLenW; }
    ImWchar STB_TEXTEDIT_GETCHAR(const ImGuiInputTextState* obj, int idx) { return obj->TextW[idx]; }
    float   STB_TEXTEDIT_GETWIDTH(ImGuiInputTextState* obj, int line_start_idx, int char_idx) { ImWchar c = obj->TextW[line_start_idx + char_idx]; if (c == '\n') return STB_TEXTEDIT_GETWIDTH_NEWLINE; ImGuiContext& g = *GImGui; return g.Font->GetCharAdvance(c) * (g.FontSize / g.Font->FontSize); }
    int     STB_TEXTEDIT_KEYTOTEXT(int key) { return key >= 0x200000 ? 0 : key; }
    ImWchar STB_TEXTEDIT_NEWLINE = '\n';
    void    STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r, ImGuiInputTextState* obj, int line_start_idx)
    {
        const ImWchar* text = obj->TextW.Data;
        const ImWchar* text_remaining = NULL;
        const ImVec2 size = InputTextCalcTextSizeW(text + line_start_idx, text + obj->CurLenW, &text_remaining, NULL, true);
        r->x0 = 0.0f;
        r->x1 = size.x;
        r->baseline_y_delta = size.y;
        r->ymin = 0.0f;
        r->ymax = size.y;
        r->num_chars = (int)(text_remaining - (text + line_start_idx));
    }

    // When ImGuiInputTextFlags_Password is set, we don't want actions such as CTRL+Arrow to leak the fact that underlying data are blanks or separators.
    bool is_separator(unsigned int c) { return ImCharIsBlankW(c) || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == '|' || c == '\n' || c == '\r'; }
    int  is_word_boundary_from_right(ImGuiInputTextState* obj, int idx) { if (obj->Flags & ImGuiInputTextFlags_Password) return 0; return idx > 0 ? (is_separator(obj->TextW[idx - 1]) && !is_separator(obj->TextW[idx])) : 1; }
    int  is_word_boundary_from_left(ImGuiInputTextState* obj, int idx) { if (obj->Flags & ImGuiInputTextFlags_Password) return 0; return idx > 0 ? (!is_separator(obj->TextW[idx - 1]) && is_separator(obj->TextW[idx])) : 1; }
    int  STB_TEXTEDIT_MOVEWORDLEFT_IMPL(ImGuiInputTextState* obj, int idx) { idx--; while (idx >= 0 && !is_word_boundary_from_right(obj, idx)) idx--; return idx < 0 ? 0 : idx; }
    int  STB_TEXTEDIT_MOVEWORDRIGHT_MAC(ImGuiInputTextState* obj, int idx) { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_left(obj, idx)) idx++; return idx > len ? len : idx; }
    int  STB_TEXTEDIT_MOVEWORDRIGHT_WIN(ImGuiInputTextState* obj, int idx) { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_right(obj, idx)) idx++; return idx > len ? len : idx; }
    int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(ImGuiInputTextState* obj, int idx) { if (ImGui::GetIO().ConfigMacOSXBehaviors) return STB_TEXTEDIT_MOVEWORDRIGHT_MAC(obj, idx); else return STB_TEXTEDIT_MOVEWORDRIGHT_WIN(obj, idx); }
#define STB_TEXTEDIT_MOVEWORDLEFT   STB_TEXTEDIT_MOVEWORDLEFT_IMPL  // They need to be #define for stb_textedit.h
#define STB_TEXTEDIT_MOVEWORDRIGHT  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

    void STB_TEXTEDIT_DELETECHARS(ImGuiInputTextState* obj, int pos, int n)
    {
        ImWchar* dst = obj->TextW.Data + pos;

        // We maintain our buffer length in both UTF-8 and wchar formats
        obj->Edited = true;
        obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
        obj->CurLenW -= n;

        // Offset remaining text (FIXME-OPT: Use memmove)
        const ImWchar* src = obj->TextW.Data + pos + n;
        while (ImWchar c = *src++)
            *dst++ = c;
        *dst = '\0';
    }

    bool STB_TEXTEDIT_INSERTCHARS(ImGuiInputTextState* obj, int pos, const ImWchar* new_text, int new_text_len)
    {
        const bool is_resizable = (obj->Flags & ImGuiInputTextFlags_CallbackResize) != 0;
        const int text_len = obj->CurLenW;
        IM_ASSERT(pos <= text_len);

        const int new_text_len_utf8 = ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
        if (!is_resizable && (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufCapacityA))
            return false;

        // Grow internal buffer if needed
        if (new_text_len + text_len + 1 > obj->TextW.Size)
        {
            if (!is_resizable)
                return false;
            IM_ASSERT(text_len < obj->TextW.Size);
            obj->TextW.resize(text_len + ImClamp(new_text_len * 4, 32, ImMax(256, new_text_len)) + 1);
        }

        ImWchar* text = obj->TextW.Data;
        if (pos != text_len)
            memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos) * sizeof(ImWchar));
        memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));

        obj->Edited = true;
        obj->CurLenW += new_text_len;
        obj->CurLenA += new_text_len_utf8;
        obj->TextW[obj->CurLenW] = '\0';

        return true;
    }

    // We don't use an enum so we can build even with conflicting symbols (if another user of stb_textedit.h leak their STB_TEXTEDIT_K_* symbols)
#define STB_TEXTEDIT_K_LEFT         0x200000 // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT        0x200001 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP           0x200002 // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN         0x200003 // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART    0x200004 // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND      0x200005 // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART    0x200006 // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND      0x200007 // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE       0x200008 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE    0x200009 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO         0x20000A // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO         0x20000B // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT     0x20000C // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT    0x20000D // keyboard input to move cursor right one word
#define STB_TEXTEDIT_K_PGUP         0x20000E // keyboard input to move cursor up a page
#define STB_TEXTEDIT_K_PGDOWN       0x20000F // keyboard input to move cursor down a page
#define STB_TEXTEDIT_K_SHIFT        0x400000

#define STB_TEXTEDIT_IMPLEMENTATION
#include "imstb_textedit.h"

// stb_textedit internally allows for a single undo record to do addition and deletion, but somehow, calling
// the stb_textedit_paste() function creates two separate records, so we perform it manually. (FIXME: Report to nothings/stb?)
    void stb_textedit_replace(ImGuiInputTextState* str, STB_TexteditState* state, const STB_TEXTEDIT_CHARTYPE* text, int text_len)
    {
        stb_text_makeundo_replace(str, state, 0, str->CurLenW, text_len);
        ImStb::STB_TEXTEDIT_DELETECHARS(str, 0, str->CurLenW);
        state->cursor = state->select_start = state->select_end = 0;
        if (text_len <= 0)
            return;
        if (ImStb::STB_TEXTEDIT_INSERTCHARS(str, 0, text, text_len))
        {
            state->cursor = state->select_start = state->select_end = text_len;
            state->has_preferred_x = 0;
            return;
        }
        IM_ASSERT(0); // Failed to insert character, normally shouldn't happen because of how we currently use stb_textedit_replace()
    }

} // namespace ImStb

// Return false to discard a character.
bool InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data, ImGuiInputSource input_source)
{
    IM_ASSERT(input_source == ImGuiInputSource_Keyboard || input_source == ImGuiInputSource_Clipboard);
    unsigned int c = *p_char;

    // Filter non-printable (NB: isprint is unreliable! see #2467)
    bool apply_named_filters = true;
    if (c < 0x20)
    {
        bool pass = false;
        pass |= (c == '\n' && (flags & ImGuiInputTextFlags_Multiline)); // Note that an Enter KEY will emit \r and be ignored (we poll for KEY in InputText() code)
        pass |= (c == '\t' && (flags & ImGuiInputTextFlags_AllowTabInput));
        if (!pass)
            return false;
        apply_named_filters = false; // Override named filters below so newline and tabs can still be inserted.
    }

    if (input_source != ImGuiInputSource_Clipboard)
    {
        // We ignore Ascii representation of delete (emitted from Backspace on OSX, see #2578, #2817)
        if (c == 127)
            return false;

        // Filter private Unicode range. GLFW on OSX seems to send private characters for special keys like arrow keys (FIXME)
        if (c >= 0xE000 && c <= 0xF8FF)
            return false;
    }

    // Filter Unicode ranges we are not handling in this build
    if (c > IM_UNICODE_CODEPOINT_MAX)
        return false;

    // Generic named filters
    if (apply_named_filters && (flags & (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsScientific)))
    {
        // The libc allows overriding locale, with e.g. 'setlocale(LC_NUMERIC, "de_DE.UTF-8");' which affect the output/input of printf/scanf to use e.g. ',' instead of '.'.
        // The standard mandate that programs starts in the "C" locale where the decimal point is '.'.
        // We don't really intend to provide widespread support for it, but out of empathy for people stuck with using odd API, we support the bare minimum aka overriding the decimal point.
        // Change the default decimal_point with:
        //   ImGui::GetCurrentContext()->PlatformLocaleDecimalPoint = *localeconv()->decimal_point;
        // Users of non-default decimal point (in particular ',') may be affected by word-selection logic (is_word_boundary_from_right/is_word_boundary_from_left) functions.
        ImGuiContext& g = *GImGui;
        const unsigned c_decimal_point = (unsigned int)g.PlatformLocaleDecimalPoint;

        // Full-width -> half-width conversion for numeric fields (https://en.wikipedia.org/wiki/Halfwidth_and_Fullwidth_Forms_(Unicode_block)
        // While this is mostly convenient, this has the side-effect for uninformed users accidentally inputting full-width characters that they may
        // scratch their head as to why it works in numerical fields vs in generic text fields it would require support in the font.
        if (flags & (ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsScientific | ImGuiInputTextFlags_CharsHexadecimal))
            if (c >= 0xFF01 && c <= 0xFF5E)
                c = c - 0xFF01 + 0x21;

        // Allow 0-9 . - + * /
        if (flags & ImGuiInputTextFlags_CharsDecimal)
            if (!(c >= '0' && c <= '9') && (c != c_decimal_point) && (c != '-') && (c != '+') && (c != '*') && (c != '/'))
                return false;

        // Allow 0-9 . - + * / e E
        if (flags & ImGuiInputTextFlags_CharsScientific)
            if (!(c >= '0' && c <= '9') && (c != c_decimal_point) && (c != '-') && (c != '+') && (c != '*') && (c != '/') && (c != 'e') && (c != 'E'))
                return false;

        // Allow 0-9 a-F A-F
        if (flags & ImGuiInputTextFlags_CharsHexadecimal)
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F'))
                return false;

        // Turn a-z into A-Z
        if (flags & ImGuiInputTextFlags_CharsUppercase)
            if (c >= 'a' && c <= 'z')
                c += (unsigned int)('A' - 'a');

        if (flags & ImGuiInputTextFlags_CharsNoBlank)
            if (ImCharIsBlankW(c))
                return false;

        *p_char = c;
    }

    // Custom callback filter
    if (flags & ImGuiInputTextFlags_CallbackCharFilter)
    {
        ImGuiInputTextCallbackData callback_data;
        memset(&callback_data, 0, sizeof(ImGuiInputTextCallbackData));
        callback_data.EventFlag = ImGuiInputTextFlags_CallbackCharFilter;
        callback_data.EventChar = (ImWchar)c;
        callback_data.Flags = flags;
        callback_data.UserData = user_data;
        if (callback(&callback_data) != 0)
            return false;
        *p_char = callback_data.EventChar;
        if (!callback_data.EventChar)
            return false;
    }

    return true;
}

// Find the shortest single replacement we can make to get the new text from the old text.
// Important: needs to be run before TextW is rewritten with the new characters because calling STB_TEXTEDIT_GETCHAR() at the end.
// FIXME: Ideally we should transition toward (1) making InsertChars()/DeleteChars() update undo-stack (2) discourage (and keep reconcile) or obsolete (and remove reconcile) accessing buffer directly.
static void InputTextReconcileUndoStateAfterUserCallback(ImGuiInputTextState* state, const char* new_buf_a, int new_length_a)
{
    ImGuiContext& g = *GImGui;
    const ImWchar* old_buf = state->TextW.Data;
    const int old_length = state->CurLenW;
    const int new_length = ImTextCountCharsFromUtf8(new_buf_a, new_buf_a + new_length_a);
    g.TempBuffer.reserve_discard((new_length + 1) * sizeof(ImWchar));
    ImWchar* new_buf = (ImWchar*)(void*)g.TempBuffer.Data;
    ImTextStrFromUtf8(new_buf, new_length + 1, new_buf_a, new_buf_a + new_length_a);

    const int shorter_length = ImMin(old_length, new_length);
    int first_diff;
    for (first_diff = 0; first_diff < shorter_length; first_diff++)
        if (old_buf[first_diff] != new_buf[first_diff])
            break;
    if (first_diff == old_length && first_diff == new_length)
        return;

    int old_last_diff = old_length - 1;
    int new_last_diff = new_length - 1;
    for (; old_last_diff >= first_diff && new_last_diff >= first_diff; old_last_diff--, new_last_diff--)
        if (old_buf[old_last_diff] != new_buf[new_last_diff])
            break;

    const int insert_len = new_last_diff - first_diff + 1;
    const int delete_len = old_last_diff - first_diff + 1;
    if (insert_len > 0 || delete_len > 0)
        if (STB_TEXTEDIT_CHARTYPE* p = stb_text_createundo(&state->Stb.undostate, first_diff, delete_len, insert_len))
            for (int i = 0; i < delete_len; i++)
                p[i] = ImStb::STB_TEXTEDIT_GETCHAR(state, first_diff + i);
}

bool elements::input_text_ex(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* callback_user_data)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    IM_ASSERT(buf != NULL && buf_size >= 0);
    IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackHistory) && (flags & ImGuiInputTextFlags_Multiline)));        // Can't use both together (they both use up/down keys)
    IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) && (flags & ImGuiInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const bool RENDER_SELECTION_WHEN_INACTIVE = false;
    const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
    const bool is_readonly = (flags & ImGuiInputTextFlags_ReadOnly) != 0;
    const bool is_password = (flags & ImGuiInputTextFlags_Password) != 0;
    const bool is_undoable = (flags & ImGuiInputTextFlags_NoUndoRedo) == 0;
    const bool is_resizable = (flags & ImGuiInputTextFlags_CallbackResize) != 0;
    if (is_resizable)
        IM_ASSERT(callback != NULL); // Must provide a callback if you set the ImGuiInputTextFlags_CallbackResize flag!

    if (is_multiline) // Open group before calling GetID() because groups tracks id created within their scope (including the scrollbar)
        BeginGroup();
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec2 frame_size = CalcItemSize(size_arg, CalcItemWidth(), (is_multiline ? g.FontSize * 8.0f : label_size.y) + style.FramePadding.y * 2.0f); // Arbitrary default of 8 lines high for multi-line
    const ImVec2 total_size = ImVec2(frame_size.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_size.y);

    const ImRect frame_bb(window->DC.CursorPos + ImVec2(window->Size.x - frame_size.x - 50, 10), window->DC.CursorPos + ImVec2(window->Size.x - 50, 10 + frame_size.y));
    const ImRect total_bb(frame_bb.Min, frame_bb.Min + total_size);

    const ImRect rect_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(window->Size.x - 8, 39));

    ImGuiWindow* draw_window = window;
    ImVec2 inner_size = frame_size;
    ImGuiItemStatusFlags item_status_flags = 0;
    ImGuiLastItemData item_data_backup;
    if (is_multiline)
    {
        ImVec2 backup_pos = window->DC.CursorPos;
        ItemSize(rect_bb, style.FramePadding.y);
        if (!ItemAdd(rect_bb, id, &frame_bb, ImGuiItemFlags_Inputable))
        {
            EndGroup();
            return false;
        }
        item_status_flags = g.LastItemData.StatusFlags;
        item_data_backup = g.LastItemData;
        window->DC.CursorPos = backup_pos;

        // We reproduce the contents of BeginChildFrame() in order to provide 'label' so our window internal data are easier to read/debug.
        // FIXME-NAV: Pressing NavActivate will trigger general child activation right before triggering our own below. Harmless but bizarre.
        PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);
        PushStyleVar(ImGuiStyleVar_ChildRounding, style.FrameRounding);
        PushStyleVar(ImGuiStyleVar_ChildBorderSize, style.FrameBorderSize);
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // Ensure no clip rect so mouse hover can reach FramePadding edges
        bool child_visible = BeginChildEx(label, id, frame_bb.GetSize(), true, ImGuiWindowFlags_NoMove);
        PopStyleVar(3);
        PopStyleColor();
        if (!child_visible)
        {
            EndChild();
            EndGroup();
            return false;
        }
        draw_window = g.CurrentWindow; // Child window
        draw_window->DC.NavLayersActiveMaskNext |= (1 << draw_window->DC.NavLayerCurrent); // This is to ensure that EndChild() will display a navigation highlight so we can "enter" into it.
        draw_window->DC.CursorPos += style.FramePadding;
        inner_size.x -= draw_window->ScrollbarSizes.x;
    }
    else
    {
        // Support for internal ImGuiInputTextFlags_MergedItem flag, which could be redesigned as an ItemFlags if needed (with test performed in ItemAdd)
        ItemSize(rect_bb, style.FramePadding.y);
        if (!(flags & ImGuiInputTextFlags_MergedItem))
            if (!ItemAdd(rect_bb, id, &frame_bb, ImGuiItemFlags_Inputable))
                return false;
        item_status_flags = g.LastItemData.StatusFlags;
    }
    const bool hovered = ItemHoverable(frame_bb, id);
    if (hovered)
        g.MouseCursor = ImGuiMouseCursor_TextInput;

    // We are only allowed to access the state if we are already the active widget.
    ImGuiInputTextState* state = GetInputTextState(id);

    const bool input_requested_by_tabbing = (item_status_flags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
    const bool input_requested_by_nav = (g.ActiveId != id) && ((g.NavActivateInputId == id) || (g.NavActivateId == id && g.NavInputSource == ImGuiInputSource_Keyboard));

    const bool user_clicked = hovered && io.MouseClicked[0];
    const bool user_scroll_finish = is_multiline && state != NULL && g.ActiveId == 0 && g.ActiveIdPreviousFrame == GetWindowScrollbarID(draw_window, ImGuiAxis_Y);
    const bool user_scroll_active = is_multiline && state != NULL && g.ActiveId == GetWindowScrollbarID(draw_window, ImGuiAxis_Y);
    bool clear_active_id = false;
    bool select_all = false;

    float scroll_y = is_multiline ? draw_window->Scroll.y : FLT_MAX;

    const bool init_changed_specs = (state != NULL && state->Stb.single_line != !is_multiline);
    const bool init_make_active = (user_clicked || user_scroll_finish || input_requested_by_nav || input_requested_by_tabbing);
    const bool init_state = (init_make_active || user_scroll_active);
    if ((init_state && g.ActiveId != id) || init_changed_specs)
    {
        // Access state even if we don't own it yet.
        state = &g.InputTextState;
        state->CursorAnimReset();

        // Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
        // From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
        const int buf_len = (int)strlen(buf);
        state->InitialTextA.resize(buf_len + 1);    // UTF-8. we use +1 to make sure that .Data is always pointing to at least an empty string.
        memcpy(state->InitialTextA.Data, buf, buf_len + 1);

        // Preserve cursor position and undo/redo stack if we come back to same widget
        // FIXME: Since we reworked this on 2022/06, may want to differenciate recycle_cursor vs recycle_undostate?
        bool recycle_state = (state->ID == id && !init_changed_specs);
        if (recycle_state && (state->CurLenA != buf_len || (state->TextAIsValid && strncmp(state->TextA.Data, buf, buf_len) != 0)))
            recycle_state = false;

        // Start edition
        const char* buf_end = NULL;
        state->ID = id;
        state->TextW.resize(buf_size + 1);          // wchar count <= UTF-8 count. we use +1 to make sure that .Data is always pointing to at least an empty string.
        state->TextA.resize(0);
        state->TextAIsValid = false;                // TextA is not valid yet (we will display buf until then)
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, buf_size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);      // We can't get the result from ImStrncpy() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.

        if (recycle_state)
        {
            // Recycle existing cursor/selection/undo stack but clamp position
            // Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
            state->CursorClamp();
        }
        else
        {
            state->ID = id;
            state->ScrollX = 0.0f;
            stb_textedit_initialize_state(&state->Stb, !is_multiline);
        }

        if (!is_multiline)
        {
            if (flags & ImGuiInputTextFlags_AutoSelectAll)
                select_all = true;
            if (input_requested_by_nav && (!recycle_state || !(g.NavActivateFlags & ImGuiActivateFlags_TryToPreserveState)))
                select_all = true;
            if (input_requested_by_tabbing || (user_clicked && io.KeyCtrl))
                select_all = true;
        }

        if (flags & ImGuiInputTextFlags_AlwaysOverwrite)
            state->Stb.insert_mode = 1; // stb field name is indeed incorrect (see #2863)
    }

    const bool is_osx = io.ConfigMacOSXBehaviors;
    if (g.ActiveId != id && init_make_active)
    {
        IM_ASSERT(state && state->ID == id);
        ImGui::SetActiveID(id, window);
        ImGui::SetFocusID(id, window);
        ImGui::FocusWindow(window);
    }
    if (g.ActiveId == id)
    {
        // Declare some inputs, the other are registered and polled via Shortcut() routing system.
        if (user_clicked)
            ImGui::SetKeyOwner(ImGuiKey_MouseLeft, id);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        if (is_multiline || (flags & ImGuiInputTextFlags_CallbackHistory))
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Up) | (1 << ImGuiDir_Down);
        ImGui::SetKeyOwner(ImGuiKey_Home, id);
        ImGui::SetKeyOwner(ImGuiKey_End, id);
        if (is_multiline)
        {
            ImGui::SetKeyOwner(ImGuiKey_PageUp, id);
            ImGui::SetKeyOwner(ImGuiKey_PageDown, id);
        }
        if (is_osx)
            ImGui::SetKeyOwner(ImGuiMod_Alt, id);
        if (flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_AllowTabInput)) // Disable keyboard tabbing out as we will use the \t character.
            ImGui::SetShortcutRouting(ImGuiKey_Tab, id);
    }

    // We have an edge case if ActiveId was set through another widget (e.g. widget being swapped), clear id immediately (don't wait until the end of the function)
    if (g.ActiveId == id && state == NULL)
        ClearActiveID();

    // Release focus when we click outside
    if (g.ActiveId == id && io.MouseClicked[0] && !init_state && !init_make_active) //-V560
        clear_active_id = true;

    // Lock the decision of whether we are going to take the path displaying the cursor or selection
    bool render_cursor = (g.ActiveId == id) || (state && user_scroll_active);
    bool render_selection = state && (state->HasSelection() || select_all) && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    bool value_changed = false;
    bool validated = false;

    // When read-only we always use the live data passed to the function
    // FIXME-OPT: Because our selection/cursor code currently needs the wide text we need to convert it when active, which is not ideal :(
    if (is_readonly && state != NULL && (render_cursor || render_selection))
    {
        const char* buf_end = NULL;
        state->TextW.resize(buf_size + 1);
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);
        state->CursorClamp();
        render_selection &= state->HasSelection();
    }

    // Select the buffer to render.
    const bool buf_display_from_state = (render_cursor || render_selection || g.ActiveId == id) && !is_readonly && state && state->TextAIsValid;
    const bool is_displaying_hint = (hint != NULL && (buf_display_from_state ? state->TextA.Data : buf)[0] == 0);

    // Password pushes a temporary font with only a fallback glyph
    if (is_password && !is_displaying_hint)
    {
        const ImFontGlyph* glyph = g.Font->FindGlyph('*');
        ImFont* password_font = &g.InputTextPasswordFont;
        password_font->FontSize = g.Font->FontSize;
        password_font->Scale = g.Font->Scale;
        password_font->Ascent = g.Font->Ascent;
        password_font->Descent = g.Font->Descent;
        password_font->ContainerAtlas = g.Font->ContainerAtlas;
        password_font->FallbackGlyph = glyph;
        password_font->FallbackAdvanceX = glyph->AdvanceX;
        IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
        PushFont(password_font);
    }

    // Process mouse inputs and character inputs
    int backup_current_text_length = 0;
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        backup_current_text_length = state->CurLenA;
        state->Edited = false;
        state->BufCapacityA = buf_size;
        state->Flags = flags;

        // Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
        // Down the line we should have a cleaner library-wide concept of Selected vs Active.
        g.ActiveIdAllowOverlap = !io.MouseDown[0];
        g.WantTextInputNextFrame = 1;

        // Edit in progress
        const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + state->ScrollX;
        const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y) : (g.FontSize * 0.5f));

        const bool is_osx = io.ConfigMacOSXBehaviors;
        if (select_all)
        {
            state->SelectAll();
            state->SelectedAllMouseLock = true;
        }
        else if (hovered && io.MouseClickedCount[0] >= 2 && !io.KeyShift)
        {
            stb_textedit_click(state, &state->Stb, mouse_x, mouse_y);
            const int multiclick_count = (io.MouseClickedCount[0] - 2);
            if ((multiclick_count % 2) == 0)
            {
                // Double-click: Select word
                // We always use the "Mac" word advance for double-click select vs CTRL+Right which use the platform dependent variant:
                // FIXME: There are likely many ways to improve this behavior, but there's no "right" behavior (depends on use-case, software, OS)
                const bool is_bol = (state->Stb.cursor == 0) || ImStb::STB_TEXTEDIT_GETCHAR(state, state->Stb.cursor - 1) == '\n';
                if (STB_TEXT_HAS_SELECTION(&state->Stb) || !is_bol)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
                //state->OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
                if (!STB_TEXT_HAS_SELECTION(&state->Stb))
                    ImStb::stb_textedit_prep_selection_at_cursor(&state->Stb);
                state->Stb.cursor = ImStb::STB_TEXTEDIT_MOVEWORDRIGHT_MAC(state, state->Stb.cursor);
                state->Stb.select_end = state->Stb.cursor;
                ImStb::stb_textedit_clamp(state, &state->Stb);
            }
            else
            {
                // Triple-click: Select line
                const bool is_eol = ImStb::STB_TEXTEDIT_GETCHAR(state, state->Stb.cursor) == '\n';
                state->OnKeyPressed(STB_TEXTEDIT_K_LINESTART);
                state->OnKeyPressed(STB_TEXTEDIT_K_LINEEND | STB_TEXTEDIT_K_SHIFT);
                state->OnKeyPressed(STB_TEXTEDIT_K_RIGHT | STB_TEXTEDIT_K_SHIFT);
                if (!is_eol && is_multiline)
                {
                    ImSwap(state->Stb.select_start, state->Stb.select_end);
                    state->Stb.cursor = state->Stb.select_end;
                }
                state->CursorFollow = false;
            }
            state->CursorAnimReset();
        }
        else if (io.MouseClicked[0] && !state->SelectedAllMouseLock)
        {
            if (hovered)
            {
                if (io.KeyShift)
                    stb_textedit_drag(state, &state->Stb, mouse_x, mouse_y);
                else
                    stb_textedit_click(state, &state->Stb, mouse_x, mouse_y);
                state->CursorAnimReset();
            }
        }
        else if (io.MouseDown[0] && !state->SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
        {
            stb_textedit_drag(state, &state->Stb, mouse_x, mouse_y);
            state->CursorAnimReset();
            state->CursorFollow = true;
        }
        if (state->SelectedAllMouseLock && !io.MouseDown[0])
            state->SelectedAllMouseLock = false;

        // We expect backends to emit a Tab key but some also emit a Tab character which we ignore (#2467, #1336)
        // (For Tab and Enter: Win32/SFML/Allegro are sending both keys and chars, GLFW and SDL are only sending keys. For Space they all send all threes)
        const bool ignore_char_inputs = (io.KeyCtrl && !io.KeyAlt) || (is_osx && io.KeySuper);
        if ((flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressed(ImGuiKey_Tab) && !ignore_char_inputs && !io.KeyShift && !is_readonly)
        {
            unsigned int c = '\t'; // Insert TAB
            if (InputTextFilterCharacter(&c, flags, callback, callback_user_data, ImGuiInputSource_Keyboard))
                state->OnKeyPressed((int)c);
        }

        // Process regular text input (before we check for Return because using some IME will effectively send a Return?)
        // We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
        if (io.InputQueueCharacters.Size > 0)
        {
            if (!ignore_char_inputs && !is_readonly && !input_requested_by_nav)
                for (int n = 0; n < io.InputQueueCharacters.Size; n++)
                {
                    // Insert character if they pass filtering
                    unsigned int c = (unsigned int)io.InputQueueCharacters[n];
                    if (c == '\t') // Skip Tab, see above.
                        continue;
                    if (InputTextFilterCharacter(&c, flags, callback, callback_user_data, ImGuiInputSource_Keyboard))
                        state->OnKeyPressed((int)c);
                }

            // Consume characters
            io.InputQueueCharacters.resize(0);
        }
    }

    // Process other shortcuts/key-presses
    bool revert_edit = false;
    if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
    {
        IM_ASSERT(state != NULL);
        IM_ASSERT(io.KeyMods == ImGui::GetMergedKeyModFlags() && "Mismatching io.KeyCtrl/io.KeyShift/io.KeyAlt/io.KeySuper vs io.KeyMods"); // We rarely do this check, but if anything let's do it here.

        const int row_count_per_page = ImMax((int)((inner_size.y - style.FramePadding.y) / g.FontSize), 1);
        state->Stb.row_count_per_page = row_count_per_page;

        const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        const bool is_wordmove_key_down = is_osx ? io.KeyAlt : io.KeyCtrl;                     // OS X style: Text editing cursor movement using Alt instead of Ctrl
        const bool is_startend_key_down = is_osx && io.KeySuper && !io.KeyCtrl && !io.KeyAlt;  // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End

        // Using Shortcut() with ImGuiInputFlags_RouteFocused (default policy) to allow routing operations for other code (e.g. calling window trying to use CTRL+A and CTRL+B: formet would be handled by InputText)
        // Otherwise we could simply assume that we own the keys as we are active.
        const ImGuiInputFlags f_repeat = ImGuiInputFlags_Repeat;
        const bool is_cut = (ImGui::Shortcut(ImGuiMod_Shortcut | ImGuiKey_X, id, f_repeat) || ImGui::Shortcut(ImGuiMod_Shift | ImGuiKey_Delete, id, f_repeat)) && !is_readonly && !is_password && (!is_multiline || state->HasSelection());
        const bool is_copy = (ImGui::Shortcut(ImGuiMod_Shortcut | ImGuiKey_C, id) || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Insert, id)) && !is_password && (!is_multiline || state->HasSelection());
        const bool is_paste = (ImGui::Shortcut(ImGuiMod_Shortcut | ImGuiKey_V, id, f_repeat) || ImGui::Shortcut(ImGuiMod_Shift | ImGuiKey_Insert, id, f_repeat)) && !is_readonly;
        const bool is_undo = (ImGui::Shortcut(ImGuiMod_Shortcut | ImGuiKey_Z, id, f_repeat)) && !is_readonly && is_undoable;
        const bool is_redo = (ImGui::Shortcut(ImGuiMod_Shortcut | ImGuiKey_Y, id, f_repeat) || (is_osx && ImGui::Shortcut(ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_Z, id, f_repeat))) && !is_readonly && is_undoable;
        const bool is_select_all = ImGui::Shortcut(ImGuiMod_Shortcut | ImGuiKey_A, id);

        // We allow validate/cancel with Nav source (gamepad) to makes it easier to undo an accidental NavInput press with no keyboard wired, but otherwise it isn't very useful.
        const bool nav_gamepad_active = (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) != 0 && (io.BackendFlags & ImGuiBackendFlags_HasGamepad) != 0;
        const bool is_enter_pressed = ImGui::IsKeyPressed(ImGuiKey_Enter, true) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, true);
        const bool is_gamepad_validate = nav_gamepad_active && (ImGui::IsKeyPressed(ImGuiKey_NavGamepadActivate, false) || ImGui::IsKeyPressed(ImGuiKey_NavGamepadInput, false));
        const bool is_cancel = ImGui::Shortcut(ImGuiKey_Escape, id, f_repeat) || (nav_gamepad_active && ImGui::Shortcut(ImGuiKey_NavGamepadCancel, id, f_repeat));

        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
        else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
        else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && is_multiline) { if (io.KeyCtrl) ImGui::SetScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f)); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask); }
        else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow) && is_multiline) { if (io.KeyCtrl) ImGui::SetScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, ImGui::GetScrollMaxY())); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask); }
        else if (ImGui::IsKeyPressed(ImGuiKey_PageUp) && is_multiline) { state->OnKeyPressed(STB_TEXTEDIT_K_PGUP | k_mask); scroll_y -= row_count_per_page * g.FontSize; }
        else if (ImGui::IsKeyPressed(ImGuiKey_PageDown) && is_multiline) { state->OnKeyPressed(STB_TEXTEDIT_K_PGDOWN | k_mask); scroll_y += row_count_per_page * g.FontSize; }
        else if (ImGui::IsKeyPressed(ImGuiKey_Home)) { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
        else if (ImGui::IsKeyPressed(ImGuiKey_End)) { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
        else if (ImGui::IsKeyPressed(ImGuiKey_Delete) && !is_readonly && !is_cut)
        {
            if (!state->HasSelection())
            {
                // OSX doesn't seem to have Super+Delete to delete until end-of-line, so we don't emulate that (as opposed to Super+Backspace)
                if (is_wordmove_key_down)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
            }
            state->OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask);
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Backspace) && !is_readonly)
        {
            if (!state->HasSelection())
            {
                if (is_wordmove_key_down)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
                else if (is_osx && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
                    state->OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
            }
            state->OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
        }
        else if (is_enter_pressed || is_gamepad_validate)
        {
            // Determine if we turn Enter into a \n character
            bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
            if (!is_multiline || is_gamepad_validate || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
            {
                validated = true;
                if (io.ConfigInputTextEnterKeepActive && !is_multiline)
                    state->SelectAll(); // No need to scroll
                else
                    clear_active_id = true;
            }
            else if (!is_readonly)
            {
                unsigned int c = '\n'; // Insert new line
                if (InputTextFilterCharacter(&c, flags, callback, callback_user_data, ImGuiInputSource_Keyboard))
                    state->OnKeyPressed((int)c);
            }
        }
        else if (is_cancel)
        {
            if (flags & ImGuiInputTextFlags_EscapeClearsAll)
            {
                if (buf[0] != 0)
                {
                    revert_edit = true;
                }
                else
                {
                    render_cursor = render_selection = false;
                    clear_active_id = true;
                }
            }
            else
            {
                clear_active_id = revert_edit = true;
                render_cursor = render_selection = false;
            }
        }
        else if (is_undo || is_redo)
        {
            state->OnKeyPressed(is_undo ? STB_TEXTEDIT_K_UNDO : STB_TEXTEDIT_K_REDO);
            state->ClearSelection();
        }
        else if (is_select_all)
        {
            state->SelectAll();
            state->CursorFollow = true;
        }
        else if (is_cut || is_copy)
        {
            // Cut, Copy
            if (io.SetClipboardTextFn)
            {
                const int ib = state->HasSelection() ? ImMin(state->Stb.select_start, state->Stb.select_end) : 0;
                const int ie = state->HasSelection() ? ImMax(state->Stb.select_start, state->Stb.select_end) : state->CurLenW;
                const int clipboard_data_len = ImTextCountUtf8BytesFromStr(state->TextW.Data + ib, state->TextW.Data + ie) + 1;
                char* clipboard_data = (char*)IM_ALLOC(clipboard_data_len * sizeof(char));
                ImTextStrToUtf8(clipboard_data, clipboard_data_len, state->TextW.Data + ib, state->TextW.Data + ie);
                ImGui::SetClipboardText(clipboard_data);
                ImGui::MemFree(clipboard_data);
            }
            if (is_cut)
            {
                if (!state->HasSelection())
                    state->SelectAll();
                state->CursorFollow = true;
                stb_textedit_cut(state, &state->Stb);
            }
        }
        else if (is_paste)
        {
            if (const char* clipboard = ImGui::GetClipboardText())
            {
                // Filter pasted buffer
                const int clipboard_len = (int)strlen(clipboard);
                ImWchar* clipboard_filtered = (ImWchar*)IM_ALLOC((clipboard_len + 1) * sizeof(ImWchar));
                int clipboard_filtered_len = 0;
                for (const char* s = clipboard; *s != 0; )
                {

                    unsigned int c;
                    s += ImTextCharFromUtf8(&c, s, NULL);
                    if (c == 0)
                        break;
                    if (!InputTextFilterCharacter(&c, flags, callback, callback_user_data, ImGuiInputSource_Clipboard))
                        continue;
                    clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
                }
                clipboard_filtered[clipboard_filtered_len] = 0;
                if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
                {
                    stb_textedit_paste(state, &state->Stb, clipboard_filtered, clipboard_filtered_len);
                    state->CursorFollow = true;
                }
                ImGui::MemFree(clipboard_filtered);
            }
        }

        // Update render selection flag after events have been handled, so selection highlight can be displayed during the same frame.
        render_selection |= state->HasSelection() && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    }

    // Process callbacks and apply result back to user's buffer.
    const char* apply_new_text = NULL;
    int apply_new_text_length = 0;
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        if (revert_edit && !is_readonly)
        {
            if (strcmp(buf, state->InitialTextA.Data) != 0)
            {
                // Restore initial value. Only return true if restoring to the initial value changes the current buffer contents.
                // Push records into the undo stack so we can CTRL+Z the revert operation itself
                apply_new_text = state->InitialTextA.Data;
                apply_new_text_length = state->InitialTextA.Size - 1;
                ImVector<ImWchar> w_text;
                if (apply_new_text_length > 0)
                {
                    w_text.resize(ImTextCountCharsFromUtf8(apply_new_text, apply_new_text + apply_new_text_length) + 1);
                    ImTextStrFromUtf8(w_text.Data, w_text.Size, apply_new_text, apply_new_text + apply_new_text_length);
                }
                stb_textedit_replace(state, &state->Stb, w_text.Data, (apply_new_text_length > 0) ? (w_text.Size - 1) : 0);
            }
        }

        // Apply ASCII value
        if (!is_readonly)
        {
            state->TextAIsValid = true;
            state->TextA.resize(state->TextW.Size * 4 + 1);
            ImTextStrToUtf8(state->TextA.Data, state->TextA.Size, state->TextW.Data, NULL);
        }

        // When using 'ImGuiInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
        // If we didn't do that, code like InputInt() with ImGuiInputTextFlags_EnterReturnsTrue would fail.
        // This also allows the user to use InputText() with ImGuiInputTextFlags_EnterReturnsTrue without maintaining any user-side storage (please note that if you use this property along ImGuiInputTextFlags_CallbackResize you can end up with your temporary string object unnecessarily allocating once a frame, either store your string data, either if you don't then don't use ImGuiInputTextFlags_CallbackResize).
        const bool apply_edit_back_to_user_buffer = !revert_edit || (validated && (flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0);
        if (apply_edit_back_to_user_buffer)
        {
            // Apply new value immediately - copy modified buffer back
            // Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
            // FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
            // FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.

            // User callback
            if ((flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_CallbackAlways)) != 0)
            {
                IM_ASSERT(callback != NULL);

                // The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
                ImGuiInputTextFlags event_flag = 0;
                ImGuiKey event_key = ImGuiKey_COUNT;
                if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressed(ImGuiKey_Tab))
                {
                    event_flag = ImGuiInputTextFlags_CallbackCompletion;
                    event_key = ImGuiKey_Tab;
                }
                else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressed(ImGuiKey_UpArrow))
                {
                    event_flag = ImGuiInputTextFlags_CallbackHistory;
                    event_key = ImGuiKey_UpArrow;
                }
                else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressed(ImGuiKey_DownArrow))
                {
                    event_flag = ImGuiInputTextFlags_CallbackHistory;
                    event_key = ImGuiKey_DownArrow;
                }
                else if ((flags & ImGuiInputTextFlags_CallbackEdit) && state->Edited)
                {
                    event_flag = ImGuiInputTextFlags_CallbackEdit;
                }
                else if (flags & ImGuiInputTextFlags_CallbackAlways)
                {
                    event_flag = ImGuiInputTextFlags_CallbackAlways;
                }

                if (event_flag)
                {
                    ImGuiInputTextCallbackData callback_data;
                    memset(&callback_data, 0, sizeof(ImGuiInputTextCallbackData));
                    callback_data.EventFlag = event_flag;
                    callback_data.Flags = flags;
                    callback_data.UserData = callback_user_data;

                    char* callback_buf = is_readonly ? buf : state->TextA.Data;
                    callback_data.EventKey = event_key;
                    callback_data.Buf = callback_buf;
                    callback_data.BufTextLen = state->CurLenA;
                    callback_data.BufSize = state->BufCapacityA;
                    callback_data.BufDirty = false;

                    // We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
                    ImWchar* text = state->TextW.Data;
                    const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + state->Stb.cursor);
                    const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_start);
                    const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_end);

                    // Call user code
                    callback(&callback_data);

                    // Read back what user may have modified
                    callback_buf = is_readonly ? buf : state->TextA.Data; // Pointer may have been invalidated by a resize callback
                    IM_ASSERT(callback_data.Buf == callback_buf);         // Invalid to modify those fields
                    IM_ASSERT(callback_data.BufSize == state->BufCapacityA);
                    IM_ASSERT(callback_data.Flags == flags);
                    const bool buf_dirty = callback_data.BufDirty;
                    if (callback_data.CursorPos != utf8_cursor_pos || buf_dirty) { state->Stb.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos); state->CursorFollow = true; }
                    if (callback_data.SelectionStart != utf8_selection_start || buf_dirty) { state->Stb.select_start = (callback_data.SelectionStart == callback_data.CursorPos) ? state->Stb.cursor : ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart); }
                    if (callback_data.SelectionEnd != utf8_selection_end || buf_dirty) { state->Stb.select_end = (callback_data.SelectionEnd == callback_data.SelectionStart) ? state->Stb.select_start : ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd); }
                    if (buf_dirty)
                    {
                        IM_ASSERT((flags & ImGuiInputTextFlags_ReadOnly) == 0);
                        IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
                        InputTextReconcileUndoStateAfterUserCallback(state, callback_data.Buf, callback_data.BufTextLen); // FIXME: Move the rest of this block inside function and rename to InputTextReconcileStateAfterUserCallback() ?
                        if (callback_data.BufTextLen > backup_current_text_length && is_resizable)
                            state->TextW.resize(state->TextW.Size + (callback_data.BufTextLen - backup_current_text_length)); // Worse case scenario resize
                        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, callback_data.Buf, NULL);
                        state->CurLenA = callback_data.BufTextLen;  // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
                        state->CursorAnimReset();
                    }
                }
            }

            // Will copy result string if modified
            if (!is_readonly && strcmp(state->TextA.Data, buf) != 0)
            {
                apply_new_text = state->TextA.Data;
                apply_new_text_length = state->CurLenA;
            }
        }
    }

    // Copy result to user buffer. This can currently only happen when (g.ActiveId == id)
    if (apply_new_text != NULL)
    {
        // We cannot test for 'backup_current_text_length != apply_new_text_length' here because we have no guarantee that the size
        // of our owned buffer matches the size of the string object held by the user, and by design we allow InputText() to be used
        // without any storage on user's side.
        IM_ASSERT(apply_new_text_length >= 0);
        if (is_resizable)
        {
            ImGuiInputTextCallbackData callback_data;
            callback_data.EventFlag = ImGuiInputTextFlags_CallbackResize;
            callback_data.Flags = flags;
            callback_data.Buf = buf;
            callback_data.BufTextLen = apply_new_text_length;
            callback_data.BufSize = ImMax(buf_size, apply_new_text_length + 1);
            callback_data.UserData = callback_user_data;
            callback(&callback_data);
            buf = callback_data.Buf;
            buf_size = callback_data.BufSize;
            apply_new_text_length = ImMin(callback_data.BufTextLen, buf_size - 1);
            IM_ASSERT(apply_new_text_length <= buf_size);
        }
        //IMGUI_DEBUG_PRINT("InputText(\"%s\"): apply_new_text length %d\n", label, apply_new_text_length);

        // If the underlying buffer resize was denied or not carried to the next frame, apply_new_text_length+1 may be >= buf_size.
        ImStrncpy(buf, apply_new_text, ImMin(apply_new_text_length + 1, buf_size));
        value_changed = true;
    }

    // Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
    if (clear_active_id && g.ActiveId == id)
        ClearActiveID();

    window->DrawList->AddRectFilled(rect_bb.Min, rect_bb.Max, ImColor(0.03f, 0.03f, 0.03f, hovered || IsItemActive() ? 0.13f : 0.1f), 2.0f);

    // Render frame
    if (!is_multiline)
    {
        RenderNavHighlight(frame_bb, id);
        RenderFrame(frame_bb.Min, frame_bb.Max, ImColor(0.03f, 0.03f, 0.03f, 1.0f), false, 3.0f);
    }

    const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + inner_size.x, frame_bb.Min.y + inner_size.y); // Not using frame_bb.Max because we have adjusted size
    ImVec2 draw_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
    ImVec2 text_size(0.0f, 0.0f);

    // Set upper limit of single-line InputTextEx() at 2 million characters strings. The current pathological worst case is a long line
    // without any carriage return, which would makes ImFont::RenderText() reserve too many vertices and probably crash. Avoid it altogether.
    // Note that we only use this limit on single-line InputText(), so a pathologically large line on a InputTextMultiline() would still crash.
    const int buf_display_max_length = 2 * 1024 * 1024;
    const char* buf_display = buf_display_from_state ? state->TextA.Data : buf; //-V595
    const char* buf_display_end = NULL; // We have specialized paths below for setting the length
    if (is_displaying_hint)
    {
        buf_display = hint;
        buf_display_end = hint + strlen(hint);
    }

    // Render text. We currently only render selection when the widget is active or while scrolling.
    // FIXME: We could remove the '&& render_cursor' to keep rendering selection when inactive.
    if (render_cursor || render_selection)
    {
        IM_ASSERT(state != NULL);
        if (!is_displaying_hint)
            buf_display_end = buf_display + state->CurLenA;

        // Render text (with cursor and selection)
        // This is going to be messy. We need to:
        // - Display the text (this alone can be more easily clipped)
        // - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
        // - Measure text height (for scrollbar)
        // We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
        // FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
        const ImWchar* text_begin = state->TextW.Data;
        ImVec2 cursor_offset, select_start_offset;

        {
            // Find lines numbers straddling 'cursor' (slot 0) and 'select_start' (slot 1) positions.
            const ImWchar* searches_input_ptr[2] = { NULL, NULL };
            int searches_result_line_no[2] = { -1000, -1000 };
            int searches_remaining = 0;
            if (render_cursor)
            {
                searches_input_ptr[0] = text_begin + state->Stb.cursor;
                searches_result_line_no[0] = -1;
                searches_remaining++;
            }
            if (render_selection)
            {
                searches_input_ptr[1] = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
                searches_result_line_no[1] = -1;
                searches_remaining++;
            }

            // Iterate all lines to find our line numbers
            // In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
            searches_remaining += is_multiline ? 1 : 0;
            int line_count = 0;
            //for (const ImWchar* s = text_begin; (s = (const ImWchar*)wcschr((const wchar_t*)s, (wchar_t)'\n')) != NULL; s++)  // FIXME-OPT: Could use this when wchar_t are 16-bit
            for (const ImWchar* s = text_begin; *s != 0; s++)
                if (*s == '\n')
                {
                    line_count++;
                    if (searches_result_line_no[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_no[0] = line_count; if (--searches_remaining <= 0) break; }
                    if (searches_result_line_no[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_no[1] = line_count; if (--searches_remaining <= 0) break; }
                }
            line_count++;
            if (searches_result_line_no[0] == -1)
                searches_result_line_no[0] = line_count;
            if (searches_result_line_no[1] == -1)
                searches_result_line_no[1] = line_count;

            // Calculate 2d position by finding the beginning of the line and measuring distance
            cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
            cursor_offset.y = searches_result_line_no[0] * g.FontSize;
            if (searches_result_line_no[1] >= 0)
            {
                select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
                select_start_offset.y = searches_result_line_no[1] * g.FontSize;
            }

            // Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
            if (is_multiline)
                text_size = ImVec2(inner_size.x, line_count * g.FontSize);
        }

        // Scroll
        if (render_cursor && state->CursorFollow)
        {
            // Horizontal scroll in chunks of quarter width
            if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll))
            {
                const float scroll_increment_x = inner_size.x * 0.25f;
                const float visible_width = inner_size.x - style.FramePadding.x;
                if (cursor_offset.x < state->ScrollX)
                    state->ScrollX = IM_FLOOR(ImMax(0.0f, cursor_offset.x - scroll_increment_x));
                else if (cursor_offset.x - visible_width >= state->ScrollX)
                    state->ScrollX = IM_FLOOR(cursor_offset.x - visible_width + scroll_increment_x);
            }
            else
            {
                state->ScrollX = 0.0f;
            }

            // Vertical scroll
            if (is_multiline)
            {
                // Test if cursor is vertically visible
                if (cursor_offset.y - g.FontSize < scroll_y)
                    scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
                else if (cursor_offset.y - (inner_size.y - style.FramePadding.y * 2.0f) >= scroll_y)
                    scroll_y = cursor_offset.y - inner_size.y + style.FramePadding.y * 2.0f;
                const float scroll_max_y = ImMax((text_size.y + style.FramePadding.y * 2.0f) - inner_size.y, 0.0f);
                scroll_y = ImClamp(scroll_y, 0.0f, scroll_max_y);
                draw_pos.y += (draw_window->Scroll.y - scroll_y);   // Manipulate cursor pos immediately avoid a frame of lag
                draw_window->Scroll.y = scroll_y;
            }

            state->CursorFollow = false;
        }

        // Draw selection
        const ImVec2 draw_scroll = ImVec2(state->ScrollX, 0.0f);
        if (render_selection)
        {
            const ImWchar* text_selected_begin = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
            const ImWchar* text_selected_end = text_begin + ImMax(state->Stb.select_start, state->Stb.select_end);

            ImU32 bg_color = GetColorU32(ImGuiCol_TextSelectedBg, render_cursor ? 1.0f : 0.6f); // FIXME: current code flow mandate that render_cursor is always true here, we are leaving the transparent one for tests.
            float bg_offy_up = is_multiline ? 0.0f : -1.0f;    // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
            float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
            ImVec2 rect_pos = draw_pos + select_start_offset - draw_scroll;
            for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
            {
                if (rect_pos.y > clip_rect.w + g.FontSize)
                    break;
                if (rect_pos.y < clip_rect.y)
                {
                    //p = (const ImWchar*)wmemchr((const wchar_t*)p, '\n', text_selected_end - p);  // FIXME-OPT: Could use this when wchar_t are 16-bit
                    //p = p ? p + 1 : text_selected_end;
                    while (p < text_selected_end)
                        if (*p++ == '\n')
                            break;
                }
                else
                {
                    ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
                    if (rect_size.x <= 0.0f) rect_size.x = IM_FLOOR(g.Font->GetCharAdvance((ImWchar)' ') * 0.50f); // So we can see selected empty lines
                    ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
                    rect.ClipWith(clip_rect);
                    if (rect.Overlaps(clip_rect))
                        draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
                }
                rect_pos.x = draw_pos.x - draw_scroll.x;
                rect_pos.y += g.FontSize;
            }
        }

        // We test for 'buf_display_max_length' as a way to avoid some pathological cases (e.g. single-line 1 MB string) which would make ImDrawList crash.
        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? ImGuiCol_TextDisabled : ImGuiCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, draw_pos - draw_scroll, col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }

        // Draw blinking cursor
        if (render_cursor)
        {
            state->CursorAnim += io.DeltaTime;
            bool cursor_is_visible = (!g.IO.ConfigInputTextCursorBlink) || (state->CursorAnim <= 0.0f) || ImFmod(state->CursorAnim, 1.20f) <= 0.80f;
            ImVec2 cursor_screen_pos = ImFloor(draw_pos + cursor_offset - draw_scroll);
            ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
            if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
                draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(ImGuiCol_Text));

            // Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
            if (!is_readonly)
            {
                g.PlatformImeData.WantVisible = true;
                g.PlatformImeData.InputPos = ImVec2(cursor_screen_pos.x - 1.0f, cursor_screen_pos.y - g.FontSize);
                g.PlatformImeData.InputLineHeight = g.FontSize;
            }
        }
    }
    else
    {
        // Render text only (no selection, no cursor)
        if (is_multiline)
            text_size = ImVec2(inner_size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_display_end) * g.FontSize); // We don't need width
        else if (!is_displaying_hint && g.ActiveId == id)
            buf_display_end = buf_display + state->CurLenA;
        else if (!is_displaying_hint)
            buf_display_end = buf_display + strlen(buf_display);

        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? ImGuiCol_TextDisabled : ImGuiCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, draw_pos, col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }
    }

    if (is_password && !is_displaying_hint)
        PopFont();

    if (is_multiline)
    {
        // For focus requests to work on our multiline we need to ensure our child ItemAdd() call specifies the ImGuiItemFlags_Inputable (ref issue #4761)...
        Dummy(ImVec2(text_size.x, text_size.y + style.FramePadding.y));
        ImGuiItemFlags backup_item_flags = g.CurrentItemFlags;
        g.CurrentItemFlags |= ImGuiItemFlags_Inputable | ImGuiItemFlags_NoTabStop;
        EndChild();
        item_data_backup.StatusFlags |= (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredWindow);
        g.CurrentItemFlags = backup_item_flags;

        // ...and then we need to undo the group overriding last item data, which gets a bit messy as EndGroup() tries to forward scrollbar being active...
        // FIXME: This quite messy/tricky, should attempt to get rid of the child window.
        EndGroup();
        if (g.LastItemData.ID == 0)
        {
            g.LastItemData.ID = id;
            g.LastItemData.InFlags = item_data_backup.InFlags;
            g.LastItemData.StatusFlags = item_data_backup.StatusFlags;
        }
    }

    // Log as text
    if (g.LogEnabled && (!is_password || is_displaying_hint))
    {
        LogSetNextTextDecoration("{", "}");
        LogRenderedText(&draw_pos, buf_display, buf_display_end);
    }

    if (label_size.x > 0) {
        ImVec2 label_pos = ImVec2(rect_bb.Min.x + 10, (rect_bb.Min.y + rect_bb.Max.y) / 2 - label_size.y / 2);

        RenderText(label_pos, label);
    }

    if (value_changed && !(flags & ImGuiInputTextFlags_NoMarkEdited))
        MarkItemEdited(id);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
        return validated;
    else
        return value_changed;
}

bool elements::input_text(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    return input_text_ex(label, NULL, buf, (int)buf_size, ImVec2(150, 0), flags, callback, user_data);
}


ImVec2 elements::CalcWindowExpectedSize(ImGuiWindow* window)
{
    ImVec2 size_contents = elements::CalcWindowContentSize(window);
    ImVec2 size_auto_fit = elements::CalcWindowAutoFitSize(window, size_contents);
    ImVec2 size_final = elements::CalcWindowSizeAfterConstraint(window, size_auto_fit);
    return size_final;
}
ImVec2 elements::CalcWindowContentSize(ImGuiWindow* window)
{
    if (window->Collapsed)
        if (window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
            return window->ContentSize;
    if (window->Hidden && window->HiddenFramesCannotSkipItems == 0 && window->HiddenFramesCanSkipItems > 0)
        return window->ContentSize;

    ImVec2 sz;
    sz.x = IM_FLOOR((window->ContentSizeExplicit.x != 0.0f) ? window->ContentSizeExplicit.x : window->DC.CursorMaxPos.x - window->DC.CursorStartPos.x);
    sz.y = IM_FLOOR((window->ContentSizeExplicit.y != 0.0f) ? window->ContentSizeExplicit.y : window->DC.CursorMaxPos.y - window->DC.CursorStartPos.y);
    return sz;
}
ImRect elements::GetWindowAllowedExtentRect(ImGuiWindow* window)
{
    IM_UNUSED(window);
    ImVec2 padding = GImGui->Style.DisplaySafeAreaPadding;
    ImRect r_screen = elements::GetViewportRect();
    r_screen.Expand(ImVec2((r_screen.GetWidth() > padding.x * 2) ? -padding.x : 0.0f, (r_screen.GetHeight() > padding.y * 2) ? -padding.y : 0.0f));
    return r_screen;
}

ImVec2 elements::CalcWindowAutoFitSize(ImGuiWindow* window, const ImVec2& size_contents)
{
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    ImVec2 size_decorations = ImVec2(0.0f, window->TitleBarHeight() + window->MenuBarHeight());
    ImVec2 size_pad = window->WindowPadding * 2.0f;
    ImVec2 size_desired = size_contents + size_pad + size_decorations;
    if (window->Flags & ImGuiWindowFlags_Tooltip)
    {
        // Tooltip always resize
        return size_desired;
    }
    else
    {
        // Maximum window size is determined by the viewport size or monitor size
        const bool is_popup = (window->Flags & ImGuiWindowFlags_Popup) != 0;
        const bool is_menu = (window->Flags & ImGuiWindowFlags_ChildMenu) != 0;
        ImVec2 size_min = style.WindowMinSize;
        if (is_popup || is_menu) // Popups and menus bypass style.WindowMinSize by default, but we give then a non-zero minimum size to facilitate understanding problematic cases (e.g. empty popups)
            size_min = ImMin(size_min, ImVec2(4.0f, 4.0f));
        ImVec2 size_auto_fit = ImClamp(size_desired, size_min, ImMax(size_min, g.IO.DisplaySize - style.DisplaySafeAreaPadding * 2.0f));

        // When the window cannot fit all contents (either because of constraints, either because screen is too small),
        // we are growing the size on the other axis to compensate for expected scrollbar. FIXME: Might turn bigger than ViewportSize-WindowPadding.
        ImVec2 size_auto_fit_after_constraint = elements::CalcWindowSizeAfterConstraint(window, size_auto_fit);
        bool will_have_scrollbar_x = (size_auto_fit_after_constraint.x - size_pad.x - size_decorations.x < size_contents.x && !(window->Flags& ImGuiWindowFlags_NoScrollbar) && (window->Flags& ImGuiWindowFlags_HorizontalScrollbar)) || (window->Flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar);
        bool will_have_scrollbar_y = (size_auto_fit_after_constraint.y - size_pad.y - size_decorations.y < size_contents.y && !(window->Flags& ImGuiWindowFlags_NoScrollbar)) || (window->Flags & ImGuiWindowFlags_AlwaysVerticalScrollbar);
        if (will_have_scrollbar_x)
            size_auto_fit.y += style.ScrollbarSize;
        if (will_have_scrollbar_y)
            size_auto_fit.x += style.ScrollbarSize;
        return size_auto_fit;
    }
}

ImVec2 elements::CalcWindowSizeAfterConstraint(ImGuiWindow* window, ImVec2 new_size)
{
    ImGuiContext& g = *GImGui;
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        // Using -1,-1 on either X/Y axis to preserve the current size.
        ImRect cr = g.NextWindowData.SizeConstraintRect;
        new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
        new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
        if (g.NextWindowData.SizeCallback)
        {
            ImGuiSizeCallbackData data;
            data.UserData = g.NextWindowData.SizeCallbackUserData;
            data.Pos = window->Pos;
            data.CurrentSize = window->SizeFull;
            data.DesiredSize = new_size;
            g.NextWindowData.SizeCallback(&data);
            new_size = data.DesiredSize;
        }
        new_size.x = IM_FLOOR(new_size.x);
        new_size.y = IM_FLOOR(new_size.y);
    }

    // Minimum size
    if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
    {
        ImGuiWindow* window_for_height = window;
        new_size = ImMax(new_size, g.Style.WindowMinSize);
        new_size.y = ImMax(new_size.y, window_for_height->TitleBarHeight() + window_for_height->MenuBarHeight() + ImMax(0.0f, g.Style.WindowRounding - 1.0f)); // Reduce artifacts with very small windows
    }
    return new_size;
}

ImRect elements::GetViewportRect()
{
    ImGuiContext& g = *GImGui;
    return ImRect(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
}

void elements::Theme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->WindowBorderSize = 0;
    style->WindowRounding = 12.f;
    style->FrameRounding = 4.f;
    style->WindowPadding = ImVec2(0, 0);
    style->ItemSpacing = ImVec2(0, 5);
    style->ChildBorderSize = 0;
    style->ChildRounding = 12.f;
    style->ScrollbarRounding = 15.f;
    style->ScrollbarSize = 0.0f;
    style->PopupRounding = 4.f;
    style->PopupBorderSize = 0;

    colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 255 / 255.f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(9 / 255.f, 9 / 255.f, 9 / 255.f, 200 / 255.f);
    colors[ImGuiCol_ChildBg] = ImVec4(11 / 255.f, 11 / 255.f, 11 / 255.f, 120 / 255.f);
    colors[ImGuiCol_PopupBg] = ImVec4(18 / 255.f, 18 / 255.f, 18 / 255.f, 200 / 255.f);
    colors[ImGuiCol_Border] = ImVec4(23 / 255.f, 22 / 255.f, 27 / 255.f, 255 / 255.f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.40f, 0.40f, 0.90f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(33 / 255.f, 34 / 255.f, 41 / 255.f, 180 / 255.f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(30 / 255.f, 31 / 255.f, 37 / 255.f, 200 / 255.f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(30 / 255.f, 31 / 255.f, 37 / 255.f, 200 / 255.f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(35 / 255.f, 39 / 255.f, 48 / 255.f, 0.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(35 / 255.f, 39 / 255.f, 48 / 255.f, 0.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(35 / 255.f, 39 / 255.f, 48 / 255.f, 0.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(27 / 255.f, 34 / 255.f, 44 / 255.f, 50 / 255.f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(27 / 255.f, 34 / 255.f, 44 / 255.f, 50 / 255.f);
    colors[ImGuiCol_ButtonActive] = ImVec4(27 / 255.f, 34 / 255.f, 44 / 255.f, 50 / 255.f);
    colors[ImGuiCol_Header] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 0.10f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 0.10f);
    colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
    colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
    colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
    colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(90 / 255.f, 54 / 255.f, 124 / 255.f, 80 / 255.f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

}

void elements::multi_combo(const char* label, bool combos[], std::vector<std::string> list)
{
    std::string preview;
    std::vector<std::string> vec;

    for (int i = 0, j = 0; i < list.size(); i++)
    {
        if (combos[i])
        {
            vec.push_back(list[i]);
            if (j > 2)
                preview = vec.at(0) + ", " + vec.at(1) + ", " + vec.at(2) + ", ...";
            else if (j)
                preview += ", " + (std::string)list[i];
            else
                preview = list[i];

            j++;
        }
    }
    if (begincombo(label, preview.c_str(), list.size(), ImGuiComboFlags_None))
    {
        for (int i = 0; i < list.size(); i++) {
            selectable(list[i].c_str(), &combos[i], ImGuiSelectableFlags_DontClosePopups, ImVec2(0, 20));
        }
        EndPopup();
    }

    preview = "None";
}
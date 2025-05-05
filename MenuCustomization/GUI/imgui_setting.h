#pragma once

#include "imgui.h"

namespace settings {

    inline ImColor general_color = ImColor(31, 130, 165, 255);

    namespace tabs {

        inline ImColor text_active;
        inline ImColor text_hov;
        inline ImColor text;
    };
}

inline int tabs = 0, sub_tabs = 0;
#pragma once

#include <string_view>
#include <functional>

#include <imgui/imgui.h>


/** The c++-latest module of libImGui is intended for wrappers and other conveniences that make
    use of modern features of new versions of C++.

    No other extensions should be added here - that is left for other modules, which hovewer
    will then in turn required those same C++ features.
 */

namespace ImGui {

    struct InputTextFlags {

        int CharsDecimal : 1 = 0;
        int CharsHexadecimal : 1 = 0;
        int CharsUppercase : 1 = 0;
        int CharsNoBlank : 1 = 0;
        int AutoSelectAll : 1 = 0;
        int EnterReturnsTrue : 1 = 0;
        int CallbackCompletion : 1 = 0;
        int CallbackHistory : 1 = 0;
        int CallbackAlways : 1 = 0;
        int CallbackCharFilter : 1 = 0;
        int AllowTabInput : 1 = 0;
        int CtrlEnterForNewLine : 1 = 0;
        int NoHorizontalScroll : 1 = 0;
        int AlwaysInsertMode : 1 = 0;
        int ReadOnly : 1 = 0;
        int Password : 1 = 0;
        int NoUndoRedo : 1 = 0;
        int CharsScientific : 1 = 0;
        int CallbackResize : 1 = 0;

        operator ImGuiInputTextFlags() const {

            ImGuiInputTextFlags value = 0;
            if (CharsDecimal       ) value |= ImGuiInputTextFlags_CharsDecimal       ;
            if (CharsHexadecimal   ) value |= ImGuiInputTextFlags_CharsHexadecimal   ;
            if (CharsUppercase     ) value |= ImGuiInputTextFlags_CharsUppercase     ;
            if (CharsNoBlank       ) value |= ImGuiInputTextFlags_CharsNoBlank       ;
            if (AutoSelectAll      ) value |= ImGuiInputTextFlags_AutoSelectAll      ;
            if (EnterReturnsTrue   ) value |= ImGuiInputTextFlags_EnterReturnsTrue   ;
            if (CallbackCompletion ) value |= ImGuiInputTextFlags_CallbackCompletion ;
            if (CallbackHistory    ) value |= ImGuiInputTextFlags_CallbackHistory    ;
            if (CallbackAlways     ) value |= ImGuiInputTextFlags_CallbackAlways     ;
            if (CallbackCharFilter ) value |= ImGuiInputTextFlags_CallbackCharFilter ;
            if (AllowTabInput      ) value |= ImGuiInputTextFlags_AllowTabInput      ;
            if (CtrlEnterForNewLine) value |= ImGuiInputTextFlags_CtrlEnterForNewLine;
            if (NoHorizontalScroll ) value |= ImGuiInputTextFlags_NoHorizontalScroll ;
            if (AlwaysInsertMode   ) value |= ImGuiInputTextFlags_AlwaysInsertMode   ;
            if (ReadOnly           ) value |= ImGuiInputTextFlags_ReadOnly           ;
            if (Password           ) value |= ImGuiInputTextFlags_Password           ;
            if (NoUndoRedo         ) value |= ImGuiInputTextFlags_NoUndoRedo         ;
            if (CharsScientific    ) value |= ImGuiInputTextFlags_CharsScientific    ;
            if (CallbackResize     ) value |= ImGuiInputTextFlags_CallbackResize     ;
            if (CharsDecimal       ) value |= ImGuiInputTextFlags_CharsDecimal       ;
            return value;
        }
    };

    static_assert(sizeof(ImGui::InputTextFlags) <= sizeof(int));

    template <size_t LabelSize, size_t BufferSize, typename UserData>
    bool InputText(const char (&label)[LabelSize], char (&buffer)[BufferSize], ImGui::InputTextFlags flags,
        int (*callback)(ImGuiInputTextCallbackData*) = nullptr,
        UserData *user_data = nullptr
    ) {

        return ImGui::InputText(label, buffer, BufferSize, (int)flags, callback, user_data);
    }

} // ns ImGui

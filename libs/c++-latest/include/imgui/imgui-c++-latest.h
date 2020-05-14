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

    /** Namespace-based variant of ImGuiInputTextFlags 
     */
    struct InputTextFlags {

        static const int CharsDecimal        = ImGuiInputTextFlags_CharsDecimal;
        static const int CharsHexadecimal    = ImGuiInputTextFlags_CharsHexadecimal;
        static const int CharsUppercase      = ImGuiInputTextFlags_CharsUppercase;
        static const int CharsNoBlank        = ImGuiInputTextFlags_CharsNoBlank;
        static const int AutoSelectAll       = ImGuiInputTextFlags_AutoSelectAll;
        static const int EnterReturnsTrue    = ImGuiInputTextFlags_EnterReturnsTrue;
        static const int CallbackCompletion  = ImGuiInputTextFlags_CallbackCompletion;
        static const int CallbackHistory     = ImGuiInputTextFlags_CallbackHistory;
        static const int CallbackAlways      = ImGuiInputTextFlags_CallbackAlways;
        static const int CallbackCharFilter  = ImGuiInputTextFlags_CallbackCharFilter;
        static const int AllowTabInput       = ImGuiInputTextFlags_AllowTabInput;
        static const int CtrlEnterForNewLine = ImGuiInputTextFlags_CtrlEnterForNewLine;
        static const int NoHorizontalScroll  = ImGuiInputTextFlags_NoHorizontalScroll;
        static const int AlwaysInsertMode    = ImGuiInputTextFlags_AlwaysInsertMode;
        static const int ReadOnly            = ImGuiInputTextFlags_ReadOnly;
        static const int Password            = ImGuiInputTextFlags_Password;
        static const int NoUndoRedo          = ImGuiInputTextFlags_NoUndoRedo;
        static const int CharsScientific     = ImGuiInputTextFlags_CharsScientific;
        static const int CallbackResize      = ImGuiInputTextFlags_CallbackResize;

        ImGuiInputTextFlags flags;

        InputTextFlags(ImGuiInputTextFlags flags) : flags{flags} {}

        operator ImGuiInputTextFlags () const { return flags; }
    };

    static_assert(sizeof(ImGui::InputTextFlags) <= sizeof(int));

    // INPUTTEXT VARIANTS

    template <size_t BufferSize, typename UserData>
    bool InputText(const char *label, char (&buffer)[BufferSize], ImGui::InputTextFlags flags,
        int (*callback)(ImGuiInputTextCallbackData*) = nullptr,
        UserData *user_data = nullptr
    ) {

        return ImGui::InputText(label, buffer, BufferSize, (int)flags, callback, user_data);
    }

    // TODO: more overloads, and also for InputTextEx()

} // ns ImGui

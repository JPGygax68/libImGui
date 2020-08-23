#pragma once

#include <string_view>
#include <string>
#include <functional>
#include <limits>
#include <optional>
// #include <cstdarg>

#include <fmt/core.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


/** The c++-latest module of libImGui is intended for wrappers and other conveniences that make
    use of modern features of new versions of C++.

    No other extensions should be added here - that is left for other modules, which hovewer
    will then in turn require those same C++ features.
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

    // Convert several data types to either a label or an ID

    template <typename T>
    inline auto to_label(T value) { return std::to_string(value); }

    template <>
    inline auto to_label(const char *label) { return label; }


    // Text() variants ------------------------------------

    template <typename ...Args>
    inline void FormattedText(std::string_view tmpl, Args... args) {

        auto text = fmt::format(tmpl, args...);

        ImGui::TextUnformatted(text.data(), text.data() + text.size());
    }


    // Button variants ------------------------------------

    struct ButtonOptions {

        std::optional<ImVec2>                   size;
        std::optional<std::string_view>         width;
        std::optional<std::string_view>         height;

        auto get_extents() -> ImVec2 {

            if (size) {
                return *size;
            }
            else {
                ImVec2 extents = { 0, 0 };
                if (width ) extents.x = parse_length(*width);
                if (height) extents.y = parse_length(*height);
                return extents;
            }
        }

    private:

        int parse_length(std::string_view spec) {

            if (spec.ends_with("em")) {
                auto value = std::stoi(std::string{spec});
                return round( GetFontSize() * value );
            }
            else
                return std::stoi(std::string{spec});
        }
    };

    inline bool Button(std::string_view label_, ButtonOptions options = {}) {
        
        std::string label{label_};

        return Button(label.c_str(), options.get_extents() );
    }

    // InputText variants ---------------------------------

    /* Using templatized user data type
    */
    template <typename UserData>
    inline bool InputText(const char *label, char *buffer, size_t b_size, ImGui::InputTextFlags flags = 0,
        int (*callback)(ImGuiInputTextCallbackData*) = nullptr,
        UserData* user_data = nullptr
    ) {

        return ImGui::InputText(label, buffer, b_size, (int)flags, callback, user_data);
    }

    /* Using templatized user data; label specified as a std::string
     */
    template <typename UserData>
    inline bool InputText(const std::string& label, char* buffer, size_t b_size, ImGui::InputTextFlags flags = 0,
        int (*callback)(ImGuiInputTextCallbackData*) = nullptr,
        UserData* user_data = nullptr
    ) {

        return ImGui::InputText(label.c_str(), buffer, b_size, (int)flags, callback, user_data);
    }

    /* Templatized userdata and label/ID; C array buffer
    */
    template <typename Label, size_t BufferSize, typename UserData>
    inline bool InputText(Label label, char (&buffer)[BufferSize], ImGui::InputTextFlags flags = 0,
        int (*callback)(ImGuiInputTextCallbackData*) = nullptr,
        UserData *user_data = nullptr
    ) {

        return ImGui::InputText(to_label(label), buffer, BufferSize, (int)flags, callback, user_data);
    }

    /* Templatized userdata and label/ID, std::string as the text buffer
    */
    template <typename Label, typename UserData = void>
    inline bool InputText(Label label, std::string& text, ImGui::InputTextFlags flags = 0,
        int (*callback)(ImGuiInputTextCallbackData*) = nullptr,
        UserData* user_data = nullptr
    ) {
        std::vector<char> buffer( std::max(text.capacity(), (size_t)512) );
        strcpy_s(buffer.data(), buffer.size(), text.c_str());
        if (ImGui::InputText(to_label(label), buffer.data(), buffer.size(), (int)flags, callback, user_data)) {
            text = buffer.data();
            return true;
        }
        else
            return false;
    }

    // TODO: a variant that takes a struct containing options (can be partially initialized)

    // TODO: more overloads, and also for InputTextEx()

    // TODO: move into separate "utils" module!

    inline void BringItemIntoView() {

        ScrollToBringRectIntoView(GetCurrentWindow(), { GetItemRectMin(), GetItemRectMax() });
    }

    inline void ScrollHereUnlessVisible(float position = 0.5f) {

        if (!ImGui::IsItemVisible()) {
            ImGui::SetScrollHereY(position);
        }
    }

} // ns ImGui

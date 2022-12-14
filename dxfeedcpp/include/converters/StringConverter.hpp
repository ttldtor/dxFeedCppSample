#pragma once

#ifndef DXFEED_HPP_INCLUDED
#    error Please include only the DXFeed.hpp header
#endif

#include "common/DXFCppConfig.hpp"

#ifdef _MSC_FULL_VER
#    pragma warning(push)
#    pragma warning(disable : 4244)
#endif

#include <codecvt>
#include <sstream>

namespace dxfcpp {

///
struct StringConverter {
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wstringConvert;

    /**
     *
     * @param utf8
     * @return
     */
    static std::wstring utf8ToWString(const std::string &utf8) noexcept {
        try {
            return wstringConvert.from_bytes(utf8);
        } catch (...) {
            return {};
        }
    }

    /**
     *
     * @param utf8
     * @return
     */
    static std::wstring utf8ToWString(const char *utf8) noexcept {
        if (utf8 == nullptr) {
            return {};
        }

        try {
            return wstringConvert.from_bytes(utf8);
        } catch (...) {
            return {};
        }
    }

    /**
     *
     * @param c
     * @return
     */
    static wchar_t utf8ToWChar(char c) noexcept {
        if (c == '\0') {
            return L'\0';
        }

        return utf8ToWString(std::string(1, c))[0];
    }

    /**
     *
     * @param utf16
     * @return
     */
    static std::string wStringToUtf8(const std::wstring &utf16) noexcept {
        try {
            return wstringConvert.to_bytes(utf16);
        } catch (...) {
            return {};
        }
    }

    /**
     *
     * @param utf16
     * @return
     */
    static std::string wStringToUtf8(const wchar_t *utf16) noexcept {
        if (utf16 == nullptr) {
            return {};
        }

        try {
            return wstringConvert.to_bytes(utf16);
        } catch (...) {
            return {};
        }
    }

    /**
     *
     * @param c
     * @return
     */
    static char wCharToUtf8(wchar_t c) noexcept {
        if (c == L'\0') {
            return '\0';
        }

        return wStringToUtf8(std::wstring(1, c))[0];
    }
};

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> StringConverter::wstringConvert{};

inline std::string exchangeCodeToString(char c) {
    if (c >= 32 && c <= 126) {
        return std::string() + c;
    }

    if (c == 0) {
        return "\\0";
    }

    std::ostringstream oss{};

    oss << "\\x" << std::hex << std::to_string(c);

    return oss.str();
}

} // namespace dxfcpp

#ifdef _MSC_FULL_VER
#    pragma warning(pop)
#endif
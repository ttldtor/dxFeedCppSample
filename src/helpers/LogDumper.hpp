#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

namespace dxfcpp {

namespace LogDumper {
#ifndef NDEBUG
inline std::string getClassName(const std::string &prettyFunctionOutput, const std::string &thisParameterTypeName) {
    std::string clazz = prettyFunctionOutput;
#    if defined(__clang__)
    auto thisDef = thisParameterTypeName + " = ";
    std::string::size_type found = prettyFunctionOutput.find(thisDef);
    if (found != std::string::npos) {
        std::string::size_type found2 = prettyFunctionOutput.find(" *]", found + thisDef.size());

        if (found2 != std::string::npos) {
            clazz = prettyFunctionOutput.substr(found + thisDef.size(), found2 - (found + thisDef.size()));
        }
    }
#    elif defined(__GNUC__) || defined(__GNUG__)
    auto thisDef = thisParameterTypeName + " = ";
    std::string::size_type found = prettyFunctionOutput.find(thisDef);
    if (found != std::string::npos) {
        std::string::size_type found2 = prettyFunctionOutput.find("*]", found + thisDef.size());

        if (found2 != std::string::npos) {
            clazz = prettyFunctionOutput.substr(found + thisDef.size(), found2 - (found + thisDef.size()));
        }
    }
#    elif defined(_MSC_VER)
#    endif
    return clazz;
}

template <typename T, typename This> inline void dump(T &&v, This &&) {
    std::ostringstream oss{};

#    ifdef _MSC_VER
    std::string pf = __FUNCSIG__;
#    else
    std::string pf = __PRETTY_FUNCTION__;
#    endif

    std::string clazz = getClassName(pf, "This");

    oss << "[" << clazz << "][" << std::this_thread::get_id() << "] (" << v << "): "
        << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
               .count()
        << "\n";

    std::cerr << oss.str();
}

template <typename T> inline void dump(T &&v) {
    std::ostringstream oss{};

    oss << "[][" << std::this_thread::get_id() << "] (" << v << "): "
        << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
               .count()
        << "\n";

    std::cerr << oss.str();
}
#endif
} // namespace LogDumper

} // namespace dxfcpp
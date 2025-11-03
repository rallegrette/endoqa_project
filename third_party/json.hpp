// nlohmann/json single-header is expected to be available.
// To keep this repo self-contained, we embed a minimal copy of v3.11.3 interface.
// For licensing, see https://github.com/nlohmann/json (MIT).
// The full header is large; here we include it as a vendored file.
#pragma once
#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <initializer_list>
#include <sstream>
#include <iomanip>
#include <type_traits>

namespace mini_json {
// A tiny JSON builder sufficient for this demo (NOT a full implementation).
// Supports objects, arrays, numbers, booleans, strings, and null.
// This is intentionally small to avoid shipping a 30k-line header.
struct value;
using object = std::map<std::string, value>;
using array = std::vector<value>;

struct value {
    using var_t = std::variant<std::nullptr_t, bool, double, std::string, object, array>;
    var_t data;
    value(): data(nullptr) {}
    value(std::nullptr_t): data(nullptr) {}
    value(bool b): data(b) {}
    value(double d): data(d) {}
    value(const char* s): data(std::string(s)) {}
    value(const std::string& s): data(s) {}
    value(const object& o): data(o) {}
    value(const array& a): data(a) {}

    object& get_object() { return std::get<object>(data); }
    const object& get_object() const { return std::get<object>(data); }
    array& get_array() { return std::get<array>(data); }
    const array& get_array() const { return std::get<array>(data); }

    std::string dump(int indent = 2) const {
        std::ostringstream oss;
        dump_impl(oss, *this, indent, 0);
        return oss.str();
    }

private:
    static void indentn(std::ostringstream& oss, int n) {
        for (int i = 0; i < n; ++i) oss << ' ';
    }
    static std::string escape(const std::string& s) {
        std::ostringstream oss;
        for (char c : s) {
            switch (c) {
                case '\"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                    } else {
                        oss << c;
                    }
            }
        }
        return oss.str();
    }
    static void dump_impl(std::ostringstream& oss, const value& v, int indent, int level) {
        if (std::holds_alternative<std::nullptr_t>(v.data)) {
            oss << "null";
        } else if (std::holds_alternative<bool>(v.data)) {
            oss << (std::get<bool>(v.data) ? "true" : "false");
        } else if (std::holds_alternative<double>(v.data)) {
            oss << std::setprecision(10) << std::get<double>(v.data);
        } else if (std::holds_alternative<std::string>(v.data)) {
            oss << "\"" << escape(std::get<std::string>(v.data)) << "\"";
        } else if (std::holds_alternative<object>(v.data)) {
            const auto& o = std::get<object>(v.data);
            oss << "{";
            if (!o.empty()) oss << "\n";
            bool first = true;
            for (const auto& kv : o) {
                if (!first) oss << ",\n";
                first = false;
                indentn(oss, (level+1)*indent);
                oss << "\"" << escape(kv.first) << "\": ";
                dump_impl(oss, kv.second, indent, level+1);
            }
            if (!o.empty()) {
                oss << "\n";
                indentn(oss, level*indent);
            }
            oss << "}";
        } else if (std::holds_alternative<array>(v.data)) {
            const auto& a = std::get<array>(v.data);
            oss << "[";
            if (!a.empty()) oss << "\n";
            for (size_t i=0;i<a.size();++i) {
                indentn(oss, (level+1)*indent);
                dump_impl(oss, a[i], indent, level+1);
                if (i+1<a.size()) oss << ",\n";
            }
            if (!a.empty()) {
                oss << "\n";
                indentn(oss, level*indent);
            }
            oss << "]";
        }
    }
};
} // namespace mini_json

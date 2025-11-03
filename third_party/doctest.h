// doctest.h - single header (MIT) - minimal subset for this project
// For the full project see https://github.com/doctest/doctest
#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

namespace doctest {
struct Test {
    std::string name;
    std::function<void()> fn;
};
inline std::vector<Test>& registry() { static std::vector<Test> r; return r; }
struct Reg {
    Reg(const std::string& n, std::function<void()> f) { registry().push_back({n,f}); }
};
inline int run() {
    int failed = 0;
    for (auto& t : registry()) {
        try { t.fn(); std::cout << "[ OK ] " << t.name << "\n"; }
        catch (const std::exception& e) { std::cout << "[FAIL] " << t.name << " : " << e.what() << "\n"; failed++; }
        catch (...) { std::cout << "[FAIL] " << t.name << " : unknown\n"; failed++; }
    }
    std::cout << registry().size() - failed << "/" << registry().size() << " tests passed\n";
    return failed;
}
struct AssertException : public std::runtime_error { using std::runtime_error::runtime_error; };
inline void check(bool cond, const char* expr, const char* file, int line) {
    if (!cond) throw AssertException(std::string("assertion failed: ") + expr + " at " + file + ":" + std::to_string(line));
}
} // namespace doctest

#define DOCTEST_TEST_CASE(name) static void name(); static doctest::Reg reg_##name(#name, name); static void name()
#define DOCTEST_CHECK(expr) doctest::check((expr), #expr, __FILE__, __LINE__)
#define DOCTEST_REQUIRE(expr) DOCTEST_CHECK(expr)
#define DOCTEST_MAIN int main(){ return doctest::run(); }

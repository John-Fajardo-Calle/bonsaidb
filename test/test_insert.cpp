#include "DatabaseEngine.hpp"
#include <cassert>
#include <cstdio>
#ifdef _WIN32
#  include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    const char* fname = "test_insert.db";
    std::remove(fname);
    DatabaseEngine engine(fname);

    Record r{};
    r.id = 1;
    snprintf(r.name, sizeof(r.name), "Test");
    r.age = 30;
    r.balance = 100.5;

    bool inserted = engine.insert(r);
    assert(inserted);

    auto found = engine.find(1);
    assert(found.has_value());
    assert(found->id == r.id);

    return 0;
}
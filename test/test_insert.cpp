#include "DatabaseEngine.hpp"
#include <cassert>
#include <cstdio>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
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
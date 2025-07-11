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
    const char* fname = "test_delete.db";
    std::remove(fname);
    DatabaseEngine engine(fname);

    for (int i = 0; i < 60; ++i) {
        Record r{};
        r.id = i;
        snprintf(r.name, sizeof(r.name), "R%d", i);
        r.age = i;
        r.balance = i * 10.0;
        assert(engine.insert(r));
    }

    for (int i = 0; i < 30; ++i) {
        assert(engine.remove(i));
    }

    for (int i = 0; i < 60; ++i) {
        auto rec = engine.find(i);
        if (i < 30) {
            assert(!rec.has_value());
        } else {
            assert(rec.has_value());
            assert(rec->id == i);
        }
    }

    auto all = engine.dumpAll();
    for (int i = 30; i < 60; ++i) {
        bool found = false;
        for (const auto& r : all) {
            if (r.id == i) { found = true; break; }
        }
        assert(found);
    }
    return 0;
}
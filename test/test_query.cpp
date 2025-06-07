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
    const char* fname = "test_query.db";
    std::remove(fname);
    DatabaseEngine engine(fname);

    for (int i = 0; i < 10; ++i) {
        Record r{};
        r.id = i;
        snprintf(r.name, sizeof(r.name), "User%d", i);
        r.age = 20 + i;
        r.balance = 50.0 * i;
        assert(engine.insert(r));
    }

    for (int i = 0; i < 10; ++i) {
        auto rec = engine.find(i);
        assert(rec.has_value());
        assert(rec->id == i);
    }

    return 0;
}
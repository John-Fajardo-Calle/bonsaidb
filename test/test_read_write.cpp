#include "DatabaseEngine.hpp"
#include <cassert>
#include <thread>
#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    const char* fname = "test_read_write.db";
    std::remove(fname);
    DatabaseEngine engine(fname);

    auto writer = [&engine]() {
        for (int i = 0; i < 100; ++i) {
            Record r{};
            r.id = i;
            snprintf(r.name, sizeof(r.name), "W%d", i);
            r.age = i % 90;
            r.balance = i * 2.0;
            engine.insert(r);
        }
    };

    auto reader = [&engine]() {
        for (int i = 0; i < 100; ++i) {
            engine.find(i);
        }
    };

    std::thread tw(writer);
    std::thread tr(reader);

    tw.join();
    tr.join();

    for (int i = 0; i < 100; ++i) {
        auto rec = engine.find(i);
        assert(rec.has_value());
        assert(rec->id == i);
    }

    return 0;
}
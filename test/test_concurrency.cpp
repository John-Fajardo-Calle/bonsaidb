#include "DatabaseEngine.hpp"
#include <cassert>
#include <thread>
#include <vector>
#include <cstdio>
#ifdef _WIN32
#  include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    const char* fname = "test_concurrency.db";
    std::remove(fname);
    DatabaseEngine engine(fname);

    auto inserter = [&engine](int start){
        for(int i=start; i<start+50; ++i){
            Record r{};
            r.id = i;
            snprintf(r.name, sizeof(r.name), "T%d", i);
            r.age = i%100;
            r.balance = i*1.5;
            engine.insert(r);
        }
    };

    std::thread t1(inserter, 0);
    std::thread t2(inserter, 50);
    t1.join();
    t2.join();

    for(int i=0;i<100;++i){
        auto rec = engine.find(i);
        assert(rec.has_value());
        assert(rec->id == i);
    }

    return 0;
}
#include <fstream>
#include <random>
#include <string>
#include <cstdio>

int main(int argc, char* argv[]) {
    if(argc != 3){
        std::printf("Uso: %s <archivo.csv> <num_registros>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];
    int count = std::stoi(argv[2]);
    std::ofstream out(filename);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> age_dist(18,70);
    for(int i=0;i<count;++i){
        out << i << ",User" << i << ',' << age_dist(rng) << ',' << i*10.0 << '\n';
    }
    return 0;
}
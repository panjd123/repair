#include <assert.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include "repair_compress.h"
#include "utils.h"

using namespace repair_compress;

void generate_random_data(value_t* data, size_t n, size_t dim, value_t MAX = 10, size_t seed = 0) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, MAX);
    for (size_t i = 0; i < n * dim; i++) {
        data[i] = dis(gen);
    }
}

int main() {
#if 0
    size_t n = 100000;
    size_t dim = 128;
    std::unique_ptr<value_t[]> udata = std::make_unique<value_t[]>(n * dim);
    generate_random_data(udata.get(), n, dim, 256);
    value_t* data = udata.get();
#else
    auto [float_udata, n, dim] = loadFvecs("/home/panjunda/tribase/Tribase/benchmarks/sift1m/origin/sift1m_base.fvecs");
    static_assert(sizeof(value_t) == sizeof(float));
    value_t* data = reinterpret_cast<value_t*>(float_udata.get());
#endif
    std::cout << "n: " << n << ", dim: " << dim << std::endl;

    auto start = std::chrono::steady_clock::now();
    auto [rule, result_vlist, result_elist] = generate_rule(data, n, dim, 10, true);
    auto end = std::chrono::steady_clock::now();

    std::cout << std::format("time: {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) << std::endl;

    if (n * dim < 256) {
        for (auto [pair, value] : rule) {
            std::cout << pair.first << " " << pair.second << " -> " << value << std::endl;
        }

        for (size_t i = 0; i < result_vlist.size(); i++) {
            std::cout << result_vlist[i] << " ";
        }
        std::cout << std::endl;

        for (size_t i = 0; i < result_elist.size(); i++) {
            std::cout << result_elist[i] << " ";
        }
        std::cout << std::endl;
    }

    std::ofstream rule_file("rule.txt");
    for (auto [pair, value] : rule) {
        rule_file << pair.first << " " << pair.second << " " << value << std::endl;
    }

    std::ofstream result_vlist_file("result.vlist", std::ios::binary);
    result_vlist_file.write(reinterpret_cast<char*>(result_vlist.data()), result_vlist.size() * sizeof(value_t));

    std::ofstream result_elist_file("result.elist", std::ios::binary);
    result_elist_file.write(reinterpret_cast<char*>(result_elist.data()), result_elist.size() * sizeof(value_t));
    return 0;
}
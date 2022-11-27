//#include <omp.h>
#include <immintrin.h>

#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <map>
#include <bitset>

#include "huffman.hpp"
#include "trees.hpp"
#include "utils.hpp"

namespace huffman {
///#################################
/// TODO:
/// * size of base unit of coding
/// * change vector with bitset
///
///#################################

std::uint32_t get_statistic(const std::uint8_t* src, const std::size_t size, std::size_t* statistic)
{
    //int num_thr = omp_get_num_threads();
    //std::size_t local_stat[2][256];
    std::uint32_t stat_vec[8][256];
    std::memset(stat_vec, 0, 256 * 8 * sizeof(std::uint32_t));
    //std::memset(statistic, 0, 256 * sizeof(std::size_t));
    //std::memset(local_stat[1], 0, 256 * sizeof(std::size_t));
//#pragma omp parallel shared(src, size, local_stat)
//    {
//        std::size_t tn = size / omp_get_num_threads();
//        int thread_num = omp_get_thread_num();
//#pragma omp for schedule(static, tn)
//        for (std::size_t i = 0; i < size; ++i)
//            local_stat[thread_num][src[i]]++;
//    }

    //for (std::size_t i = 0; i < size; ++i) {
    //    ++statistic[src[i]];
    //}

    std::size_t vec_size    = (size >> 3) << 3;
    std::size_t offset_size = size - vec_size;
    __m256i ymm0, ymm1, ymm2, ymm3, ymm4;
    const __m256i mask = _mm256_set_epi32(7 * 256,
                                          6 * 256,
                                          5 * 256,
                                          4 * 256,
                                          3 * 256,
                                          2 * 256,
                                          1 * 256,
                                          0 * 256);
    __m128i xmm0, xmm1;

    xmm1 = _mm_set_epi64x(0ul, 1ul);
    ymm4 = _mm256_broadcastd_epi32(xmm1);

    for (std::size_t i = 0; i < vec_size; i+=8) {
        //std::uint64_t src_data = *(reinterpret_cast<const std::uint64_t*>(src));
        xmm0 = _mm_lddqu_si128((const __m128i*)src); // load data (16 bytes, but 8 is needed)
        //std::cout << "hello\n";
        ymm0 = _mm256_cvtepu8_epi32(xmm0);
        //std::cout << "hello1\n";
        ymm0 = _mm256_add_epi32(ymm0, mask);
        //std::cout << "hello2\n";
        ymm1 = _mm256_i32gather_epi32((int const*)stat_vec, ymm0, 4);
        //std::cout << "hello3\n";
        ymm2 = _mm256_add_epi32(ymm1, ymm4);
        //std::cout << "hello4\n";
        _mm256_i32scatter_epi32(stat_vec, ymm0, ymm2, 4);
        //std::cout << "hello5\n";
    }

    std::uint32_t number_of_elements = 0;

    for (std::size_t i = 0; i < 256; ++i) {
        statistic[i] = stat_vec[0][i] +
                       stat_vec[1][i] +
                       stat_vec[2][i] +
                       stat_vec[3][i] +
                       stat_vec[4][i] +
                       stat_vec[5][i] +
                       stat_vec[6][i] +
                       stat_vec[7][i];
        if (statistic[i] != 0)
            ++number_of_elements;
    }
    return number_of_elements;
}

std::size_t huffman_encode(const std::uint8_t* src, const std::size_t src_size, std::uint8_t* dst)
{
    //omp_set_num_threads(2);

    using namespace detail;
    // initialize tree leaf
    std::size_t st[256];
    std::uint32_t num_elem = get_statistic(src, src_size, st) * 2 - 1;
    void* node_array_pointer = allocate_memory(sizeof(node<std::uint8_t>) * num_elem);
    node<std::uint8_t>* node_ptr = reinterpret_cast<node<std::uint8_t>*>(node_array_pointer);

    //auto grater = [](node* a, node* b) mutable -> bool { return *a > *b; };
    std::priority_queue<node<std::uint8_t>*, std::vector<node<std::uint8_t>*>, greater> q;
    std::size_t dst_index = 0;
    // barrier of statistic in output flow
    std::uint8_t barrier = 0;
    bool is_first = true;

    for (int i = 0; i < 256; ++i)
        if (st[i] > 0) {
            if (is_first) {
                barrier = static_cast<std::uint8_t>(i);
                is_first = false;
            }
            *(dst + dst_index) = static_cast<std::uint8_t>(i);
            *(reinterpret_cast<std::size_t*>(dst + dst_index + 1)) = st[i];
            dst_index += sizeof(std::uint8_t) + sizeof(std::size_t);
            q.push(new(node_ptr++) node<std::uint8_t>(st[i], static_cast<std::uint8_t>(i)));
            //std::cout << "literal - " << (char)i << ", count - " << st[i] <<std::endl;
        }
    dst[dst_index++] = barrier;

    // build tree
    while (q.size() != 1) {
        node<std::uint8_t>* first = const_cast<node<std::uint8_t>*>(q.top());
        q.pop();
        node<std::uint8_t>* second = const_cast<node<std::uint8_t>*>(q.top());
        q.pop();
        q.push(new(node_ptr++) node<std::uint8_t>(first, second));
    }

    // tree root
    node<std::uint8_t>* root = const_cast<node<std::uint8_t>*>(q.top());
    q.pop();

    std::map<std::uint8_t, std::vector<bool>> codes;
    std::vector<bool> cache{};
    get_codes(root, codes, cache);

    std::size_t bit_ptr = 0;
    std::size_t byte_ptr = dst_index;
    for (std::size_t i = 0; i < src_size; ++i) {
        for (int j = 0; j < codes[src[i]].size(); ++j) {
            if (bit_ptr >= 8) {
                bit_ptr = 0;
                byte_ptr++;
            }
            dst[byte_ptr] += (codes[src[i]][j] & 1u) << bit_ptr++;
        }
    }

    //node<std::uint8_t>::clear_tree(root);
    deallocate_memory(node_array_pointer);
    return byte_ptr + 1;
}

}

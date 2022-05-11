#include "huffman.hpp"
#include "details/example_class.hpp"

int main(int argc, char** argv)
{
    example_class example("Dynamic Huffman", huffman::dynamic_huffman_encode, huffman::dynamic_huffman_decode);
    if (argc != 4) {
        example.print_help();
        return 0;
    }
    std::string mode        = argv[1];
    std::string input_file  = argv[2];
    std::string output_file = argv[3];

    example.execute(mode, input_file, output_file);
    return 0;
}

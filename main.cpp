#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <regex>
#include <string>
#include "tree.h"


void print_map(const std::map<char, int>& map)
{
    // print a std::map<char, int> to stdout
    for (auto elem : map) {
        std::cout << elem.first << " : " << elem.second << std::endl;
    }
}

void print_encode_table(const std::unordered_map<char, std::vector<bool>>& table)
{
    // print the encode table to stdout
    for (auto elem : table) {
        std::cout << elem.first << ": ";
        for (auto ch : elem.second) {
            std::cout << ch;
        }
        std::cout << std::endl;
    }
    std::cout << "END" << std::endl;
}

void print_decode_table(const std::unordered_map<std::vector<bool>, char>& table)
{
    // print the decode table to stdout
    for (auto elem : table) {
        std::cout << elem.second << ": ";
        for (auto ch : elem.first) {
            std::cout << ch;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


std::unordered_map<std::vector<bool>, char> getDecodeTable(std::istream& input)
{
    // Parse the decode table from istream input.
    std::unordered_map<std::vector<bool>, char> decode_table;
    std::regex regexp ("^.: (1|0)+$");
    for (std::string line; line != "END" && std::getline(input, line);) {
        std::vector<bool> bitmap;
        std::smatch m;
        if (std::regex_match(line, m, regexp)) {
            char c = line[0];
            for (auto bit = line.begin() + 3; bit != line.end(); ++bit) {
                if (*bit == '0') bitmap.push_back(false);
                else bitmap.push_back(true);
            }
            decode_table[bitmap] = c;
        }
    }
    return decode_table;
}

int main(int argc, char *argv[])
{
    if (argc < 3) return 1;
    std::ifstream input {argv[2]};
    auto freq = Huffman::count_frequency(input);
    auto huffTree = Huffman::Tree(freq);
    if (std::strcmp(argv[1], "encode") == 0) {
        // do encoding
        auto encode_table = huffTree.getEncodeTable();
        print_encode_table(encode_table);
        input.clear();
        input.seekg(0, std::ios::beg);
        auto enc = Huffman::encode(input, encode_table);
        for (auto ch: enc) {
            std::cout << ch;
        }
        std::cout << std::endl;
    } else if (std::strcmp(argv[1], "decode") == 0) {
        // do decode
        input.clear();
        input.seekg(0, std::ios::beg);
        auto decode_table = getDecodeTable(input);
        print_decode_table(decode_table);
        auto dec = Huffman::decode(input, decode_table);
        for (auto ch: dec) {
            std::cout << ch;
        }
        std::cout << std::endl;
    } else {
        // error: unspecified operation
        std::cerr << "bad usage";
        return 1;
    }

}

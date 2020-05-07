#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>

namespace Huffman {

class Tree {
    /** Huffman::Tree is an implementation of the Huffman encoding tree.
    */
public:
    /**
    * @brief Tree
    * get as input a map for the leaves of the huffman tree,
    * alias all the pair (symbol, frequency) used to build the tree.
    */
    Tree(std::map<char, int> leafes);
    /**
     * @brief print print the tree using a DFS view.
     */
    void print();
    /**
     * @brief getEncodeTable
     * @return the encoding table of the tree a map (symbol, code).
     */
    std::unordered_map<char, std::vector<bool>> getEncodeTable();
    /**
     * @brief getDecodeTable
     * @return the decode table, map (code, symbol)
     */
    std::unordered_map<std::vector<bool>, char> getDecodeTable();
    /**
     * @brief printLeafes print all leafes of the tree.
     */
    void printLeafes();
private:
        struct Node {
            char sym;
            int freq;
            bool code;
            Node* left;
            Node* right;
            Node* parent;
            Node(char sym, int freq, Node* left, Node* right);
        };
        struct CompareNode {
            bool operator()(const Tree::Node* a, const Tree::Node* b) {
                if (a && b)
                    if (a->freq == b->freq)
                        return a->sym > b->sym;
                    else
                        return a->freq > b->freq;
                else
                    return false;
            }
        };

        Tree::Node* root;
        size_t no_leafes;
        std::vector<std::unique_ptr<Node>> nodes;

        int newNode(char sym, int freq);
        Tree::Node* getNode(char sym);
        Tree::Node* getLastNode();
        void utility_print(Tree::Node *node);
        void setCode();
        void setCodeNode(Node* node);

};

std::map<char, int> count_frequency(std::istream& input);
std::vector<char> encode(std::istream& input, std::unordered_map<char, std::vector<bool>>& encoding_table);
std::vector<char> decode(std::istream& input, std::unordered_map<std::vector<bool>, char>& encoding_table);

}

#endif // TREE_H

#include "tree.h"
namespace Huffman {

std::vector<char> bitmapToChar(const std::vector<bool>& bitmap)
{
    std::vector<char> res;
    for (auto bit: bitmap) {
        if (bit) res.push_back('1');
        else res.push_back('0');
    }
    return res;
}

std::vector<bool> charToBitmap(const std::vector<char>& charvec)
{
    std::vector<bool> res;
    for (auto ch: charvec) {
        if (ch == '0') res.push_back(false);
        else if (ch == '1') res.push_back(true);
        else throw std::logic_error("charvec should be only '0' or '1'");
    }
    return res;
}

std::vector<char> encode(std::istream& input, std::unordered_map<char, std::vector<bool>>& encoding_table)
{
    std::vector<char> res;
    for (char c; input >> c;) {
        auto tmp = bitmapToChar(encoding_table[c]);
        res.insert(res.end(), tmp.begin(), tmp.end());
    }
    return res;
}


std::vector<char> decode(std::istream& input, std::unordered_map<std::vector<bool>, char>& decode_table)
{
    std::vector<char> res;
    std::vector<char> tmp;
    for (char c; input >> c;) {
        tmp.push_back(c);
        auto bitmap = charToBitmap(tmp);
        auto got = decode_table.find(bitmap);
        if (got != decode_table.end()) {
            res.push_back(got->second);
            tmp.clear();
        }
    }
    return res;
}


Tree::Node::Node(char sym, int freq, Node *left, Node *right):
    sym(sym), freq(freq), left(left), right(right) {}

int Tree::newNode(char sym, int freq) {
    std::unique_ptr<Node> node = std::unique_ptr<Node>(new Node(sym, freq, nullptr, nullptr));
    nodes.push_back(std::move(node));
    return nodes.size() - 1;
}

Tree::Node* Tree::getNode(char sym)
{
    for (auto node = nodes.begin(); node != nodes.end(); ++node) {
        if (node->get()->sym == sym) return node->get();
    }
    return nullptr;
}
Tree::Node* Tree::getLastNode()
{
    return nodes.at(nodes.size() - 1).get();
}



Tree::Tree(std::map<char, int> leafes)
{
    no_leafes = leafes.size();
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
    for (auto elem : leafes) {
        newNode(elem.first, elem.second);
        pq.push(getLastNode());
    }
    while (pq.size() > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();
        newNode('\0', left->freq + right->freq);
        Node* newNode = getLastNode();
        newNode->left = left;
        newNode->right = right;
        left->parent = newNode;
        right->parent = newNode;
        pq.push(newNode);
    }
    root = pq.top();
    pq.pop();
    root->parent = nullptr;
    root->code = false;
    setCode();
}

std::map<char, int> count_frequency(std::istream& input) {
    std::map<char, int> frequency;
    for (char c; input >> c;) {
        frequency[c]++;
    }
    return frequency;
}

void Tree::utility_print(Tree::Node *node)
{
    if (node) {
        std::cout << node->sym << " : " << node->freq << " : " << node->code <<std::endl;
        utility_print(node->left);
        utility_print(node->right);
    }
}


void Tree::print()
{
    utility_print(root);
}

std::unordered_map<char, std::vector<bool>> Tree::getEncodeTable()
{
    std::unordered_map<char, std::vector<bool>> encode_table;
    for (auto node = nodes.begin(); node != nodes.end(); ++node) {
        if (node->get()->sym != '\0') {
            std::vector<bool> path;
            Node* curr = node->get();
            while (curr != root) {
                path.push_back(curr->code);
                curr = curr->parent;
            }
            std::reverse(path.begin(), path.end());
            encode_table[node->get()->sym] = path;
        }
    }
    return encode_table;
}

std::unordered_map<std::vector<bool>, char> Tree::getDecodeTable()
{
    std::unordered_map<std::vector<bool>, char> decode_table;
    auto encodeTable = getEncodeTable();
    for (auto elem: encodeTable) {
        decode_table[elem.second] = elem.first;
    }
    return decode_table;
}

void Tree::printLeafes() {
    for (auto node = nodes.begin(); node != nodes.end(); ++node) {
        if (node->get()->sym != '\0')
            std::cout << node->get()->sym << " : " << node->get()->freq << std::endl;
    }
}

void Tree::setCode()
{
    setCodeNode(root);
}

void Tree::setCodeNode(Node* node)
{
    if (node == nullptr) return;
    if (node->left) node->left->code = false;
    if (node->right) node->right->code = true;
    setCodeNode(node->left);
    setCodeNode(node->right);
}

}


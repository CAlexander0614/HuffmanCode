#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "HeapQueue.hpp"
#include "HuffmanBase.hpp"
#include <string>
#include <map>


class HuffmanTree : public HuffmanTreeBase {
public:
   

    std::string compress(const std::string inputStr);
    std::string serializeTree() const;
    std::string decompress(const std::string inputCode, const std::string serializedTree);
    
    
    void generateCodes(std::map<char, std::string>* map, HuffmanNode* node, std::string prefix);
    void printTree(HuffmanNode* node) const;
    void createTree(std::string inputStr);
    std::vector<HuffmanNode*> createPostOrderList() const;
    std::vector<HuffmanNode *> createNodeList(std::string) const;
    HuffmanNode* postFixToInFix(std::vector<HuffmanNode*>) const;
    void deleteTree(HuffmanNode* node) const;


private:

    HuffmanNode* treeNode;
    
    
};

#endif

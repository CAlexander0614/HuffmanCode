/*
 * Huffman Code Compressor/Decompressor
 * Written By: Chris Alexander
 * Purpose: This program compresses and decompresses strings using a Huffman Tree
 */

#include "HuffmanTree.hpp"
#include "HeapQueue.hpp"
#include <string>
#include <vector>
#include <stack>
#include <exception>
#include <algorithm>


//Creates a HuffmanTree
void HuffmanTree::createTree(std::string inputStr) {

    HeapQueue<HuffmanNode*, HuffmanNode::Compare>* hq = new HeapQueue<HuffmanNode*, HuffmanNode::Compare>;
    
    std::map<char,size_t> m;

    char c;

    for(int i = 0; i < int(inputStr.size()); i++){

        size_t freq = 0;

        c = inputStr[i];

        for(int j = 0; j < int(inputStr.size()); j++) {


            if(inputStr[j] == c){

                freq++;


            }


        }
        

        m.insert({c, freq});
    }
    
  
    

    //sets iterator to the first map element then iterates through map inserting pair into heap
    std::map<char, size_t>::iterator p;        
    p = m.begin();
            
    while(p!=m.end()){
                
        HuffmanNode* node = new HuffmanNode(p->first, p->second);
               
               
        hq->insert(node);
              

        p++;
                
    }
    
   
    //creates linked binary tree by popping two minimums and creating a parent node between them and returning to heap (this will repreat until there is one element in the heap which will be one tree
    while(hq->size() != 1){
        
        HuffmanNode *x = hq->min();
        hq->removeMin();
        HuffmanNode *y = hq->min();
        hq->removeMin();
        
        
        HuffmanNode* parentNode = new HuffmanNode('\0', x->getFrequency() + y->getFrequency());
        parentNode->left = x;
        parentNode->right = y;
        x->parent = parentNode;
        y->parent = parentNode;
        
        hq->insert(parentNode);
        
    }
    
    treeNode = hq->min();

}

std::string HuffmanTree::compress(const std::string inputStr){
    
    
    createTree(inputStr);
    

    //empty string passed to generateCodes function
    std::string code = "";
    
    
    //creates new map
    std::map<char, std::string> *codeMap = new std::map<char, std::string>;
    
    //fills map keys with chars from the string and map values with encoded values
    generateCodes(codeMap, treeNode, code);
    

    std::map<char, std::string>::iterator it;
    
    
    std::string returnString = "";

    //replaces each character in the string with it's encoded value
    for(int i = 0; i < int(inputStr.size()); i++){
        
        it = codeMap->begin();
        
        while(it != codeMap->end()){
        
            if(it->first != inputStr[i]){
                
                it++;
            
            } else {
                
                returnString += it->second;
                break;
            }
            
            
        }
        
       
    }

    
    return returnString;
    
}



std::string HuffmanTree::serializeTree() const {
 
    std::vector<HuffmanNode*> nodeList = HuffmanTree::createPostOrderList();
    
    std::string theString = "";
    
    for(int i = 0; i <  int(nodeList.size()); i++) {
        
        
        if(nodeList[i]->isBranch()){
            
            theString += 'B';
            
        } else {
            
            
            theString += 'L';
            theString += nodeList[i]->getCharacter();
            
        }
        
        
    }
    
    HuffmanTree::deleteTree(treeNode); //deletes the tree
    return theString;
}



std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
    
    std::vector<HuffmanNode*> postVector = HuffmanTree::createNodeList(serializedTree); //creates vector of nodes in post order
    
    HuffmanNode* node = HuffmanTree::postFixToInFix(postVector); //returns a tree called node
    
  
    HuffmanNode* root = node;
 
    
    std::string outputString = "";
    
    //traverses the tree following the input code; after a leaf is reached it's char is added to the output string and traversal returns to tree root
    for(int i = 0; i < int(inputCode.size()); i++){
        

        if(inputCode[i] == '0'){
            
            root = root->left;
          
        }
        if(inputCode[i] == '1'){
            
            root = root->right;
            
            
        }
        
        if(root->isLeaf()){
            
            outputString += root->getCharacter();
            root = node;
            
        }
        
    }
    
    return outputString;

}





//creates map; keys are chars in the provided string, values are strings calculated by preorder tree-travesal
void HuffmanTree::generateCodes(std::map<char, std::string>* amap, HuffmanNode* node, std::string prefix) {

    if(node == nullptr){
        
        return;
    }
    
    if(node->isLeaf()) {
     
        amap->insert({node->getCharacter(), prefix});
        
        
    }
    
    generateCodes(amap, node->left, prefix + '0');
    generateCodes(amap, node->right, prefix + '1');
    
}


//prints tree (in-order)
void HuffmanTree::printTree(HuffmanNode* node) const{
 
    if(node == nullptr){
     
        return;
    }
    if(node->isBranch())
    printTree(node->left);
    
    std::cout << node->getCharacter()  << " ";
    
    if(node->isBranch())
        printTree(node->right);
    
 
}



//This function creates a vector of HuffmanNodes in post order
std::vector<HuffmanNode*> HuffmanTree::createPostOrderList() const {
    
        HuffmanNode* node = treeNode;
    
        std::vector<HuffmanNode*> list;
        std::stack<HuffmanNode*> nodeStack;
    
    //Traverses tree and adds to vector
        while(node){
            
            list.push_back(node);
            
            if(node->left){
                
             nodeStack.push(node->left);
                
            }
            
            
            if(node->right){
                
                node = node->right;
                
            }
            
            else if(!nodeStack.empty()){
                
                node = nodeStack.top();
                
                nodeStack.pop();
            }
            else node = NULL;
        }
   
    
    std::reverse(list.begin(),list.end());
    

    
        return list;
    
};


//creates a vector of nodes from the input string
std::vector<HuffmanNode*> HuffmanTree::createNodeList(std::string serialString) const {
    
    int flag = 0;  //flags whether an L has been read (leaf node)
    
    std::vector<HuffmanNode*> nodeVector;
  
    for(int i = 0; i < int(serialString.size()); i ++) {

        
        if( serialString[i] == 'B' && flag == 0){ //if branch, create node with null character
            
            HuffmanNode* node = new HuffmanNode('\0',0);
            nodeVector.push_back(node);
            

        } else if( flag == 0 && serialString[i] == 'L') { //if leaf (L) set flag to 1
            flag = 1;
            continue;
            
            
        
        
        } else { //add the node's character to the string and reset flag
            
            
            HuffmanNode* node = new HuffmanNode(serialString[i], 0);
            nodeVector.push_back(node);
            flag = 0;
            
            
        }
            
        
    }

   
    for(int i = 0; i < int(nodeVector.size()); i++){
        
        std::cout << nodeVector[i]->getCharacter() << " ";
        
    }
        
        return nodeVector;
    
}



//turns postfix vector into tree using postfix to infix algorithm
HuffmanNode* HuffmanTree::postFixToInFix(std::vector<HuffmanNode*> nodeVector) const {
    
    std::stack<HuffmanNode*>  nodeStack;
    
   
    for(int i = 0; i < int(nodeVector.size()); i++){
        

        HuffmanNode* node = nodeVector[i];
        
        if(node->getCharacter() == '\0' ){ //if node is a branch pop two nodes from stack and add the new parent to stack
            
            HuffmanNode* a = nodeStack.top();
            nodeStack.pop();
    
            HuffmanNode* b = nodeStack.top();
            nodeStack.pop();
            
            node->right = a;
            a->parent = node;
            node->left = b;
            b->parent = node;
            
            nodeStack.push(node);
            

        } else {
            
 
            nodeStack.push(node); //if leaf, add to stack

        }
        
    }
    
    HuffmanNode* returnNode = nodeStack.top();

    return returnNode;
    
    
}


//deletes tree recursively
void HuffmanTree::deleteTree(HuffmanNode* node) const{
    
    if(node->left != nullptr){
        
        deleteTree(node->left);
    }
    
    if(node->right != nullptr) {
        
        deleteTree(node->right);
    }
    
    delete node;

}


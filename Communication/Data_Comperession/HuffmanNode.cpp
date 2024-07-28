#include "HuffmanNode.h"

Node::Node(unsigned char val, int freq) : value(val), frequency(freq), left(nullptr), right(nullptr) {}

Node::~Node() {
    if (left) delete left;
    if (right) delete right;
}

bool CompareNode::operator()(Node* l, Node* r) {
    return l->frequency > r->frequency;
}

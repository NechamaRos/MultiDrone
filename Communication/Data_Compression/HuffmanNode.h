#pragma once
#include <cstddef>// for size_t

struct Node {
    unsigned char value;
    size_t frequency;
    Node* left;
    Node* right;
    /// <summary>
    /// Constructor for the Node class.
    /// </summary>
    /// <param name="val">The value of the node (character).</param>
    /// <param name="freq">The frequency of the character.</param>
    Node(unsigned char val, size_t freq);
    /// <summary>
    /// Destructor for the Node class. Deletes left and right child nodes to free memory.
    /// </summary>
    ~Node();
};

struct CompareNode {
    /// <summary>
    /// Comparison operator for the Node class used in priority queue.
    /// </summary>
    /// <param name="l">Left node pointer.</param>
    /// <param name="r">Right node pointer.</param>
    /// <returns>True if the frequency of the left node is greater than the right node.</returns>
    bool operator()(Node* l, Node* r);
};

#include <cassert>
#include <deque>
#include <iostream>

using namespace std;


struct Node {
    Node(int v, Node *p)
            : value(v), parent(p) {}

    int value;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent;
};


class NodeBuilder {
public:
    Node *CreateRoot(int value) {
        nodes.emplace_back(value, nullptr);
        return &nodes.back();
    }

    Node *CreateLeftSon(Node *me, int value) {
        assert(me->left == nullptr);
        nodes.emplace_back(value, me);
        me->left = &nodes.back();
        return me->left;
    }

    Node *CreateRightSon(Node *me, int value) {
        assert(me->right == nullptr);
        nodes.emplace_back(value, me);
        me->right = &nodes.back();
        return me->right;
    }

private:
    deque<Node> nodes;
};


Node *DeepLeft(Node *me) {
    if (me->left)
        return DeepLeft(me->left);
    else
        return me;
}

Node *SiftUp(Node *me) {
    Node *par = me->parent;
    if (par == nullptr)
        return nullptr;
    if (par->left == me)
        return par;
    else
        return SiftUp(par);
}

Node *Next(Node *me) {
    if (me->right)
        return DeepLeft(me->right);
    if (me->parent && me->parent->left == me)
        return me->parent;
    if (me->parent && me->parent->right == me)
        return SiftUp(me);
    return nullptr;
}

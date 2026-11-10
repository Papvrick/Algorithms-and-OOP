#include <limits>
#include "binarysearchtree.h"

// Конструктор копирования
BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) : BinaryTree(other) {}

// Конструктор перемещения
BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept : BinaryTree(std::move(other)) {}

// Оператор присваивания
BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other)
{
    BinaryTree::operator=(other);
    return *this;
}

// Оператор перемещения
BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept
{
    BinaryTree::operator=(std::move(other));
    return *this;
}

// Добавление узла по правилу: левый < корень <= правый
BinaryTree::TreeNode* BinarySearchTree::addNode(const int key)
{
    if (!root_)
    {
        root_ = new TreeNode(key);
        return root_;
    }

    TreeNode* currentNode = root_;
    while (true)
    {
        if (key < currentNode->getKey())
        {
            if (!currentNode->getLeftChild())
            {
                currentNode->setLeftChild(new TreeNode(key));
                return currentNode->getLeftChild();
            }
            currentNode = currentNode->getLeftChild();
        }
        else
        {
            if (!currentNode->getRightChild())
            {
                currentNode->setRightChild(new TreeNode(key));
                return currentNode->getRightChild();
            }
            currentNode = currentNode->getRightChild();
        }
    }
}

// Поиск узла в дереве поиска
BinaryTree::TreeNode* BinarySearchTree::findNode(const int key) const
{
    TreeNode* current = root_;
    while (current)
    {
        if (key == current->getKey())
        {
            return current;
        }
        if (key < current->getKey())
        {
            current = current->getLeftChild();
        }
        else
        {
            current = current->getRightChild();
        }
    }
    return nullptr;
}

// Получение уровня вершины по ключу в BST
int BinarySearchTree::getNodeLevel(const int key) const
{
    TreeNode* current = root_;
    int level = 0;
    while (current)
    {
        if (key == current->getKey())
        {
            return level;
        }
        if (key < current->getKey())
        {
            current = current->getLeftChild();
        }
        else
        {
            current = current->getRightChild();
        }
        level++;
    }
    return -1;
}

// Поиск минимального ключа в BST
int BinarySearchTree::getMiminalKey() const
{
    if (!root_) return std::numeric_limits<int>::max();
    TreeNode* current = root_;
    while (current->getLeftChild())
    {
        current = current->getLeftChild();
    }
    return current->getKey();
}

// Поиск максимального ключа в BST
int BinarySearchTree::getMaxKey() const
{
    if (!root_) return std::numeric_limits<int>::min();
    TreeNode* current = root_;
    while (current->getRightChild())
    {
        current = current->getRightChild();
    }
    return current->getKey();
}

// Симметричный обход LNR
void BinarySearchTree::inOrderKeysInternal(std::vector<int>& keys, TreeNode* node) const
{
    if (!node) return;
    inOrderKeysInternal(keys, node->getLeftChild());
    keys.push_back(node->getKey());
    inOrderKeysInternal(keys, node->getRightChild());
}

// Получение всех ключей по возрастанию через LNR
std::vector<int> BinarySearchTree::getKeysAscending() const
{
    std::vector<int> keys;
    inOrderKeysInternal(keys, root_);
    return keys;
}

// Рекурсивное удаление узла по правилам BST
bool BinarySearchTree::removeNodeBST(TreeNode*& node, const int key)
{
    if (!node) return false;

    if (key < node->getKey())
    {
        TreeNode* leftChild = node->getLeftChild();
        bool res = removeNodeBST(leftChild, key);
        node->setLeftChild(leftChild);
        return res;
    }
    if (key > node->getKey())
    {
        TreeNode* rightChild = node->getRightChild();
        bool res = removeNodeBST(rightChild, key);
        node->setRightChild(rightChild);
        return res;
    }

    TreeNode* nodeToDelete = node;
    if (node->isLeafNode())
    {
        node = nullptr;
    }
    else if (!node->getLeftChild())
    {
        node = node->getRightChild();
    }
    else if (!node->getRightChild())
    {
        node = node->getLeftChild();
    }
    else
    {
        TreeNode* minRight = node->getRightChild();
        while (minRight->getLeftChild())
        {
            minRight = minRight->getLeftChild();
        }
        int minKey = minRight->getKey();
        TreeNode* rightChild = node->getRightChild();
        removeNodeBST(rightChild, minKey);
        node->setRightChild(rightChild);
        node->setKey(minKey);
        return true;
    }

    delete nodeToDelete;
    return true;
}

// Удаление узла по ключу
bool BinarySearchTree::removeNode(const int key)
{
    return removeNodeBST(root_, key);
}
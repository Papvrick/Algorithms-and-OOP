#include <stdlib.h>
#include <limits>
#include <iostream>
#include <list>
#include <algorithm>
#include "binarytree.h"

// Конструктор копирования
BinaryTree::BinaryTree(const BinaryTree& other) : root_(nullptr)
{
    if (other.root_)
    {
        root_ = copyNode(other.root_);
    }
}

// Конструктор перемещения
BinaryTree::BinaryTree(BinaryTree&& other) noexcept : root_(other.root_)
{
    other.root_ = nullptr;
}

// Деструктор
BinaryTree::~BinaryTree()
{
    clear();
}

// Оператор присваивания
BinaryTree& BinaryTree::operator=(const BinaryTree& other)
{
    if (this != &other)
    {
        clear();
        if (other.root_)
        {
            root_ = copyNode(other.root_);
        }
    }
    return *this;
}

// Оператор перемещения
BinaryTree& BinaryTree::operator=(BinaryTree&& other) noexcept
{
    if (this != &other)
    {
        clear();
        root_ = other.root_;
        other.root_ = nullptr;
    }
    return *this;
}

// Рекурсивное копирование
BinaryTree::TreeNode* BinaryTree::copyNode(const TreeNode* node)
{
    if (!node) return nullptr;
    return new TreeNode(node->getKey(),
        copyNode(node->getLeftChild()),
        copyNode(node->getRightChild()));
}

// Получение корня дерева
BinaryTree::TreeNode* BinaryTree::getRoot() const { return root_; }

// Проверка, пусто ли дерево
bool BinaryTree::isEmpty() const
{
    return !root_;
}

// Очистка дерева
void BinaryTree::clear()
{
    if (root_)
    {
        destroyNode(root_);
        root_ = nullptr;
    }
}

// Рекурсивное удаление узла и всех его потомков
void BinaryTree::destroyNode(BinaryTree::TreeNode* treeNode)
{
    if (!treeNode) return;
    if (treeNode->getLeftChild()) destroyNode(treeNode->getLeftChild());
    if (treeNode->getRightChild()) destroyNode(treeNode->getRightChild());
    delete treeNode;
}

// Удаление поддеревьев узла
void BinaryTree::deleteSubtrees(TreeNode* node)
{
    if (!node) return;
    if (node->getLeftChild())
    {
        destroyNode(node->getLeftChild());
        node->setLeftChild(nullptr);
    }
    if (node->getRightChild())
    {
        destroyNode(node->getRightChild());
        node->setRightChild(nullptr);
    }
}

// Добавление узла в дерево (методом случайного выбора поддерева)
BinaryTree::TreeNode* BinaryTree::addNode(const int key)
{
    if (!root_)
    {
        root_ = new TreeNode(key);
        return root_;
    }

    TreeNode* currentNode = root_;
    while (true)
    {
        int randomValue = rand() % 2;
        if (randomValue == 0)
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

// Рекурсивный поиск узла по ключу
BinaryTree::TreeNode* BinaryTree::findNodeInternal(TreeNode* node, int key) const
{
    if (!node || node->getKey() == key) return node;
    TreeNode* leftResult = findNodeInternal(node->getLeftChild(), key);
    if (leftResult) return leftResult;
    return findNodeInternal(node->getRightChild(), key);
}

// Поиск узла дерева по ключу
BinaryTree::TreeNode* BinaryTree::findNode(const int key) const
{
    return findNodeInternal(root_, key);
}

// Рекурсивное удаление узла по ключу
bool BinaryTree::removeNodeInternal(TreeNode*& node, const int key)
{
    if (!node) return false;

    if (node->getKey() == key)
    {
        TreeNode* nodeToDelete = node;

        if (node->isLeafNode())
        {
            node = nullptr;
        }
        else if (!node->getRightChild())
        {
            node = node->getLeftChild();
        }
        else if (!node->getLeftChild())
        {
            node = node->getRightChild();
        }
        else
        {
            TreeNode* replacement = node->getRightChild();
            TreeNode* leftmost = replacement;
            while (leftmost->getLeftChild())
            {
                leftmost = leftmost->getLeftChild();
            }
            leftmost->setLeftChild(node->getLeftChild());
            node = replacement;
        }

        delete nodeToDelete;
        return true;
    }

    TreeNode* leftChild = node->getLeftChild();
    if (removeNodeInternal(leftChild, key))
    {
        node->setLeftChild(leftChild);
        return true;
    }

    TreeNode* rightChild = node->getRightChild();
    if (removeNodeInternal(rightChild, key))
    {
        node->setRightChild(rightChild);
        return true;
    }

    return false;
}

// Удаление узла из дерева по ключу
bool BinaryTree::removeNode(const int key)
{
    return removeNodeInternal(root_, key);
}

// Рекурсивное получение высоты дерева
int BinaryTree::getHeightInternal(TreeNode* treeNode) const
{
    if (!treeNode) return 0;
    return 1 + std::max(getHeightInternal(treeNode->getLeftChild()),
        getHeightInternal(treeNode->getRightChild()));
}

// Получение высоты дерева
int BinaryTree::getHeight() const
{
    return getHeightInternal(root_);
}

// Рекурсивный подсчёт количества узлов
size_t BinaryTree::getNodeCountInternal(TreeNode* treeNode) const
{
    if (!treeNode) return 0;
    return 1 + getNodeCountInternal(treeNode->getLeftChild()) + getNodeCountInternal(treeNode->getRightChild());
}

// Получение количества узлов дерева
size_t BinaryTree::getNodeCount() const
{
    return getNodeCountInternal(root_);
}

// Рекурсивный поиск минимального ключа
int BinaryTree::getMiminalKeyInternal(TreeNode* treeNode) const
{
    if (!treeNode) return std::numeric_limits<int>::max();
    int value = treeNode->getKey();
    if (treeNode->getLeftChild()) value = std::min(value, getMiminalKeyInternal(treeNode->getLeftChild()));
    if (treeNode->getRightChild()) value = std::min(value, getMiminalKeyInternal(treeNode->getRightChild()));
    return value;
}

// Получение минимального ключа дерева
int BinaryTree::getMiminalKey() const
{
    if (!root_) return std::numeric_limits<int>::max();
    return getMiminalKeyInternal(root_);
}

// Рекурсивный поиск максимального ключа
int BinaryTree::getMaxKeyInternal(TreeNode* treeNode) const
{
    if (!treeNode) return std::numeric_limits<int>::min();
    int value = treeNode->getKey();
    if (treeNode->getLeftChild()) value = std::max(value, getMaxKeyInternal(treeNode->getLeftChild()));
    if (treeNode->getRightChild()) value = std::max(value, getMaxKeyInternal(treeNode->getRightChild()));
    return value;
}

// Получение максимального ключа дерева
int BinaryTree::getMaxKey() const
{
    if (!root_) return std::numeric_limits<int>::min();
    return getMaxKeyInternal(root_);
}

// Рекурсивная проверка дерева на сбалансированность
bool BinaryTree::isBalancedInternal(TreeNode* node) const
{
    if (!node) return true;
    int lh = getHeightInternal(node->getLeftChild());
    int rh = getHeightInternal(node->getRightChild());
    if (std::abs(lh - rh) > 1) return false;
    return isBalancedInternal(node->getLeftChild()) && isBalancedInternal(node->getRightChild());
}

// Проверка дерева на сбалансированность
bool BinaryTree::isBalanced() const
{
    return isBalancedInternal(root_);
}

// Рекурсивное получение уровня вершины по ключу
int BinaryTree::getNodeLevelInternal(TreeNode* node, int key, int currentLevel) const
{
    if (!node) return -1;
    if (node->getKey() == key) return currentLevel;
    int leftResult = getNodeLevelInternal(node->getLeftChild(), key, currentLevel + 1);
    if (leftResult != -1) return leftResult;
    return getNodeLevelInternal(node->getRightChild(), key, currentLevel + 1);
}

// Получение уровня вершины по ключу
int BinaryTree::getNodeLevel(const int key) const
{
    return getNodeLevelInternal(root_, key, 0);
}

// Получение вектора всех ключей дерева по возрастанию
std::vector<int> BinaryTree::getKeysAscending() const
{
    std::vector<int> keys = getAllKeys();
    std::sort(keys.begin(), keys.end());
    return keys;
}

// Рекурсивное получение всех узлов
void BinaryTree::getAllNodesInternal(std::vector<TreeNode*>& nodesArray, TreeNode* treeNode) const
{
    if (!treeNode) return;
    nodesArray.push_back(treeNode);
    if (treeNode->getLeftChild()) getAllNodesInternal(nodesArray, treeNode->getLeftChild());
    if (treeNode->getRightChild()) getAllNodesInternal(nodesArray, treeNode->getRightChild());
}

// Получение всех узлов дерева
std::vector<BinaryTree::TreeNode*> BinaryTree::getAllNodes() const
{
    std::vector<BinaryTree::TreeNode*> nodesArray;
    if (root_) getAllNodesInternal(nodesArray, root_);
    return nodesArray;
}

// Рекурсивное получение всех ключей
void BinaryTree::getAllKeysInternal(std::vector<int>& keysArray, TreeNode* treeNode) const
{
    if (!treeNode) return;
    keysArray.push_back(treeNode->getKey());
    if (treeNode->getLeftChild()) getAllKeysInternal(keysArray, treeNode->getLeftChild());
    if (treeNode->getRightChild()) getAllKeysInternal(keysArray, treeNode->getRightChild());
}

// Получение всех ключей дерева
std::vector<int> BinaryTree::getAllKeys() const
{
    std::vector<int> keysArray;
    if (root_) getAllKeysInternal(keysArray, root_);
    return keysArray;
}

// Вывод дерева в консоль (обход в ширину)
void BinaryTree::printToConsole() const
{
    if (!root_) return;
    std::list<BinaryTree::TreeNode*> unprocessedNodes(1, root_);
    while (!unprocessedNodes.empty())
    {
        BinaryTree::TreeNode* treeNode = unprocessedNodes.front();
        std::cout << treeNode->getKey() << " ";
        if (treeNode->getLeftChild()) unprocessedNodes.push_back(treeNode->getLeftChild());
        if (treeNode->getRightChild()) unprocessedNodes.push_back(treeNode->getRightChild());
        unprocessedNodes.pop_front();
    }
    std::cout << std::endl;
}

// Рекурсивный горизонтальный вывод
void BinaryTree::printHorizontalInternal(TreeNode* treeNode, int marginLeft, int levelSpacing) const
{
    if (!treeNode) return;
    printHorizontalInternal(treeNode->getRightChild(), marginLeft + levelSpacing, levelSpacing);
    std::cout << std::string(marginLeft, ' ') << treeNode->getKey() << std::endl;
    printHorizontalInternal(treeNode->getLeftChild(), marginLeft + levelSpacing, levelSpacing);
}

// Вывод в консоль дерева в горизонтальном виде
void BinaryTree::printHorizontal() const
{
    if (root_)
    {
        printHorizontalInternal(root_, 0, 4);
    }
    else
    {
        std::cout << "Tree is empty" << std::endl;
    }
}

// Получение родителя узла
BinaryTree::TreeNode* BinaryTree::getParent(const BinaryTree::TreeNode* inNode)
{
    return getParentInternal(inNode, root_);
}

// Рекурсивное получение родителя узла
BinaryTree::TreeNode* BinaryTree::getParentInternal(const BinaryTree::TreeNode* inNode, BinaryTree::TreeNode* subTreeRoot)
{
    if (!inNode || !subTreeRoot || inNode == subTreeRoot) return nullptr;

    if (subTreeRoot->getLeftChild() == inNode || subTreeRoot->getRightChild() == inNode)
    {
        return subTreeRoot;
    }

    BinaryTree::TreeNode* parent = getParentInternal(inNode, subTreeRoot->getLeftChild());
    if (!parent)
    {
        parent = getParentInternal(inNode, subTreeRoot->getRightChild());
    }
    return parent;
}

// Конструктор по умолчанию узла
BinaryTree::TreeNode::TreeNode() : key_(0), leftChild_(nullptr), rightChild_(nullptr) {}

// Конструктор с параметрами узла
BinaryTree::TreeNode::TreeNode(const int key, TreeNode* leftChild, TreeNode* rightChild) :
    key_(key), leftChild_(leftChild), rightChild_(rightChild) {
}

// Получение ключа узла
int BinaryTree::TreeNode::getKey() const { return key_; }

// Изменение ключа узла
void BinaryTree::TreeNode::setKey(const int key) { key_ = key; }

// Получение левого потомка
BinaryTree::TreeNode* BinaryTree::TreeNode::getLeftChild() const { return leftChild_; }

// Изменение левого потомка
void BinaryTree::TreeNode::setLeftChild(BinaryTree::TreeNode* const leftChild) { leftChild_ = leftChild; }

// Получение правого потомка
BinaryTree::TreeNode* BinaryTree::TreeNode::getRightChild() const { return rightChild_; }

// Изменение правого потомка
void BinaryTree::TreeNode::setRightChild(BinaryTree::TreeNode* const rightChild) { rightChild_ = rightChild; }

// Получение степени узла (количество потомков)
int BinaryTree::TreeNode::getNodeDegree() const
{
    int nodeDegree = 0;
    nodeDegree += leftChild_ ? 1 : 0;
    nodeDegree += rightChild_ ? 1 : 0;
    return nodeDegree;
}

// Проверка, является ли узел листом
bool BinaryTree::TreeNode::isLeafNode() const
{
    return !leftChild_ && !rightChild_;
}

// Перегрузка оператора вывода для узла
std::ostream& operator<<(std::ostream& outputStream, const BinaryTree::TreeNode& treeNode)
{
    outputStream << treeNode.getKey();
    return outputStream;
}
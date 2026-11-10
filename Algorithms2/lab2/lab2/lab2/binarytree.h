#pragma once

#include <vector>
#include <iostream>

class BinaryTree
{
public:
    class TreeNode;

    // Конструктор по умолчанию
    BinaryTree() = default;

    // Конструктор копирования
    BinaryTree(const BinaryTree& other);

    // Конструктор перемещения (Этап 1 / Этап 3)
    BinaryTree(BinaryTree&& other) noexcept;

    // Деструктор
    ~BinaryTree();

    // Операторы присваивания и перемещения (Этап 1 / Этап 3)
    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree& operator=(BinaryTree&& other) noexcept;

    // Получение корня дерева
    TreeNode* getRoot() const;

    // Очистка дерева (удаление всех узлов)
    void clear();

    // Проверка, пусто ли дерево
    bool isEmpty() const;

    // Удаление поддеревьев узла (Этап 1 / Этап 2)
    void deleteSubtrees(TreeNode* node);

    // Получение высоты дерева
    int getHeight() const;

    // Получение количества узлов дерева
    size_t getNodeCount() const;

    // Получение минимального ключа дерева
    int getMiminalKey() const;

    // Получение максимального ключа дерева
    int getMaxKey() const;

    // Проверка дерева на сбалансированность (Этап 1 / Этап 3)
    bool isBalanced() const;

    // Получение уровня вершины по ключу (индекс уровня или -1) (Этап 2)
    int getNodeLevel(const int key) const;

    // Добавление узла в дерево (методом случайного выбора поддерева)
    TreeNode* addNode(const int key);

    // Удаление узла из дерева по ключу
    bool removeNode(const int key);

    // Поиск узла дерева по ключу
    TreeNode* findNode(const int key) const;

    // Получение вектора всех ключей по возрастанию
    std::vector<int> getKeysAscending() const;

    // Вывод в консоль дерева в горизонтальном виде
    void printHorizontal() const;

    // Получение всех узлов дерева
    std::vector<TreeNode*> getAllNodes() const;

    // Вывод дерева в консоль (обход в ширину)
    void printToConsole() const;

private:
    TreeNode* root_ = nullptr;

    void getAllNodesInternal(std::vector<TreeNode*>&, TreeNode*) const;
    int getMiminalKeyInternal(TreeNode*) const;
    int getMaxKeyInternal(TreeNode*) const;
    void destroyNode(TreeNode*);
    void printHorizontalInternal(TreeNode*, int marginLeft, int levelSpacing) const;
    TreeNode* copyNode(TreeNode* node);
    size_t getNodeCountInternal(TreeNode*) const;
    int getHeightInternal(TreeNode*) const;
    bool removeNodeInternal(TreeNode*& node, const int key);
    void getAllKeysInternal(std::vector<int>&, TreeNode*) const;
    bool isBalancedInternal(TreeNode* node) const;
    int getNodeLevelInternal(TreeNode* node, int key, int currentLevel) const;
};

class BinaryTree::TreeNode
{
public:
    TreeNode();
    TreeNode(const int key, TreeNode* leftChild = nullptr, TreeNode* rightChild = nullptr);
    ~TreeNode() = default;

    int getKey() const;
    void setKey(const int);

    TreeNode* getLeftChild() const;
    void setLeftChild(TreeNode* const);

    TreeNode* getRightChild() const;
    void setRightChild(TreeNode* const);

    int getNodeDegree() const;
    bool isLeafNode() const;

private:
    int key_;
    TreeNode* leftChild_;
    TreeNode* rightChild_;
};

std::ostream& operator<<(std::ostream&, BinaryTree::TreeNode&);
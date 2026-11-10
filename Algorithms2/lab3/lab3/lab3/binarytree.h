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

    // Конструктор перемещения
    BinaryTree(BinaryTree&& other) noexcept;

    // Деструктор
    virtual ~BinaryTree();

    // Оператор присваивания
    BinaryTree& operator=(const BinaryTree& other);

    // Оператор перемещения
    BinaryTree& operator=(BinaryTree&& other) noexcept;

    // Получение корня дерева
    TreeNode* getRoot() const;

    // Очистка дерева (удаление всех узлов)
    void clear();

    // Проверка, пусто ли дерево
    bool isEmpty() const;

    // Удаление поддеревьев узла
    void deleteSubtrees(TreeNode* node);

    // Получение высоты дерева
    int getHeight() const;

    // Получение количества узлов дерева
    size_t getNodeCount() const;

    // Получение минимального ключа дерева
    virtual int getMiminalKey() const;

    // Получение максимального ключа дерева
    virtual int getMaxKey() const;

    // Проверка дерева на сбалансированность
    bool isBalanced() const;

    // Получение уровня вершины по ключу (-1, если вершина не найдена)
    virtual int getNodeLevel(const int key) const;

    // Добавление узла в дерево (методом случайного выбора поддерева)
    virtual TreeNode* addNode(const int key);

    // Удаление узла из дерева по ключу (возвращает true, если узел был удалён)
    virtual bool removeNode(const int key);

    // Поиск узла дерева по ключу
    virtual TreeNode* findNode(const int key) const;

    // Получение вектора (std::vector<int>), содержащего все ключи дерева по возрастанию
    virtual std::vector<int> getKeysAscending() const;

    // Вывод в консоль дерева в горизонтальном виде
    void printHorizontal() const;

    // Получение всех узлов дерева
    std::vector<TreeNode*> getAllNodes() const;

    // Получение всех ключей дерева
    std::vector<int> getAllKeys() const;

    // Вывод дерева в консоль (обход в ширину)
    void printToConsole() const;

    // Получение родителя узла
    TreeNode* getParent(const TreeNode* inNode);

protected:

    // Корень дерева
    TreeNode* root_ = nullptr;

    // Рекурсивное копирование узла и всех его потомков
    TreeNode* copyNode(const TreeNode* node);

    // Рекурсивное удаление узла и всех его потомков
    void destroyNode(TreeNode* treeNode);

    // Рекурсивное получение всех ключей
    void getAllKeysInternal(std::vector<int>& keysArray, TreeNode* treeNode) const;

private:

    // Рекурсивное получение всех узлов
    void getAllNodesInternal(std::vector<TreeNode*>& nodesArray, TreeNode* treeNode) const;

    // Рекурсивный поиск минимального ключа
    int getMiminalKeyInternal(TreeNode* treeNode) const;

    // Рекурсивный поиск максимального ключа
    int getMaxKeyInternal(TreeNode* treeNode) const;

    // Рекурсивное получение высоты дерева
    int getHeightInternal(TreeNode* treeNode) const;

    // Рекурсивный подсчёт количества узлов
    size_t getNodeCountInternal(TreeNode* treeNode) const;

    // Рекурсивная проверка дерева на сбалансированность
    bool isBalancedInternal(TreeNode* node) const;

    // Рекурсивное получение уровня вершины по ключу
    int getNodeLevelInternal(TreeNode* node, int key, int currentLevel) const;

    // Рекурсивный поиск узла по ключу
    TreeNode* findNodeInternal(TreeNode* node, int key) const;

    // Рекурсивное удаление узла по ключу
    bool removeNodeInternal(TreeNode*& node, const int key);

    // Рекурсивный горизонтальный вывод
    void printHorizontalInternal(TreeNode* treeNode, int marginLeft, int levelSpacing) const;

    // Рекурсивное получение родителя узла
    TreeNode* getParentInternal(const TreeNode* inNode, TreeNode* subTreeRoot);
};

class BinaryTree::TreeNode
{
public:

    // Конструктор по умолчанию
    TreeNode();

    // Конструктор с параметрами
    TreeNode(const int key, TreeNode* leftChild = nullptr, TreeNode* rightChild = nullptr);

    // Деструктор
    ~TreeNode() = default;

    // Получение ключа узла
    int getKey() const;

    // Изменение ключа узла
    void setKey(const int key);

    // Получение левого потомка
    TreeNode* getLeftChild() const;

    // Изменение левого потомка
    void setLeftChild(TreeNode* const leftChild);

    // Получение правого потомка
    TreeNode* getRightChild() const;

    // Изменение правого потомка
    void setRightChild(TreeNode* const rightChild);

    // Получение степени узла (количество потомков)
    int getNodeDegree() const;

    // Проверка, является ли узел листом
    bool isLeafNode() const;

private:

    int key_;
    TreeNode* leftChild_ = nullptr;
    TreeNode* rightChild_ = nullptr;
};

// Перегрузка оператора вывода для узла
std::ostream& operator<<(std::ostream& outputStream, const BinaryTree::TreeNode& treeNode);
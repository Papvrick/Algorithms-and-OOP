#pragma once

#include "binarytree.h"

// Класс двоичного дерева поиска
class BinarySearchTree : public BinaryTree
{
public:

    // Конструктор по умолчанию
    BinarySearchTree() = default;

    // Конструктор копирования
    BinarySearchTree(const BinarySearchTree& other);

    // Конструктор перемещения
    BinarySearchTree(BinarySearchTree&& other) noexcept;

    // Деструктор
    virtual ~BinarySearchTree() = default;

    // Оператор присваивания
    BinarySearchTree& operator=(const BinarySearchTree& other);

    // Оператор перемещения
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;

    // Добавление узла по правилам BST
    virtual TreeNode* addNode(const int key) override;

    // Удаление узла по правилам BST
    virtual bool removeNode(const int key) override;

    // Поиск узла по правилам BST
    virtual TreeNode* findNode(const int key) const override;

    // Получение минимального ключа BST (спуск в левый лист)
    virtual int getMiminalKey() const override;

    // Получение максимального ключа BST (спуск в правый лист)
    virtual int getMaxKey() const override;

    // Получение уровня вершины по ключу BST
    virtual int getNodeLevel(const int key) const override;

    // Получение отсортированных ключей (LNR симметричный обход)
    virtual std::vector<int> getKeysAscending() const override;

private:

    // Рекурсивное удаление узла из BST
    bool removeNodeBST(TreeNode*& node, const int key);

    // Симметричный обход (LNR)
    void inOrderKeysInternal(std::vector<int>& keys, TreeNode* node) const;
};
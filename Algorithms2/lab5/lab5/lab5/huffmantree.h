#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "booleanvector.h"

using uint = unsigned int;

const uint NUM_CHARS = 256;

class HuffmanTree
{
public:

    class TreeNode;

    // Конструктор по умолчанию
    HuffmanTree() = default;

    // Деструктор с освобождением памяти дерева
    ~HuffmanTree();

    // Запрет копирования во избежание двойного освобождения указателей
    HuffmanTree(const HuffmanTree&) = delete;
    HuffmanTree& operator=(const HuffmanTree&) = delete;

    // Построение дерева по входной строке текста (старые узлы очищаются)
    void build(const std::string& text);

    // Кодирование файла: возвращает коэффициент сжатия или -1.0 при ошибке
    double encode(const std::string& inputFileName, const std::string& outputFileName);

    // Декодирование файла: возвращает true при успехе, false при ошибке
    bool decode(const std::string& inputFileName, const std::string& outputFileName);

    // Экспорт структуры и частот дерева в файл
    bool exportTree(const std::string& fileName) const;

    // Импорт дерева из файла
    bool importTree(const std::string& fileName);

    // Проверка, пусто ли дерево
    bool isEmpty() const;

    // Очистка дерева
    void clear();

private:

    // Рекурсивное удаление узлов
    void destroyNode(TreeNode* node);

    // Получение битового кода одного символа по дереву
    std::vector<bool> encodeSymbol(const char symbol) const;

    // Вспомогательный метод обхода для построения кодов
    bool findSymbolCode(const TreeNode* node, const char symbol, std::vector<bool>& code) const;

    // Корень дерева Хаффмана
    TreeNode* root_ = nullptr;
};

class HuffmanTree::TreeNode
{
public:

    // Конструктор листа по частоте и символу
    TreeNode(const uint inFrequency, const char inChar);

    // Конструктор промежуточного узла по двум потомкам
    TreeNode(TreeNode* const leftChild, TreeNode* const rightChild);

    // Деструктор по умолчанию
    ~TreeNode() = default;

    // Получение суммарной частоты узла
    uint getFrequency() const;

    // Установка частоты узла
    void setFrequency(const uint newFrequency);

    // Проверка наличия символа в поддереве узла через BooleanVector
    bool containsCharacter(const unsigned char symbol) const;

    // Получение ссылки на битовый вектор символов
    const BooleanVector& getCharacterSet() const;

    // Проверка, является ли узел листом
    bool isLeafNode() const;

    // Получение левого потомка
    HuffmanTree::TreeNode* getLeftChild() const;

    // Получение правого потомка
    HuffmanTree::TreeNode* getRightChild() const;

    // Получение символа листа
    char getCharacter() const;

private:

    HuffmanTree::TreeNode* leftChild_ = nullptr;
    HuffmanTree::TreeNode* rightChild_ = nullptr;

    // Хранение множества символов узла через BooleanVector (256 бит)
    BooleanVector characterSet_;

    // Частота встречаемости
    uint frequency_ = 0;

    // Символ (актуален только для листьев)
    char symbol_ = '\0';
};
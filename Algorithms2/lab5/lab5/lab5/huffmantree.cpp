#include "huffmantree.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

// Реализация TreeNode

// Конструктор листового узла
HuffmanTree::TreeNode::TreeNode(const uint inFrequency, const char inChar)
    : leftChild_(nullptr), rightChild_(nullptr), characterSet_(NUM_CHARS, false),
    frequency_(inFrequency), symbol_(inChar)
{
    characterSet_.setBit(static_cast<unsigned char>(inChar), true);
}

// Конструктор промежуточного узла объединения
HuffmanTree::TreeNode::TreeNode(TreeNode* const leftChild, TreeNode* const rightChild)
    : leftChild_(leftChild), rightChild_(rightChild), characterSet_(NUM_CHARS, false),
    frequency_(0), symbol_('\0')
{
    if (leftChild)
    {
        frequency_ += leftChild->getFrequency();
        characterSet_ |= leftChild->getCharacterSet();
    }
    if (rightChild)
    {
        frequency_ += rightChild->getFrequency();
        characterSet_ |= rightChild->getCharacterSet();
    }
}

uint HuffmanTree::TreeNode::getFrequency() const
{
    return frequency_;
}

void HuffmanTree::TreeNode::setFrequency(const uint newFrequency)
{
    frequency_ = newFrequency;
}

bool HuffmanTree::TreeNode::containsCharacter(const unsigned char symbol) const
{
    if (symbol >= NUM_CHARS) return false;
    return characterSet_[symbol];
}

const BooleanVector& HuffmanTree::TreeNode::getCharacterSet() const
{
    return characterSet_;
}

bool HuffmanTree::TreeNode::isLeafNode() const
{
    return !leftChild_ && !rightChild_;
}

HuffmanTree::TreeNode* HuffmanTree::TreeNode::getLeftChild() const
{
    return leftChild_;
}

HuffmanTree::TreeNode* HuffmanTree::TreeNode::getRightChild() const
{
    return rightChild_;
}

char HuffmanTree::TreeNode::getCharacter() const
{
    return symbol_;
}

// Реализация HuffmanTree

HuffmanTree::~HuffmanTree()
{
    clear();
}

bool HuffmanTree::isEmpty() const
{
    return root_ == nullptr;
}

void HuffmanTree::clear()
{
    if (root_)
    {
        destroyNode(root_);
        root_ = nullptr;
    }
}

void HuffmanTree::destroyNode(TreeNode* node)
{
    if (!node) return;
    destroyNode(node->getLeftChild());
    destroyNode(node->getRightChild());
    delete node;
}

// Построение дерева по тексту
void HuffmanTree::build(const std::string& text)
{
    clear();
    if (text.empty()) return;

    // Подсчет частот всех 256 символов
    uint frequencies[NUM_CHARS] = { 0 };
    for (char ch : text)
    {
        frequencies[static_cast<unsigned char>(ch)]++;
    }

    // Создание листьев для присутствующих символов
    std::vector<TreeNode*> nodes;
    for (uint i = 0; i < NUM_CHARS; ++i)
    {
        if (frequencies[i] > 0)
        {
            nodes.push_back(new TreeNode(frequencies[i], static_cast<char>(i)));
        }
    }

    if (nodes.empty()) return;

    // Особый случай: текст состоит из одного уникального символа
    if (nodes.size() == 1)
    {
        root_ = new TreeNode(nodes[0], nullptr);
        return;
    }

    // Сортировка по возрастанию частоты
    std::sort(nodes.begin(), nodes.end(), [](const TreeNode* a, const TreeNode* b) {
        return a->getFrequency() < b->getFrequency();
        });

    // Построение дерева объединением двух минимальных узлов
    while (nodes.size() > 1)
    {
        TreeNode* left = nodes[0];
        TreeNode* right = nodes[1];
        nodes.erase(nodes.begin(), nodes.begin() + 2);

        TreeNode* parent = new TreeNode(left, right);

        // Вставка с сохранением порядка сортировки
        auto insertPos = std::lower_bound(nodes.begin(), nodes.end(), parent,
            [](const TreeNode* a, const TreeNode* b) {
                return a->getFrequency() < b->getFrequency();
            });
        nodes.insert(insertPos, parent);
    }

    root_ = nodes[0];
}

// Получение префиксного кода символа обходом дерева
std::vector<bool> HuffmanTree::encodeSymbol(const char symbol) const
{
    std::vector<bool> code;
    if (!root_) return code;

    // Если в дереве только один листовой символ
    if (root_->isLeafNode())
    {
        code.push_back(false);
        return code;
    }

    TreeNode* currentNode = root_;
    while (!currentNode->isLeafNode())
    {
        TreeNode* left = currentNode->getLeftChild();
        TreeNode* right = currentNode->getRightChild();

        if (left && left->containsCharacter(static_cast<unsigned char>(symbol)))
        {
            code.push_back(false); // 0 — шаг влево
            currentNode = left;
        }
        else if (right && right->containsCharacter(static_cast<unsigned char>(symbol)))
        {
            code.push_back(true);  // 1 — шаг вправо
            currentNode = right;
        }
        else
        {
            break;
        }
    }

    return code;
}

// Кодирование файла в сжатый бинарный файл
double HuffmanTree::encode(const std::string& inputFileName, const std::string& outputFileName)
{
    std::ifstream inFile(inputFileName, std::ios::binary);
    if (!inFile.is_open()) return -1.0;

    // Чтение всего файла в строку байтов
    std::string text((std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());
    inFile.close();

    if (text.empty()) return -1.0;

    // Если дерево еще не построено — строим автоматически
    if (isEmpty())
    {
        build(text);
    }

    // Формирование вектора битов через обход дерева Хаффмана
    std::vector<bool> encodedBits;
    encodedBits.reserve(text.size() * 4);

    for (char ch : text)
    {
        std::vector<bool> symbolCode = encodeSymbol(ch);
        if (symbolCode.empty() && !root_->isLeafNode())
        {
            // Символ не найден в текущем дереве
            return -1.0;
        }
        encodedBits.insert(encodedBits.end(), symbolCode.begin(), symbolCode.end());
    }

    uint32_t totalBits = static_cast<uint32_t>(encodedBits.size());

    // Запись битов в BooleanVector для побитового хранения
    BooleanVector bitVector(totalBits, false);
    for (uint32_t i = 0; i < totalBits; ++i)
    {
        bitVector.setBit(i, encodedBits[i]);
    }

    // Сохранение в выходной файл в виде бит
    if (!outputFileName.empty())
    {
        std::ofstream outFile(outputFileName, std::ios::binary);
        if (!outFile.is_open()) return -1.0;

        // Заголовок: 4 байта количества битов
        outFile.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));

        // Выгрузка упакованных байтов
        uint32_t numBytes = (totalBits + 7) / 8;
        for (uint32_t byteIdx = 0; byteIdx < numBytes; ++byteIdx)
        {
            uint8_t byteVal = 0;
            for (uint32_t bitIdx = 0; bitIdx < 8; ++bitIdx)
            {
                uint32_t globalBitIdx = byteIdx * 8 + bitIdx;
                if (globalBitIdx < totalBits && bitVector[globalBitIdx])
                {
                    byteVal |= (1 << bitIdx);
                }
            }
            outFile.put(static_cast<char>(byteVal));
        }
        outFile.close();
    }

    // Подсчет коэффициента сжатия: (размер кода / размер исходного) * 100%
    double originalBits = static_cast<double>(text.size() * 8);
    double compressedBits = static_cast<double>(totalBits);
    return (compressedBits / originalBits) * 100.0;
}

// Декодирование сжатого бинарного файла
bool HuffmanTree::decode(const std::string& inputFileName, const std::string& outputFileName)
{
    if (isEmpty()) return false;

    std::ifstream inFile(inputFileName, std::ios::binary);
    if (!inFile.is_open()) return false;

    // Чтение количества бит
    uint32_t totalBits = 0;
    if (!inFile.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits)))
    {
        return false;
    }

    if (totalBits == 0) return false;

    // Считывание полезных байтов
    uint32_t numBytes = (totalBits + 7) / 8;
    std::vector<uint8_t> byteBuffer(numBytes);
    inFile.read(reinterpret_cast<char*>(byteBuffer.data()), numBytes);
    inFile.close();

    // Загрузка данных в BooleanVector
    BooleanVector bitVector(totalBits, false);
    for (uint32_t i = 0; i < totalBits; ++i)
    {
        uint32_t byteIdx = i / 8;
        uint32_t bitIdx = i % 8;
        bool val = (byteBuffer[byteIdx] >> bitIdx) & 1;
        bitVector.setBit(i, val);
    }

    // Декодирование по дереву Хаффмана
    std::string decodedText;
    TreeNode* currentNode = root_;

    for (uint32_t i = 0; i < totalBits; ++i)
    {
        bool bit = bitVector[i];

        if (currentNode->isLeafNode())
        {
            decodedText += currentNode->getCharacter();
            currentNode = root_;
        }

        if (!bit)
        {
            currentNode = currentNode->getLeftChild();
        }
        else
        {
            currentNode = currentNode->getRightChild();
        }

        if (!currentNode) return false;

        if (currentNode->isLeafNode())
        {
            decodedText += currentNode->getCharacter();
            currentNode = root_;
        }
    }

    std::ofstream outFile(outputFileName, std::ios::binary);
    if (!outFile.is_open()) return false;

    outFile.write(decodedText.data(), decodedText.size());
    outFile.close();

    return true;
}

// Экспорт частотной таблицы дерева в файл
bool HuffmanTree::exportTree(const std::string& fileName) const
{
    if (isEmpty()) return false;

    std::ofstream outFile(fileName, std::ios::binary);
    if (!outFile.is_open()) return false;

    // Запись 256 частот символов (для точного восстановления)
    uint frequencies[NUM_CHARS] = { 0 };

    auto collectFrequencies = [&](auto& self, const TreeNode* node) -> void {
        if (!node) return;
        if (node->isLeafNode())
        {
            frequencies[static_cast<unsigned char>(node->getCharacter())] = node->getFrequency();
            return;
        }
        self(self, node->getLeftChild());
        self(self, node->getRightChild());
        };

    collectFrequencies(collectFrequencies, root_);

    outFile.write(reinterpret_cast<const char*>(frequencies), sizeof(frequencies));
    return true;
}

// Импорт дерева из файла частот
bool HuffmanTree::importTree(const std::string& fileName)
{
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile.is_open()) return false;

    uint frequencies[NUM_CHARS] = { 0 };
    if (!inFile.read(reinterpret_cast<char*>(frequencies), sizeof(frequencies)))
    {
        return false;
    }
    inFile.close();

    clear();

    // Восстановление списка листьев
    std::vector<TreeNode*> nodes;
    for (uint i = 0; i < NUM_CHARS; ++i)
    {
        if (frequencies[i] > 0)
        {
            nodes.push_back(new TreeNode(frequencies[i], static_cast<char>(i)));
        }
    }

    if (nodes.empty()) return false;

    if (nodes.size() == 1)
    {
        root_ = new TreeNode(nodes[0], nullptr);
        return true;
    }

    std::sort(nodes.begin(), nodes.end(), [](const TreeNode* a, const TreeNode* b) {
        return a->getFrequency() < b->getFrequency();
        });

    while (nodes.size() > 1)
    {
        TreeNode* left = nodes[0];
        TreeNode* right = nodes[1];
        nodes.erase(nodes.begin(), nodes.begin() + 2);

        TreeNode* parent = new TreeNode(left, right);

        auto insertPos = std::lower_bound(nodes.begin(), nodes.end(), parent,
            [](const TreeNode* a, const TreeNode* b) {
                return a->getFrequency() < b->getFrequency();
            });
        nodes.insert(insertPos, parent);
    }

    root_ = nodes[0];
    return true;
}
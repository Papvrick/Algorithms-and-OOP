#pragma once

#include <QWidget>
#include <QFont>
#include <QPen>
#include <QBrush>
#include "binarytree.h"

// Виджет для графической отрисовки дерева
class CanvasWidget : public QWidget
{
    Q_OBJECT

public:

    // Конструктор
    CanvasWidget(QWidget* parent = nullptr);

    // Событие перерисовки виджета
    virtual void paintEvent(QPaintEvent* event) override;

    // Установка подсвечиваемого ключа (для операции поиска)
    void setHighlightedKey(int key);

private:

    // Рекурсивная отрисовка узла и связей
    void drawTreeNode(BinaryTree::TreeNode* treeNode, QPainter& painter, int width, int xOffset, int yOffset);

    // Константы размеров
    const int NODE_CIRCLE_RADIUS = 20;
    const int PADDING_BETWEEN_TREE_LEVELS = 75;

    // Ключ для подсветки узла
    int highlightedKey_ = -1;

    // Элементы стиля отрисовки
    QFont treeNodeKeyFont;
    QPen nodeBorderPen;
    QPen linePen;
    QPen nodeKeyTextPen;
    QBrush nodeBrush;
    QBrush highlightedBrush;
};
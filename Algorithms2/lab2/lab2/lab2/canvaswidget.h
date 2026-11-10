#pragma once

#include <QWidget>
#include <QFont>
#include <QPen>
#include <QBrush>
#include "binarytree.h"

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    CanvasWidget(QWidget* parent = nullptr);
    virtual void paintEvent(QPaintEvent* event) override;
    void setHighlightedKey(int key);

private:
    void drawTreeNode(BinaryTree::TreeNode* treeNode, QPainter& painter, int width, int xOffset, int yOffset);

    const int NODE_CIRCLE_RADIUS = 20;
    const int PADDING_BETWEEN_TREE_LEVELS = 80;

    int highlightedKey_ = -1;

    QFont treeNodeKeyFont;
    QPen nodeBorderPen;
    QPen linePen;
    QPen nodeKeyTextPen;
    QBrush nodeBrush;
    QBrush highlightedBrush;
};
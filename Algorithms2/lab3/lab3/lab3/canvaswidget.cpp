#include <QPainter>
#include <QPaintEvent>
#include "mainwindow.h"
#include "binarytree.h"
#include "canvaswidget.h"

// Конструктор виджета отрисовки
CanvasWidget::CanvasWidget(QWidget* parent) : QWidget(parent)
{
    treeNodeKeyFont = QFont("Arial", 11, QFont::Weight::Bold);

    nodeBorderPen = QPen(QColor(0, 0, 0));
    nodeBorderPen.setWidth(2);

    linePen = QPen(QColor(60, 60, 60));
    linePen.setWidth(2);

    nodeKeyTextPen = QPen(QColor(0, 0, 0));
    nodeBrush = QBrush(QColor(255, 192, 203));
    highlightedBrush = QBrush(QColor(144, 238, 144));
}

// Установка ключа для подсветки
void CanvasWidget::setHighlightedKey(int key)
{
    highlightedKey_ = key;
    update();
}

// Отрисовка дерева
void CanvasWidget::paintEvent(QPaintEvent* event)
{
    MainWindow* mainWindowInstance = qobject_cast<MainWindow*>(window());
    BinaryTree* tree = mainWindowInstance ? mainWindowInstance->getTree() : nullptr;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(QColor(255, 255, 255)));

    int rectWidth = event->rect().width();

    if (tree && tree->getRoot())
    {
        drawTreeNode(tree->getRoot(), painter, rectWidth, rectWidth / 2, 25);
    }
    else
    {
        painter.setFont(QFont("Arial", 14));
        painter.setPen(QColor(128, 128, 128));
        painter.drawText(event->rect(), Qt::AlignCenter, "Дерево пусто");
    }
}

// Рекурсивная отрисовка узла и его потомков
void CanvasWidget::drawTreeNode(BinaryTree::TreeNode* treeNode, QPainter& painter, int width, int xOffset, int yOffset)
{
    QPoint nodeCenterCoords(xOffset, yOffset + NODE_CIRCLE_RADIUS);
    int halfWidth = width / 2;
    int bottomNodeOffset = yOffset + PADDING_BETWEEN_TREE_LEVELS;

    // Отрисовка левого потомка и соединительной линии
    if (treeNode->getLeftChild())
    {
        int leftSubTreeXOffset = xOffset - halfWidth / 2;
        painter.setPen(linePen);
        painter.drawLine(nodeCenterCoords, QPoint(leftSubTreeXOffset, bottomNodeOffset + NODE_CIRCLE_RADIUS));
        drawTreeNode(treeNode->getLeftChild(), painter, halfWidth, leftSubTreeXOffset, bottomNodeOffset);
    }

    // Отрисовка правого потомка и соединительной линии
    if (treeNode->getRightChild())
    {
        int rightSubTreeXOffset = xOffset + halfWidth / 2;
        painter.setPen(linePen);
        painter.drawLine(nodeCenterCoords, QPoint(rightSubTreeXOffset, bottomNodeOffset + NODE_CIRCLE_RADIUS));
        drawTreeNode(treeNode->getRightChild(), painter, halfWidth, rightSubTreeXOffset, bottomNodeOffset);
    }

    // Заливка кружка
    painter.setBrush(treeNode->getKey() == highlightedKey_ ? highlightedBrush : nodeBrush);
    painter.setPen(nodeBorderPen);
    painter.drawEllipse(nodeCenterCoords, NODE_CIRCLE_RADIUS, NODE_CIRCLE_RADIUS);

    // Отрисовка ключа внутри кружка
    painter.setFont(treeNodeKeyFont);
    painter.setPen(nodeKeyTextPen);
    QRect textRect(xOffset - NODE_CIRCLE_RADIUS, yOffset, NODE_CIRCLE_RADIUS * 2, NODE_CIRCLE_RADIUS * 2);
    painter.drawText(textRect, Qt::AlignCenter, QString::number(treeNode->getKey()));
}
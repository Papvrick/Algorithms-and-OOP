#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QMessageBox>
#include <time.h>
#include <stdlib.h>

#include "canvaswidget.h"
#include "mainwindow.h"
#include "binarytree.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    tree_ = new BinaryTree();

    // Заполнение начальными 15 узлами
    for (int i = 0; i < 15; i++)
    {
        tree_->addNode(rand() % 100);
    }

    setWindowTitle("Binary Tree GUI");
    resize(1000, 720);

    mainLayout_ = new QHBoxLayout();
    canvasWidget_ = new CanvasWidget();
    mainLayout_->addWidget(canvasWidget_, 1);

    // Панель элементов управления
    QWidget* controlPanel = new QWidget(this);
    controlPanel->setFixedWidth(260);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);

    controlLayout->addWidget(new QLabel("Ключ для операций:", this));
    keySpinBox_ = new QSpinBox(this);
    keySpinBox_->setRange(-10000, 10000);
    keySpinBox_->setValue(50);
    controlLayout->addWidget(keySpinBox_);

    QPushButton* btnAdd = new QPushButton("Добавить узел", this);
    QPushButton* btnRemove = new QPushButton("Удалить узел", this);
    QPushButton* btnFind = new QPushButton("Найти узел", this);
    QPushButton* btnDelSub = new QPushButton("Удалить поддеревья узла", this);
    QPushButton* btnBalance = new QPushButton("Проверить баланс", this);
    QPushButton* btnClear = new QPushButton("Очистить дерево", this);

    controlLayout->addWidget(btnAdd);
    controlLayout->addWidget(btnRemove);
    controlLayout->addWidget(btnFind);
    controlLayout->addWidget(btnDelSub);
    controlLayout->addWidget(btnBalance);
    controlLayout->addWidget(btnClear);

    infoLabel_ = new QLabel(this);
    infoLabel_->setWordWrap(true);
    controlLayout->addWidget(infoLabel_);
    controlLayout->addStretch();

    mainLayout_->addWidget(controlPanel);

    container_ = new QWidget();
    setCentralWidget(container_);
    container_->setLayout(mainLayout_);

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(btnFind, &QPushButton::clicked, this, &MainWindow::onFindClicked);
    connect(btnDelSub, &QPushButton::clicked, this, &MainWindow::onDeleteSubtreesClicked);
    connect(btnBalance, &QPushButton::clicked, this, &MainWindow::onCheckBalancedClicked);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    updateTreeInfo();
}

MainWindow::~MainWindow()
{
    delete tree_;
}

void MainWindow::updateTreeInfo()
{
    QString info = QString("Количество узлов: %1\nВысота: %2\n")
        .arg(tree_->getNodeCount())
        .arg(tree_->getHeight());

    if (!tree_->isEmpty())
    {
        info += QString("Минимум: %1\nМаксимум: %2\n").arg(tree_->getMiminalKey()).arg(tree_->getMaxKey());
    }

    std::vector<int> sortedKeys = tree_->getKeysAscending();
    info += "Ключи по возрастанию:\n[";
    for (size_t i = 0; i < sortedKeys.size(); ++i)
    {
        info += QString::number(sortedKeys[i]) + (i + 1 < sortedKeys.size() ? ", " : "");
    }
    info += "]";

    infoLabel_->setText(info);
}

void MainWindow::onAddClicked()
{
    tree_->addNode(keySpinBox_->value());
    canvasWidget_->setHighlightedKey(-1);
    canvasWidget_->update();
    updateTreeInfo();
}

void MainWindow::onRemoveClicked()
{
    int key = keySpinBox_->value();
    if (!tree_->removeNode(key))
    {
        QMessageBox::information(this, "Удаление", "Узел с таким ключом не найден.");
    }
    canvasWidget_->setHighlightedKey(-1);
    canvasWidget_->update();
    updateTreeInfo();
}

void MainWindow::onFindClicked()
{
    int key = keySpinBox_->value();
    BinaryTree::TreeNode* found = tree_->findNode(key);
    if (found)
    {
        int level = tree_->getNodeLevel(key);
        canvasWidget_->setHighlightedKey(key);
        QMessageBox::information(this, "Поиск", QString("Узел %1 найден на уровне %2!").arg(key).arg(level));
    }
    else
    {
        canvasWidget_->setHighlightedKey(-1);
        QMessageBox::warning(this, "Поиск", "Узел не найден.");
    }
}

void MainWindow::onDeleteSubtreesClicked()
{
    int key = keySpinBox_->value();
    BinaryTree::TreeNode* node = tree_->findNode(key);
    if (node)
    {
        tree_->deleteSubtrees(node);
        canvasWidget_->update();
        updateTreeInfo();
        QMessageBox::information(this, "Поддеревья", QString("Поддеревья узла %1 удалены.").arg(key));
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Узел не найден.");
    }
}

void MainWindow::onCheckBalancedClicked()
{
    if (tree_->isBalanced())
    {
        QMessageBox::information(this, "Балансировка", "Дерево сбалансировано.");
    }
    else
    {
        QMessageBox::information(this, "Балансировка", "Дерево НЕ сбалансировано.");
    }
}

void MainWindow::onClearClicked()
{
    tree_->clear();
    canvasWidget_->setHighlightedKey(-1);
    canvasWidget_->update();
    updateTreeInfo();
}
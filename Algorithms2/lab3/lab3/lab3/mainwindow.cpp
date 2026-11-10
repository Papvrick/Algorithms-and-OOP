#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <time.h>
#include <stdlib.h>

#include "canvaswidget.h"
#include "mainwindow.h"
#include "binarytree.h"
#include "binarysearchtree.h"

// Конструктор главного окна
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // По умолчанию создаем обычное двоичное дерево
    currentTree_ = new BinaryTree();
    for (int i = 0; i < 10; ++i)
    {
        currentTree_->addNode(rand() % 100);
    }

    setWindowTitle("Двоичные деревья (Лабораторная работа)");
    resize(1050, 750);

    mainLayout_ = new QHBoxLayout();
    canvasWidget_ = new CanvasWidget(this);
    mainLayout_->addWidget(canvasWidget_, 1);

    // Боковая панель управления
    QWidget* controlPanel = new QWidget(this);
    controlPanel->setFixedWidth(280);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);

    // Выбор типа дерева (Этап 3)
    controlLayout->addWidget(new QLabel("Тип дерева:", this));
    treeTypeComboBox_ = new QComboBox(this);
    treeTypeComboBox_->addItem("Обычное двоичное дерево");
    treeTypeComboBox_->addItem("Двоичное дерево поиска (BST)");
    controlLayout->addWidget(treeTypeComboBox_);

    // Ввод ключа
    controlLayout->addWidget(new QLabel("Ключ узла:", this));
    keySpinBox_ = new QSpinBox(this);
    keySpinBox_->setRange(-9999, 9999);
    keySpinBox_->setValue(50);
    controlLayout->addWidget(keySpinBox_);

    // Кнопки операций
    QPushButton* btnAdd = new QPushButton("Добавить узел", this);
    QPushButton* btnRemove = new QPushButton("Удалить узел", this);
    QPushButton* btnFind = new QPushButton("Найти узел", this);
    QPushButton* btnDelSub = new QPushButton("Удалить поддеревья", this);
    QPushButton* btnBalance = new QPushButton("Проверить баланс", this);
    QPushButton* btnClear = new QPushButton("Очистить дерево", this);

    controlLayout->addWidget(btnAdd);
    controlLayout->addWidget(btnRemove);
    controlLayout->addWidget(btnFind);
    controlLayout->addWidget(btnDelSub);
    controlLayout->addWidget(btnBalance);
    controlLayout->addWidget(btnClear);

    // Метка информации
    infoLabel_ = new QLabel(this);
    infoLabel_->setWordWrap(true);
    controlLayout->addWidget(infoLabel_);
    controlLayout->addStretch();

    mainLayout_->addWidget(controlPanel);

    container_ = new QWidget();
    setCentralWidget(container_);
    container_->setLayout(mainLayout_);

    // Сигналы и слоты
    connect(treeTypeComboBox_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTreeTypeChanged);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(btnFind, &QPushButton::clicked, this, &MainWindow::onFindClicked);
    connect(btnDelSub, &QPushButton::clicked, this, &MainWindow::onDeleteSubtreesClicked);
    connect(btnBalance, &QPushButton::clicked, this, &MainWindow::onCheckBalancedClicked);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    updateTreeInfo();
}

// Деструктор главного окна
MainWindow::~MainWindow()
{
    delete currentTree_;
}

// Переключение между обычным деревом и BST
void MainWindow::onTreeTypeChanged(int index)
{
    delete currentTree_;

    if (index == 0)
    {
        currentTree_ = new BinaryTree();
    }
    else
    {
        currentTree_ = new BinarySearchTree();
    }

    for (int i = 0; i < 10; ++i)
    {
        currentTree_->addNode(rand() % 100);
    }

    canvasWidget_->setHighlightedKey(-1);
    canvasWidget_->update();
    updateTreeInfo();
}

// Обновление текста с характеристиками дерева
void MainWindow::updateTreeInfo()
{
    QString info = QString("Узлов: %1\nВысота: %2\n")
        .arg(currentTree_->getNodeCount())
        .arg(currentTree_->getHeight());

    if (!currentTree_->isEmpty())
    {
        info += QString("Минимум: %1\nМаксимум: %2\n").arg(currentTree_->getMiminalKey()).arg(currentTree_->getMaxKey());
    }

    std::vector<int> sorted = currentTree_->getKeysAscending();
    info += "Ключи по возрастанию:\n[";
    for (size_t i = 0; i < sorted.size(); ++i)
    {
        info += QString::number(sorted[i]) + (i + 1 < sorted.size() ? ", " : "");
    }
    info += "]";

    infoLabel_->setText(info);
}

// Слот добавления
void MainWindow::onAddClicked()
{
    currentTree_->addNode(keySpinBox_->value());
    canvasWidget_->setHighlightedKey(-1);
    canvasWidget_->update();
    updateTreeInfo();
}

// Слот удаления
void MainWindow::onRemoveClicked()
{
    int key = keySpinBox_->value();
    if (!currentTree_->removeNode(key))
    {
        QMessageBox::information(this, "Удаление", "Узел с таким ключом не найден.");
    }
    canvasWidget_->setHighlightedKey(-1);
    canvasWidget_->update();
    updateTreeInfo();
}

// Слот поиска
void MainWindow::onFindClicked()
{
    int key = keySpinBox_->value();
    BinaryTree::TreeNode* found = currentTree_->findNode(key);
    if (found)
    {
        int level = currentTree_->getNodeLevel(key);
        canvasWidget_->setHighlightedKey(key);
        QMessageBox::information(this, "Поиск", QString("Узел %1 найден на уровне: %2").arg(key).arg(level));
    }
    else
    {
        canvasWidget_->setHighlightedKey(-1);
        QMessageBox::warning(this, "Поиск", "Узел не найден.");
    }
}

// Слот удаления поддеревьев
void MainWindow::onDeleteSubtreesClicked()
{
    int key = keySpinBox_->value();
    BinaryTree::TreeNode* node = currentTree_->findNode(key);
    if (node)
    {
        currentTree_->deleteSubtrees(node);
        canvasWidget_->setHighlightedKey(-1);
        canvasWidget_->update();
        updateTreeInfo();
        QMessageBox::information(this, "Успех", QString("Поддеревья узла %1 удалены.").arg(key));
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Узел не найден.");
    }
}

// Слот проверки сбалансированности
void MainWindow::onCheckBalancedClicked()
{
    QString status = currentTree_->isBalanced() ? "Дерево сбалансировано!" : "Дерево НЕ сбалансировано.";
    QMessageBox::information(this, "Баланс", status);
}

// Слот очистки
void MainWindow::onClearClicked()
{
    currentTree_->clear();
    canvasWidget_->setHighlightedKey(-1);
    canvasWidget_->update();
    updateTreeInfo();
}
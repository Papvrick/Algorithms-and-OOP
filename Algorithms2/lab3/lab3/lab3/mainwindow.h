#pragma once

#include <QMainWindow>

class QHBoxLayout;
class QVBoxLayout;
class QWidget;
class QSpinBox;
class QLabel;
class QComboBox;

class CanvasWidget;
class BinaryTree;

// Главное окно приложения
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // Конструктор
    MainWindow(QWidget* parent = nullptr);

    // Деструктор
    ~MainWindow();

    // Получение указателя на активное дерево через базовый класс
    BinaryTree* getTree() { return currentTree_; }

private slots:

    // Слоты кнопок
    void onAddClicked();
    void onRemoveClicked();
    void onFindClicked();
    void onDeleteSubtreesClicked();
    void onCheckBalancedClicked();
    void onClearClicked();
    void onTreeTypeChanged(int index);

private:

    // Обновление характеристик дерева в метке
    void updateTreeInfo();

    QWidget* container_;
    CanvasWidget* canvasWidget_;
    QHBoxLayout* mainLayout_;

    // Работа с деревом через указатель на базовый класс
    BinaryTree* currentTree_ = nullptr;

    QComboBox* treeTypeComboBox_;
    QSpinBox* keySpinBox_;
    QLabel* infoLabel_;
};
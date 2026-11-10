#pragma once

#include <QMainWindow>

class QHBoxLayout;
class QVBoxLayout;
class QWidget;
class QSpinBox;
class QLabel;

class CanvasWidget;
class BinaryTree;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    BinaryTree* getTree() { return tree_; }

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onFindClicked();
    void onDeleteSubtreesClicked();
    void onCheckBalancedClicked();
    void onClearClicked();

private:
    void updateTreeInfo();

    QWidget* container_;
    CanvasWidget* canvasWidget_;
    QHBoxLayout* mainLayout_;
    BinaryTree* tree_;

    QSpinBox* keySpinBox_;
    QLabel* infoLabel_;
};
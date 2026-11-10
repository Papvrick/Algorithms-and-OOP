#pragma once

#include <QMainWindow>

class QHBoxLayout;
class QVBoxLayout;
class QTableWidget;
class QLineEdit;
class QPushButton;
class QComboBox;
class QLabel;

class HashTable;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:

    void onInsertClicked();
    void onRemoveClicked();
    void onFindClicked();
    void onResizeClicked();
    void onHashFunctionChanged(int index);

private:

    void updateTableGui();

    QWidget* containter_;
    QHBoxLayout* mainLayout_;
    QVBoxLayout* controlPanelLayout_;

    QTableWidget* tableWidget_;

    QLineEdit* keyEdit_;
    QLineEdit* valueEdit_;
    QLineEdit* resizeEdit_;
    QComboBox* hashFuncComboBox_;
    QLabel* statusLabel_;

    HashTable* testTable_;
};
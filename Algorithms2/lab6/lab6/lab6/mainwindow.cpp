#include <random>
#include <string>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "hashtable.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    // Инициализация хеш-таблицы (метод внешних цепочек)
    testTable_ = new HashTable(15);

    // Начальное автозаполнение данными
    std::random_device randomDevice;
    std::mt19937_64 generator(randomDevice());
    std::uniform_int_distribution<uint> distribution(100, 9999);

    for (int i = 0; i < 18; ++i)
    {
        uint key = distribution(generator);
        testTable_->insert(key, "Val_" + std::to_string(i + 1));
    }

    setWindowTitle("Лабораторная 6: Хеш-таблица (Метод внешних цепочек)");
    resize(1020, 720);

    containter_ = new QWidget(this);
    mainLayout_ = new QHBoxLayout(containter_);

    // Табличный виджет
    tableWidget_ = new QTableWidget(0, 2, this);
    tableWidget_->setHorizontalHeaderLabels(QStringList() << "Хеш" << "Внешняя цепочка (Ключ - Значение)");
    tableWidget_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableWidget_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableWidget_->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_->setSelectionMode(QAbstractItemView::NoSelection);

    // Панель элементов управления
    controlPanelLayout_ = new QVBoxLayout();

    keyEdit_ = new QLineEdit(this);
    keyEdit_->setPlaceholderText("Ключ (uint)");

    valueEdit_ = new QLineEdit(this);
    valueEdit_->setPlaceholderText("Значение (string)");

    QPushButton* insertBtn = new QPushButton("Добавить / Обновить", this);
    QPushButton* removeBtn = new QPushButton("Удалить по ключу", this);
    QPushButton* findBtn = new QPushButton("Найти по ключу", this);

    resizeEdit_ = new QLineEdit(this);
    resizeEdit_->setPlaceholderText("Новый размер (N)");
    QPushButton* resizeBtn = new QPushButton("Изменить размер", this);

    hashFuncComboBox_ = new QComboBox(this);
    hashFuncComboBox_->addItem("Золотое сечение (Функция 2)");
    hashFuncComboBox_->addItem("Полиномиальная c=1, d=5 (Функция 1)");

    statusLabel_ = new QLabel("Готов к работе", this);
    statusLabel_->setWordWrap(true);

    // Размещение на панели
    controlPanelLayout_->addWidget(new QLabel("<b>Управление ключами:</b>", this));
    controlPanelLayout_->addWidget(keyEdit_);
    controlPanelLayout_->addWidget(valueEdit_);
    controlPanelLayout_->addWidget(insertBtn);
    controlPanelLayout_->addWidget(removeBtn);
    controlPanelLayout_->addWidget(findBtn);
    controlPanelLayout_->addSpacing(15);

    controlPanelLayout_->addWidget(new QLabel("<b>Размер хеш-таблицы:</b>", this));
    controlPanelLayout_->addWidget(resizeEdit_);
    controlPanelLayout_->addWidget(resizeBtn);
    controlPanelLayout_->addSpacing(15);

    controlPanelLayout_->addWidget(new QLabel("<b>Выбор хеш-функции:</b>", this));
    controlPanelLayout_->addWidget(hashFuncComboBox_);
    controlPanelLayout_->addSpacing(20);

    controlPanelLayout_->addWidget(statusLabel_);
    controlPanelLayout_->addStretch();

    mainLayout_->addWidget(tableWidget_, 3);
    mainLayout_->addLayout(controlPanelLayout_, 1);

    setCentralWidget(containter_);

    // Сигналы и слоты
    connect(insertBtn, &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(findBtn, &QPushButton::clicked, this, &MainWindow::onFindClicked);
    connect(resizeBtn, &QPushButton::clicked, this, &MainWindow::onResizeClicked);
    connect(hashFuncComboBox_, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onHashFunctionChanged);

    updateTableGui();
}

MainWindow::~MainWindow()
{
    delete testTable_;
}

void MainWindow::updateTableGui()
{
    testTable_->showInGui(tableWidget_);
}

void MainWindow::onInsertClicked()
{
    bool ok = false;
    uint key = keyEdit_->text().toUInt(&ok);
    if (!ok)
    {
        statusLabel_->setText("Ошибка: введите неотрицательное число в поле ключа.");
        return;
    }

    std::string val = valueEdit_->text().toStdString();
    testTable_->insert(key, val);
    updateTableGui();
    statusLabel_->setText(QString("Вставлен ключ: %1").arg(key));
}

void MainWindow::onRemoveClicked()
{
    bool ok = false;
    uint key = keyEdit_->text().toUInt(&ok);
    if (!ok)
    {
        statusLabel_->setText("Ошибка: некорректный ключ.");
        return;
    }

    if (testTable_->remove(key))
    {
        statusLabel_->setText(QString("Ключ %1 удален.").arg(key));
    }
    else
    {
        statusLabel_->setText(QString("Ключ %1 не найден.").arg(key));
    }
    updateTableGui();
}

void MainWindow::onFindClicked()
{
    bool ok = false;
    uint key = keyEdit_->text().toUInt(&ok);
    if (!ok)
    {
        statusLabel_->setText("Ошибка: некорректный ключ.");
        return;
    }

    if (testTable_->contains(key))
    {
        statusLabel_->setText(QString("Ключ %1 НАЙДЕН. Значение: \"%2\"")
            .arg(key)
            .arg(QString::fromStdString((*testTable_)[key])));
    }
    else
    {
        statusLabel_->setText(QString("Ключ %1 НЕ найден.").arg(key));
    }
}

void MainWindow::onResizeClicked()
{
    bool ok = false;
    uint newSize = resizeEdit_->text().toUInt(&ok);
    if (!ok || newSize == 0)
    {
        statusLabel_->setText("Ошибка: размер должен быть > 0.");
        return;
    }

    testTable_->resize(newSize);
    updateTableGui();
    statusLabel_->setText(QString("Размер изменен на %1 ячеек. Таблица перехеширована.").arg(newSize));
}

void MainWindow::onHashFunctionChanged(int index)
{
    if (index == 0)
    {
        testTable_->setHashFunction(new GoldenRatioHashFunction());
        statusLabel_->setText("Установлена функция 2 (Золотое сечение). Таблица перехеширована.");
    }
    else if (index == 1)
    {
        testTable_->setHashFunction(new QuadraticHashFunction());
        statusLabel_->setText("Установлена функция 1 (Параметры c=1, d=5). Таблица перехеширована.");
    }
    updateTableGui();
}
#include "QtWidgetsApplication1.h"
#include "ui_QtWidgetsApplication1.h"

void QtWidgetsApplication1::updateTable() {
    ui->crosswordTable->clear();
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem("");
            ui->crosswordTable->setItem(j, i, item);
        }
        ui->crosswordTable->setColumnWidth(i, 20);
        ui->crosswordTable->setRowHeight(i, 20);
    }
    ui->crosswordTable->verticalHeader()->setMinimumSectionSize(20);
    ui->crosswordTable->horizontalHeader()->setMinimumSectionSize(20);
    ui->crosswordTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    ui->crosswordTable->horizontalHeader()->hide();
    ui->crosswordTable->verticalHeader()->hide();
    ui->crosswordTable->show();
}

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::QtWidgetsApplication1)
{
    ui->setupUi(this);

    ui->crosswordTable->setItemDelegate(new SingleLetterDelegate(ui->crosswordTable));
    ui->crosswordTable->setColumnCount(40);
    ui->crosswordTable->setRowCount(40);
    ui->crosswordTable->hide();
    ui->checkButton->hide();
    ui->clearCrosswordButton->hide();
    ui->editCrossword->hide();

    // Скрываем элементы для добавления выражений
    ui->finishCreationButton->hide();
    
    ui->exps->hide();
    ui->exps->setEnabled(false);

    // Подключение сигналов к слотам
    connect(ui->createNewCrosswordButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onCreateNewCrossword);
    connect(ui->crosswordTable, &QTableWidget::cellClicked, this, &QtWidgetsApplication1::onCellClicked);
    connect(ui->finishCreationButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onFinishCreation);
    connect(ui->clearCrosswordButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onClearCrossword);
    connect(ui->editCrossword, &QPushButton::clicked, this, &QtWidgetsApplication1::onEditCrossword);
    connect(ui->checkButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onCheckButton);
    connect(ui->startButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onStartButton);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    delete ui;
}

void QtWidgetsApplication1::onStartButton()
{
    ui->startButton->hide();
    ui->startButton->setDisabled(true);
    updateTable();
    ui->crosswordTable->setEnabled(true);
    ui->checkButton->show();
    ui->clearCrosswordButton->show();
    ui->editCrossword->show();
    ui->exps->show();
    ui->createNewCrosswordButton->setGeometry(1180, 320, 170, 50);
}

void QtWidgetsApplication1::onCheckButton()
{
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (!correctAnswers[qMakePair(j, i)].isNull() && correctAnswers[qMakePair(j, i)].at(0).isDigit()) {
                if (!correctAnswers[qMakePair(j + 1, i)].isNull()) {
                    int k = j + 1;
                    int length_correct = 0;
                    int length_entered = 0;
                    while (!correctAnswers[qMakePair(k, i)].isNull()) {
                        length_correct++;
                        if (ui->crosswordTable->item(k, i)->text() == correctAnswers[qMakePair(k, i)]) {
                            length_entered++;
                        }
                        k++;
                    }
                    if (length_correct > 0) {
                        if (length_correct == length_entered) {
                            for (int k = j + 1; k < j + 1 + length_correct; ++k) {
                                ui->crosswordTable->item(k, i)->setBackground(Qt::green);
                            }
                        }
                        else {
                            for (int k = j + 1; k < j + 1 + length_correct; ++k) {
                                ui->crosswordTable->item(k, i)->setBackground(Qt::red);
                            }
                        }
                    }
                }
                else if (!correctAnswers[qMakePair(j, i + 1)].isNull()) {
                    int k = i + 1;
                    int length_correct = 0;
                    int length_entered = 0;
                    while (!correctAnswers[qMakePair(j, k)].isNull()) {
                        length_correct++;
                        if (ui->crosswordTable->item(j, k)->text() == correctAnswers[qMakePair(j, k)]) {
                            length_entered++;
                        }
                        k++;
                    }
                    if (length_correct > 0) {
                        if (length_correct == length_entered) {
                            for (int k = i + 1; k < i + 1 + length_correct; ++k) {
                                ui->crosswordTable->item(j, k)->setBackground(Qt::green);
                            }
                        }
                        else {
                            for (int k = i + 1; k < i + 1 + length_correct; ++k) {
                                ui->crosswordTable->item(j, k)->setBackground(Qt::red);
                            }
                        }
                    }
                }
            }
        }
    }
}

void QtWidgetsApplication1::onEditCrossword()
{
    ui->checkButton->hide();
    ui->finishCreationButton->show();
    ui->exps->show();
    ui->exps->setEnabled(true);
    ui->crosswordTable->setShowGrid(true);
    updateTable();
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (correctAnswers[qMakePair(j, i)].isNull() == false) {
                ui->crosswordTable->item(j, i)->setText(correctAnswers[qMakePair(j, i)]);
            }
        }
    }
}

void QtWidgetsApplication1::onCreateNewCrossword()
{
    ui->checkButton->hide();
    ui->finishCreationButton->show();
    ui->exps->clear();
    ui->exps->setPlainText(QStringLiteral("Выражения: "));
    ui->exps->show();
    ui->exps->setEnabled(true);
    ui->crosswordTable->setShowGrid(true);
    ui->clearCrosswordButton->show();
    ui->startButton->hide();
    ui->startButton->setDisabled(true);
    updateTable();
    ui->crosswordTable->setEnabled(true);
    correctAnswers.clear();
}

void QtWidgetsApplication1::onCellClicked(int row, int column)
{
    ui->crosswordTable->editItem(ui->crosswordTable->item(row, column));
}

void QtWidgetsApplication1::onFinishCreation()
{
    // Путь к изображению
    QString imagePath = "tableItem.png";
    QPixmap pixmap(imagePath);
    QBrush brush(pixmap);
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (ui->crosswordTable->item(j, i)->text() == "") 
            {
                ui->crosswordTable->item(j, i)->setBackground(Qt::transparent);
                ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
            }
            else if (ui->crosswordTable->item(j, i)->text().at(0).isLetter()) 
            {
                correctAnswers[qMakePair(j, i)] = ui->crosswordTable->item(j, i)->text();
                ui->crosswordTable->item(j, i)->setText("");
                ui->crosswordTable->item(j, i)->setBackground(brush);
            }
            else 
            {
                correctAnswers[qMakePair(j, i)] = ui->crosswordTable->item(j, i)->text();
                ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
            }
        }
    }
    ui->crosswordTable->setShowGrid(false);
    ui->checkButton->show();
    ui->finishCreationButton->hide();
    ui->exps->setEnabled(false);
    ui->checkButton->show();
    ui->clearCrosswordButton->show();
    ui->editCrossword->show();
    ui->createNewCrosswordButton->setGeometry(1180, 320, 170, 50);
}

void QtWidgetsApplication1::onClearCrossword()
{
    QString imagePath = "tableItem.png";
    QPixmap pixmap(imagePath);
    QBrush brush(pixmap);
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (ui->crosswordTable->item(j, i)->flags() != 0) {
                ui->crosswordTable->item(j, i)->setText("");
                ui->crosswordTable->item(j, i)->setBackground(brush);
            }
        }
    }
}

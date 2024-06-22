#include "QtWidgetsApplication1.h"
#include "ui_QtWidgetsApplication1.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadCrosswordConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadCrosswordConfig()
{
    // Example configuration for a crossword
    // Load your actual crossword configuration here
    ui->crosswordTable->setRowCount(5);
    ui->crosswordTable->setColumnCount(5);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem();
            ui->crosswordTable->setItem(i, j, item);
        }
    }
    ui->confirmButton->setEnabled(false);
    ui->confirmButton->setVisible(false);
    ui->addExpButton->setEnabled(false);
    ui->addExpButton->setVisible(false);
    ui->createButton->setEnabled(true);
    ui->createButton->setVisible(true);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
    connect(ui->createButton, &QPushButton::clicked, this, &MainWindow::on_createButton_clicked);
}

void MainWindow::loadCrosswordConfigToCreate() 
{
    ui->crosswordTable->setRowCount(40);
    ui->crosswordTable->setColumnCount(40);

    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem();
            ui->crosswordTable->setItem(i, j, item);
        }
        ui->crosswordTable->setColumnWidth(i, 10);
    }
    ui->confirmButton->setEnabled(true);
    ui->confirmButton->setVisible(true);
    ui->addExpButton->setEnabled(true);
    ui->addExpButton->setVisible(true);
    ui->createButton->setEnabled(false);
    ui->createButton->setVisible(false);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
    connect(ui->confirmButton, &QPushButton::clicked, this, &MainWindow::on_confirmButton_clicked);
}

void MainWindow::on_createButton_clicked() 
{
    loadCrosswordConfigToCreate();
}

void MainWindow::on_confirmButton_clicked()
{
    loadCrosswordConfig();
}

void MainWindow::on_clearButton_clicked()
{
    for (int i = 0; i < ui->crosswordTable->rowCount(); ++i) {
        for (int j = 0; j < ui->crosswordTable->columnCount(); ++j) {
            QTableWidgetItem* item = ui->crosswordTable->item(i, j);
            if (item) {
                item->setText("");
                item->setBackground(Qt::white);
            }
        }
    }
}

void MainWindow::checkCrossword()
{
    for (int i = 0; i < ui->crosswordTable->rowCount(); ++i) {
        for (int j = 0; j < ui->crosswordTable->columnCount(); ++j) {
            QTableWidgetItem* item = ui->crosswordTable->item(i, j);
            if (item) {
                QLineEdit* regexEdit = qobject_cast<QLineEdit*>(ui->crosswordTable->cellWidget(5, j));
                if (regexEdit) {
                    QString regexStr = regexEdit->text();
                    QRegularExpression regex(regexStr);
                    if (checkWord(item->text(), regex)) {
                        item->setBackground(Qt::green);
                    }
                    else {
                        item->setBackground(Qt::red);
                    }
                }
            }
        }
    }
}

bool MainWindow::checkWord(const QString& word, const QRegularExpression& regex)
{
    QRegularExpressionMatch match = regex.match(word);
    return match.hasMatch();
}

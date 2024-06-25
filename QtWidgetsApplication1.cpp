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
    name_of_save = nullptr;
    textLabel = new QLabel(this);
    pixmap = new QPixmap(imagePath);
    brush = new QBrush(*pixmap);
    ui->setupUi(this);
    ui->graphicsView->hide();

    ui->crosswordTable->setItemDelegate(new SingleLetterDelegate(ui->crosswordTable));
    ui->crosswordTable->setColumnCount(40);
    ui->crosswordTable->setRowCount(40);
    ui->crosswordTable->hide();
    ui->clearCrosswordButton->hide();
    ui->editCrossword->hide();

    ui->finishCreationButton->hide();
    ui->restartButton->hide();
    ui->exps->hide();
    ui->exps->setEnabled(false);
    ui->saveButton->hide();

    QDir().mkpath(SAVES_DIR);

    // Подключение сигналов к слотам
    
    connect(ui->loadButton, &QPushButton::clicked, this, &QtWidgetsApplication1::loadCrossword);
    //connect(ui->crosswordTable, &QTableWidget::itemChanged, this, &QtWidgetsApplication1::onCheck);
    connect(ui->crosswordTable, &QTableWidget::itemClicked, this, &QtWidgetsApplication1::onCheck);
    connect(ui->createNewCrosswordButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onCreateNewCrossword);
    connect(ui->crosswordTable, &QTableWidget::cellClicked, this, &QtWidgetsApplication1::onCellClicked);
    connect(ui->finishCreationButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onFinishCreation);
    connect(ui->clearCrosswordButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onClearCrossword);
    connect(ui->editCrossword, &QPushButton::clicked, this, &QtWidgetsApplication1::onEditCrossword);
    connect(ui->startButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onStartButton);
    connect(ui->saveButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onSaveButton);
    connect(ui->restartButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onRestartButton);
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
    ui->clearCrosswordButton->show();
    ui->editCrossword->show();
    ui->exps->show();
    ui->createNewCrosswordButton->setGeometry(1180, 320, 170, 50);
}

void QtWidgetsApplication1::onSaveButton()
{
    
    textLabel->clear();
    textLabel->setText(QStringLiteral("Напишите имя сохранения (на английском): "));
    textLabel->setGeometry(1180, 660, 200, 30);
    textLabel->show();
    name_of_save = new QLineEdit(this);
    name_of_save->setGeometry(1180, 700, 200, 20);
    name_of_save->setEnabled(true);
    name_of_save->setReadOnly(false);
    // Установить валидатор для QLineEdit
    QRegExp rx("[a-zA-Z0-9]+"); // Разрешены только английские буквы и цифры
    QRegExpValidator* validator = new QRegExpValidator(rx, this);
    name_of_save->setValidator(validator);
    name_of_save->show();

    connect(name_of_save, &QLineEdit::editingFinished, this, &QtWidgetsApplication1::saveToFile);
}

/// <summary>
/// //////////////////////////////////////////////////// не всегда очищаются цвета
/// </summary>

void QtWidgetsApplication1::saveToFile()
{
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (ui->crosswordTable->item(j, i)->text() != "")
            {
                correctAnswers[qMakePair(j, i)] = ui->crosswordTable->item(j, i)->text();
            }
        }
    }
    QString name = name_of_save->text();
    QString fileName = SAVES_DIR + "/" + name + ".txt";
    name_of_save->clear();
    name_of_save->setDisabled(true);
    name_of_save->hide();
    textLabel->hide();

    QFile file(fileName);

    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Имя файла не может быть пустым"));
        return;
    }

    if (file.exists()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QStringLiteral("Файл с таким именем уже сущетсвует"),
            QStringLiteral("Перезаписать его?"),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream f(&file);
    QMap<QPair<int, int>, QString>::iterator it;
    for (it = correctAnswers.begin(); it != correctAnswers.end(); ++it) {
        if (it.value() != "") {
            f << it.key().first << " " << it.key().second << " " << it.value() << "\n";
        }
    }
    f << "Expressions:\n";
    f << ui->exps->toPlainText();
    file.close();
    QMessageBox::information(this, QStringLiteral("Сохранено"), QStringLiteral("Файл с именем ") + name + QStringLiteral(" успешно сохранен."));
}

void QtWidgetsApplication1::loadCrossword()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Загрузите кроссворд"), SAVES_DIR, QStringLiteral("Файлы: (*.txt)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, QStringLiteral("Ошибка"), QStringLiteral("Ошибка при загрузке кроссворда"));
        return;
    }
    correctAnswers.clear();
    updateTable();
    ui->crosswordTable->setEnabled(true);
    QString content = file.readAll();
    file.close();

    QStringList words = content.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QPixmap pixmap(imagePath);
    QBrush brush(pixmap);
    QString exps_t;
    int k = 0;
    for (; k < words.size();) 
    {
        if (words[k] == "Expressions:") break;
        int j = words[k++].toInt();
        int i = words[k++].toInt();
        QString t = words[k++];
        correctAnswers[qMakePair(j, i)] = t;
        bool is_number = true;
        for (int z = 0; z < t.size(); z++) 
        {
            if (t[z] < '0' || t[z]>'9') is_number = false;
        }
        if (is_number)
        {
            ui->crosswordTable->item(j, i)->setBackground(Qt::transparent);
            ui->crosswordTable->item(j, i)->setText(t);
            ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
        }
        else
        {
            ui->crosswordTable->item(j, i)->setBackground(brush);
            ui->crosswordTable->item(j, i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        }
    }
    for (int i = 0; i < 40; ++i) 
    {
        for (int j = 0; j < 40; ++j)
        { 
            if (ui->crosswordTable->item(j, i)->flags() == (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable)) continue;
            ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
        }
    }
    for (; k < words.size(); ++k)
    {
        exps_t += words[k] + " ";
    }
    ui->startButton->hide();
    ui->exps->setPlainText(exps_t);
    ui->crosswordTable->setShowGrid(false);
    ui->finishCreationButton->hide();
    ui->exps->setEnabled(false);
    ui->exps->show();
    ui->clearCrosswordButton->show();
    ui->editCrossword->show();
    ui->createNewCrosswordButton->setGeometry(1180, 320, 170, 50);

    file.close();
}


void QtWidgetsApplication1::onRestartButton()
{
    ui->graphicsView->clearMask();
    ui->graphicsView->hide();
    ui->restartButton->hide();
    onClearCrossword();
}


void QtWidgetsApplication1::onCheck()
{
    int correct_words = 0;
    int entered_words = 0;
    bool is_cleared = false;
    QMap<QPair<int, int>, QString>::iterator it;
    int i = 0;
    int j = 0;
    int k = 0;
    int length_correct = 0;
    int length_entered = 0;
    for (it = correctAnswers.begin(); it != correctAnswers.end(); ++it) {
        if (it.value() != "") {
            j = it.key().first;
            i = it.key().second;
            is_cleared = false;
            if (it.value().at(0).isDigit()) {
                if (!correctAnswers[qMakePair(j + 1, i)].isNull()) {
                    k = j + 1;
                    correct_words++;
                    length_correct = 0;
                    length_entered = 0;
                    while (!correctAnswers[qMakePair(k, i)].isNull()) {
                        length_correct++;
                        if (ui->crosswordTable->item(k, i)->text() == "") {
                            is_cleared = true;
                        }
                        if (ui->crosswordTable->item(k, i)->text() == correctAnswers[qMakePair(k, i)]) {
                            length_entered++;
                        }
                        k++;
                    }
                    if (is_cleared) {
                        for (k = j + 1; k < j + 1 + length_correct; ++k) {
                            if (ui->crosswordTable->item(k, i)->background() != *brush) {
                                ui->crosswordTable->item(k, i)->setBackground(*brush);
                            }
                        }
                    }
                    else if (length_correct > 0) {
                        if (length_correct == length_entered) {
                            entered_words++;
                            for (k = j + 1; k < j + 1 + length_correct; ++k) {
                                if (ui->crosswordTable->item(k, i)->backgroundColor() == Qt::green) {
                                    continue;
                                }
                                ui->crosswordTable->item(k, i)->setBackground(Qt::green);
                            }
                        }
                        else {
                            for (k = j + 1; k < j + 1 + length_correct; ++k) {
                                if (ui->crosswordTable->item(k, i)->backgroundColor() == Qt::red) {
                                    continue;
                                }
                                ui->crosswordTable->item(k, i)->setBackground(Qt::red);
                            }
                        }
                    }

                }
                else if (!correctAnswers[qMakePair(j, i + 1)].isNull()) {
                    k = i + 1;
                    length_correct = 0;
                    length_entered = 0;
                    correct_words++;
                    while (!correctAnswers[qMakePair(j, k)].isNull()) {
                        length_correct++;
                        if (ui->crosswordTable->item(j, k)->text() == "") {
                            is_cleared = true;
                        }
                        if (ui->crosswordTable->item(j, k)->text() == correctAnswers[qMakePair(j, k)]) {
                            length_entered++;
                        }
                        k++;
                    }
                    if (is_cleared) {
                        for (k = i + 1; k < i + 1 + length_correct; ++k) {
                            ui->crosswordTable->item(j, k)->setBackground(*brush);
                        }
                    }
                    else if (length_correct > 0) {
                        if (length_correct == length_entered) {
                            entered_words++;
                            for (k = i + 1; k < i + 1 + length_correct; ++k) {
                                if (ui->crosswordTable->item(j, k)->backgroundColor() == Qt::green) {
                                    continue;
                                }
                                ui->crosswordTable->item(j, k)->setBackground(Qt::green);
                            }
                        }
                        else {
                            for (k = i + 1; k < i + 1 + length_correct; ++k) {
                                if (ui->crosswordTable->item(j, k)->backgroundColor() == Qt::red) {
                                    continue;
                                }
                                ui->crosswordTable->item(j, k)->setBackground(Qt::red);
                            }
                        }
                    }
                }
            }
       }
    }
    
    if (correct_words > 0 && correct_words == entered_words) {
        showWinMessage();
    }
    else {
        ui->graphicsView->hide();
    }
}

void QtWidgetsApplication1::onEditCrossword()
{
    ui->saveButton->show();
    ui->finishCreationButton->show();
    ui->exps->show();
    ui->exps->setEnabled(true);
    ui->crosswordTable->setShowGrid(true);
    onClearCrossword();
    ui->crosswordTable->setEnabled(true);
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (!correctAnswers[qMakePair(j, i)].isNull()) {
                ui->crosswordTable->item(j, i)->setText(correctAnswers[qMakePair(j, i)]);
                ui->crosswordTable->item(j, i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
                ui->crosswordTable->item(j, i)->setBackground(Qt::transparent);
            }
        }
    }
}

void QtWidgetsApplication1::onCreateNewCrossword()
{
    ui->saveButton->show();
    ui->finishCreationButton->show();
    ui->exps->clear();
    ui->exps->setPlainText(QStringLiteral("Выражения: "));
    ui->exps->show();
    ui->exps->setEnabled(true);
    ui->crosswordTable->setShowGrid(true);
    ui->clearCrosswordButton->show();
    ui->startButton->hide();
    ui->startButton->setDisabled(true);
    correctAnswers.clear();
    updateTable();
    ui->crosswordTable->setEnabled(true);
}

void QtWidgetsApplication1::onCellClicked(int row, int column)
{
    ui->crosswordTable->editItem(ui->crosswordTable->item(row, column));
}

void QtWidgetsApplication1::onFinishCreation()
{
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
                ui->crosswordTable->item(j, i)->setBackground(*brush);
            }
            else 
            {
                correctAnswers[qMakePair(j, i)] = ui->crosswordTable->item(j, i)->text();
                ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
            }
        }
    }
    ui->crosswordTable->setShowGrid(false);
    ui->finishCreationButton->hide();
    ui->exps->setEnabled(false);
    ui->clearCrosswordButton->show();
    ui->editCrossword->show();
    ui->createNewCrosswordButton->setGeometry(1180, 320, 170, 50);
}

void QtWidgetsApplication1::onClearCrossword()
{
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (ui->crosswordTable->item(j, i)->flags() != 0) {
                ui->crosswordTable->item(j, i)->setText("");
                ui->crosswordTable->item(j, i)->setBackground(*brush);
            }
        }
    }
    ui->graphicsView->hide();
}

void QtWidgetsApplication1::showWinMessage()
{
    ui->restartButton->show();
    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
    ui->graphicsView->setAttribute(Qt::WA_TranslucentBackground);
    ui->graphicsView->setStyleSheet("background: transparent; border: none;");
    // Установка прозрачного фона для QGraphicsScene
    scene->setBackgroundBrush(Qt::transparent);
    QGraphicsTextItem* winMessage;
    winMessage = scene->addText(QStringLiteral("Вы выиграли!"));
    winMessage->setDefaultTextColor(Qt::blue);
    winMessage->setFont(QFont("Arial", 20));
    winMessage->setPos(50, 50);
}

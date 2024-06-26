#include "QtWidgetsApplication1.h"
#include "ui_QtWidgetsApplication1.h"

// ���������� ��������� ���������
//bool operator==(QPair<int, int> p1, QPair<int, int> p2)
//{
//    return (p1.first == p2.first && p1.second == p2.second);
//}

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

    QRegularExpression regex(QStringLiteral("[�-��-���]+|\\d{0,2}"));
    delegate = new LetterOrNumberDelegate(regex, this);
    mode = 0;
    ui->crosswordTable->setItemDelegate(delegate);
    ui->crosswordTable->setColumnCount(40);
    ui->crosswordTable->setRowCount(40);
    ui->crosswordTable->hide();
    ui->clearCrosswordButton->hide();
    ui->editCrossword->hide();

    ui->finishCreationButton->hide();
    ui->menuButton->hide();
    ui->exps->hide();
    ui->exps->setEnabled(false);
    ui->saveButton->hide();

    ui->classicModeBox->setChecked(true);
    ui->classicModeBox->setEnabled(true);
    ui->classicModeBox->show();
    ui->regexModeBox->setChecked(false);
    ui->regexModeBox->setEnabled(true);
    ui->regexModeBox->show();
    

    QDir().mkpath(CLASSIC_SAVES_DIR);

    // ����������� �������� � ������
    connect(ui->classicModeBox, &QCheckBox::stateChanged, this, &QtWidgetsApplication1::onClassicMode);
    connect(ui->regexModeBox, &QCheckBox::stateChanged, this, &QtWidgetsApplication1::onRegexMode);
    connect(ui->loadButton, &QPushButton::clicked, this, &QtWidgetsApplication1::loadCrossword);
    connect(ui->crosswordTable, &QTableWidget::itemClicked, this, &QtWidgetsApplication1::onCheck);
    connect(ui->createNewCrosswordButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onCreateNewCrossword);
    connect(ui->crosswordTable, &QTableWidget::cellClicked, this, &QtWidgetsApplication1::onCellClicked);
    connect(ui->finishCreationButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onFinishCreation);
    connect(ui->clearCrosswordButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onClearCrossword);
    connect(ui->editCrossword, &QPushButton::clicked, this, &QtWidgetsApplication1::onEditCrossword);
    connect(ui->startButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onStartButton);
    connect(ui->saveButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onSaveButton);
    connect(ui->menuButton, &QPushButton::clicked, this, &QtWidgetsApplication1::onMenuButton);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    delete ui;
}

// ���� ������ ������������ �����
void QtWidgetsApplication1::onClassicMode(int state)
{
    if (state == Qt::Checked) {
        ui->regexModeBox->setChecked(false);
    }
    else {
        ui->regexModeBox->setChecked(true);
    }
    QDir().mkpath(CLASSIC_SAVES_DIR);
    mode = 0;
    QRegularExpression regex(QStringLiteral("[�-��-���]+|\\d{0,2}"));
    delegate = new LetterOrNumberDelegate(regex, this);
    ui->crosswordTable->setItemDelegate(delegate);
}

// ���� ������ ����� � ����������� �����������
void QtWidgetsApplication1::onRegexMode(int state)
{
    if (state == Qt::Checked) {
        ui->classicModeBox->setChecked(false);
    }
    else {
        ui->classicModeBox->setChecked(true);
    }
    QDir().mkpath(REGEX_SAVES_DIR);
    mode = 1;
    QRegularExpression regex(QStringLiteral(".*"));
    delegate = new LetterOrNumberDelegate(regex, this);
    ui->crosswordTable->setItemDelegate(delegate);
}

// ������ �����
void QtWidgetsApplication1::onStartButton()
{
    ui->menuButton->show();
    ui->classicModeBox->hide();
    ui->regexModeBox->hide();
    ui->startButton->hide();
    ui->startButton->setDisabled(true);
    updateTable();
    ui->crosswordTable->setEnabled(true);
    ui->clearCrosswordButton->show();
    ui->editCrossword->show();
    ui->exps->show();
    ui->createNewCrosswordButton->setGeometry(1180, 320, 170, 50);
    if (mode == 0)
    {
        QRegularExpression regex(QStringLiteral("[�-��-���]+"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
    }
    else
    {
        QRegularExpression regex(QStringLiteral(".*"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
    }
    loadCrossword();
}

// �������� ������ ���������� � ����
void QtWidgetsApplication1::onSaveButton()
{
    
    textLabel->clear();
    textLabel->setText(QStringLiteral("�������� ��� ���������� (�� ����������): "));
    textLabel->setGeometry(1180, 660, 200, 30);
    textLabel->show();
    name_of_save = new QLineEdit(this);
    name_of_save->setGeometry(1180, 700, 200, 20);
    name_of_save->setEnabled(true);
    name_of_save->setReadOnly(false);
    // ���������� ��������� ��� QLineEdit
    QRegExp rx("[a-zA-Z0-9]+"); // ��������� ������ ���������� ����� � �����
    QRegExpValidator* validator = new QRegExpValidator(rx, this);
    name_of_save->setValidator(validator);
    name_of_save->show();

    connect(name_of_save, &QLineEdit::editingFinished, this, &QtWidgetsApplication1::saveToFile);
}

// ���������� ���������� � ����
void QtWidgetsApplication1::saveToFile()
{
    
    QString name = name_of_save->text();
    QString fileName;
    if (mode == 0) {
        fileName = CLASSIC_SAVES_DIR + "/" + name + ".txt";
    }
    else {
        fileName = REGEX_SAVES_DIR + "/" + name + ".txt";
    }
    name_of_save->clear();
    name_of_save->setDisabled(true);
    name_of_save->hide();
    textLabel->hide();

    QFile file(fileName);

    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("������"), QStringLiteral("��� ����� �� ����� ���� ������"));
        return;
    }

    if (file.exists()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QStringLiteral("���� � ����� ������ ��� ����������"),
            QStringLiteral("������������ ���?"),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream f(&file);
    if (mode == 0)
    {
        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 40; ++j) {
                if (ui->crosswordTable->item(j, i)->text() != "")
                {
                    correctAnswers[qMakePair(j, i)] = ui->crosswordTable->item(j, i)->text();
                }
            }
        }
        QMap<QPair<int, int>, QString>::iterator it;
        for (it = correctAnswers.begin(); it != correctAnswers.end(); ++it) {
            if (it.value() != "") {
                f << it.key().first << " " << it.key().second << " " << it.value() << "\n";
            }
        }
    }
    else
    {
        setRegexAnswers();
        cellsSelected();
        QMap<QPair<int, int>, QRegularExpression>::iterator it;
        int c = 1; // ������� 
        for (it = regexAnswer.begin(); it != regexAnswer.end(); ++it)
        {
            f << c << " " << it.key().first << " " << it.key().second << "\n";
            c++;
        }
        f << "cellsSelected:\n";
        for (int i = 0; i < cellsSelectedVector.size(); ++i)
        {
            f << cellsSelectedVector[i].first << " " << cellsSelectedVector[i].second << "\n";
        }
    }
    f << "Expressions:\n";
    f << ui->exps->toPlainText();
    file.close();
    QMessageBox::information(this, QStringLiteral("���������"), QStringLiteral("���� � ������ ") + name + QStringLiteral(" ������� ��������."));
}


// �������� ���������� �� �����

void QtWidgetsApplication1::loadCrossword()
{
    QString fileName = "";
    if (mode == 0) {
        fileName = QFileDialog::getOpenFileName(this, QStringLiteral("��������� ���������"), CLASSIC_SAVES_DIR, QStringLiteral("�����: (*.txt)"));
    }
    else {
        fileName = QFileDialog::getOpenFileName(this, QStringLiteral("��������� ���������"), REGEX_SAVES_DIR, QStringLiteral("�����: (*.txt)"));
    }

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, QStringLiteral("������"), QStringLiteral("������ ��� �������� ����������"));
        return;
    }

    QString content = file.readAll();
    file.close();

    QStringList words = content.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QPixmap pixmap(imagePath);
    QBrush brush(pixmap);
    QString exps_t;
    int k = 0;
    updateTable();
    ui->crosswordTable->setEnabled(true);
    if (mode == 0)
    {
        k = 0;
        correctAnswers.clear();
        //for (; k < words.size();)
        while(words[k] != "Expressions:")
        {
            //if (words[k] == "Expressions:") break;
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
        k++; // ��������� �� ��������� �����, ��� ��� ������� - �����-��������
        for (; k < words.size(); ++k)
        {
            exps_t += words[k] + " ";
        }
        for (int i = 0; i < 40; ++i)
        {
            for (int j = 0; j < 40; ++j)
            {
                if (ui->crosswordTable->item(j, i)->flags() == (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable)) continue;
                ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
            }
        }
        QRegularExpression regex(QStringLiteral("[�-��-���]+"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
    }
    else
    {
        k = 0;
        cellsSelectedVector.clear();
        regexAnswer.clear();
        int j = 0;
        int i = 0;
        while(words[k] != "cellsSelected:")
        {
            QString t = words[k++];
            j = words[k++].toInt();
            i = words[k++].toInt();
            regexAnswer[qMakePair(j, i)] = QRegularExpression("");
            ui->crosswordTable->item(j, i)->setBackground(Qt::transparent);     //
            ui->crosswordTable->item(j, i)->setText(t);                         // ������������� ����� - ������ ���� � ���������
            ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);          //
        }
        k++; // ��������� �� ��������� �����, ��� ��� ������� - �����-��������
        while (words[k] != "Expressions:")
        {
            j = words[k++].toInt();
            i = words[k++].toInt();
            cellsSelectedVector.push_back(qMakePair(j, i));
            ui->crosswordTable->item(j, i)->setBackground(brush);     // ��������� ������ ��� ����������
            ui->crosswordTable->item(j, i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        }
        k++; // ��������� �� ��������� �����, ��� ��� ������� - �����-��������
        if (words[k].toInt() == 0) k++; // ���� ��������� ����� �� ����� (����� ���������) - ����������
        
        QMap<QPair<int, int>, QRegularExpression>::iterator it_regex;
        
        //for (; k < words.size(); ++k)
        for (it_regex = regexAnswer.begin(); it_regex != regexAnswer.end();)
        {
            if (words[k].toInt() == 0)
            {
                regexAnswer[it_regex.key()] = QRegularExpression(words[k]);
                it_regex++;
            }
            exps_t += words[k] + " ";
            k++;
        }
        for (int i = 0; i < 40; ++i)
        {
            for (int j = 0; j < 40; ++j)
            {
                if (ui->crosswordTable->item(j, i)->flags() == (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable)) continue;
                ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
            }
        }
        QRegularExpression regex(QStringLiteral(".*"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
    }
    
    ui->startButton->hide();
    ui->regexModeBox->hide();
    ui->classicModeBox->hide();
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


void QtWidgetsApplication1::onMenuButton()
{
    ui->graphicsView->clearMask();
    ui->graphicsView->hide();
    ui->crosswordTable->hide();
    ui->clearCrosswordButton->hide();
    ui->editCrossword->hide();

    ui->finishCreationButton->hide();
    ui->menuButton->hide();
    ui->exps->hide();
    ui->exps->setEnabled(false);
    ui->saveButton->hide();
    ui->startButton->show();
    ui->startButton->setEnabled(true);

    ui->classicModeBox->setChecked(true);
    ui->classicModeBox->setEnabled(true);
    ui->classicModeBox->show();
    ui->regexModeBox->setChecked(false);
    ui->regexModeBox->setEnabled(true);
    ui->regexModeBox->show();
    ui->menuButton->hide();
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
    if (mode == 0)
    {
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
    else
    {
        entered_words = 0;
        int num_of_exp = 0;
        QMap<QPair<int, int>, QRegularExpression>::iterator it;
        for (it = regexAnswer.begin(); it != regexAnswer.end(); ++it)
        {
            if (it.value() != static_cast<QRegularExpression>(""))
            {
                QString text;
                j = it.key().first;
                i = it.key().second; // �������������� �������� ������ � ������ - ������� ���������
                is_cleared = false;
                num_of_exp++; // ����� ��������� - ��������� �������� �� ����������� -> ������ ���������� 1 ������ ��������
                if (ui->crosswordTable->item(j, i)->flags() == (Qt::NoItemFlags))
                {
                    if (ui->crosswordTable->item(j + 1, i)->flags() != (Qt::NoItemFlags)) {
                        k = j + 1;
                        length_entered = 0;
                        length_correct = 0;
                        while (ui->crosswordTable->item(k, i)->flags() != (Qt::NoItemFlags)) {
                            if (ui->crosswordTable->item(k, i)->text() == "") {
                                is_cleared = true;
                            }
                            else
                            {
                                length_entered++;
                                text += ui->crosswordTable->item(k, i)->text();
                            }
                            length_correct++;
                            k++;
                        }
                        if (is_cleared) {
                            for (k = j + 1; k < j + 1 + length_correct; ++k) {
                                ui->crosswordTable->item(k, i)->setBackground(*brush);
                            }
                        }
                        else if (length_correct == length_entered && length_correct > 0) {
                            if (isValidText(text, regexAnswer[qMakePair(j, i)]))
                            {
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
                    else if (ui->crosswordTable->item(j, i + 1)->flags() != (Qt::NoItemFlags)) {
                        k = i + 1;
                        length_entered = 0;
                        length_correct = 0;
                        while (ui->crosswordTable->item(j, k)->flags() != (Qt::NoItemFlags)) {
                            if (ui->crosswordTable->item(j, k)->text() == "") {
                                is_cleared = true;
                            }
                            else
                            {
                                length_entered++;
                                text += ui->crosswordTable->item(j, k)->text();
                            }
                            length_correct++;
                            k++;
                        }
                        if (is_cleared) {
                            for (k = i + 1; k < i + 1 + length_correct; ++k) {
                                ui->crosswordTable->item(j, k)->setBackground(*brush);
                            }
                        }
                        else if (length_correct == length_entered && length_correct > 0) {
                            if (isValidText(text, regexAnswer[qMakePair(j, i)]))
                            {
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
        if (entered_words == regexAnswer.size() && entered_words > 0) {
            showWinMessage();
        }
        else {
            ui->graphicsView->hide();
        }
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
    QRegularExpression regex(QStringLiteral("[�-��-���]|\\d{0,2}"));
    delegate = new LetterOrNumberDelegate(regex, this);
    ui->crosswordTable->setItemDelegate(delegate);
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            if (!correctAnswers[qMakePair(j, i)].isNull()) {
                ui->crosswordTable->item(j, i)->setText(correctAnswers[qMakePair(j, i)]);
                ui->crosswordTable->item(j, i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
                ui->crosswordTable->item(j, i)->setBackground(Qt::transparent);
            }
        }
    }
    if (mode == 1)
    {
        // ��������� ������� ������ �����
        regex = QRegularExpression(QStringLiteral("d{0,2}"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
        // ��������� ��������� ���������� �����
        ui->crosswordTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ui->crosswordTable->setSelectionBehavior(QAbstractItemView::SelectItems);

    }
}

void QtWidgetsApplication1::onCreateNewCrossword()
{
    ui->menuButton->show();
    ui->classicModeBox->hide();
    ui->regexModeBox->hide();
    ui->saveButton->show();
    ui->finishCreationButton->show();
    ui->exps->clear();
    ui->exps->setPlainText(QStringLiteral("���������: "));
    ui->exps->show();
    ui->exps->setEnabled(true);
    ui->crosswordTable->setShowGrid(true);
    ui->clearCrosswordButton->show();
    ui->startButton->hide();
    ui->startButton->setDisabled(true);
    correctAnswers.clear();
    updateTable();
    ui->crosswordTable->setEnabled(true);
    QRegularExpression regex(QStringLiteral("[�-��-���]|\\d{0,2}"));
    delegate = new LetterOrNumberDelegate(regex, this);
    ui->crosswordTable->setItemDelegate(delegate);
    if (mode == 1)
    {
        // ��������� ������� ������ �����
        regex = QRegularExpression(QStringLiteral("\\d{0,2}"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
        // ��������� ��������� ���������� �����
        ui->crosswordTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ui->crosswordTable->setSelectionBehavior(QAbstractItemView::SelectItems);

    }
}

void QtWidgetsApplication1::onCellClicked(int row, int column)
{
    ui->crosswordTable->editItem(ui->crosswordTable->item(row, column));
}

void QtWidgetsApplication1::setRegexAnswers()
{
    QString content = ui->exps->toPlainText();
    QStringList words = content.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QString expression = "";
    QRegularExpression t_regex;
    regexAnswer.clear();
    int k = 0;
    for (int i = 0; i < words.size(); ++i)
    {
        expression = "";
        if (words[i].toInt() != 0)
        {
            k = i + 1;
            while (words[k].toInt() == 0)
            {
                expression += words[k];
                k++;
                if (k >= words.size()) break;
            }
            t_regex = QRegularExpression(expression);
            for (int x = 0; x < 40; ++x)
            {
                for (int y = 0; y < 40; ++y)
                {
                    if (ui->crosswordTable->item(y, x)->text().toInt() == regexAnswer.size() + 1)
                    {
                        regexAnswer[qMakePair(y, x)] = t_regex; // � ������� ����������� ���������� ������� ������ � ������� ��������� � ���� ���������
                    }
                }
            }

        }
    }
}

void QtWidgetsApplication1::onFinishCreation()
{
    if (mode == 0)
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
        QRegularExpression regex(QStringLiteral("[�-��-���]"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
    }
    else
    {
        setRegexAnswers();
        cellsSelected();
        std::vector<QPair<int, int>> cells = cellsSelectedVector;
        for (int i = 0; i < 40; ++i)
        {
            for (int j = 0; j < 40; ++j)
            {
                if (contains(cells, qMakePair(j, i)))
                {
                    ui->crosswordTable->item(j, i)->setText("");
                    ui->crosswordTable->item(j, i)->setBackground(*brush);
                }
                else if (ui->crosswordTable->item(j, i)->text().toInt() != 0)
                {
                    ui->crosswordTable->item(j, i)->setBackground(Qt::transparent);
                    ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
                }
                else
                {
                    ui->crosswordTable->item(j, i)->setText("");
                    ui->crosswordTable->item(j, i)->setBackground(Qt::transparent);
                    ui->crosswordTable->item(j, i)->setFlags(Qt::NoItemFlags);
                }
            }
        }

        QRegularExpression regex(QStringLiteral(".*"));
        delegate = new LetterOrNumberDelegate(regex, this);
        ui->crosswordTable->setItemDelegate(delegate);
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
    ui->menuButton->show();
    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
    ui->graphicsView->setAttribute(Qt::WA_TranslucentBackground);
    ui->graphicsView->setStyleSheet("background: transparent; border: none;");
    // ��������� ����������� ���� ��� QGraphicsScene
    scene->setBackgroundBrush(Qt::transparent);
    QGraphicsTextItem* winMessage;
    winMessage = scene->addText(QStringLiteral("�� ��������!"));
    winMessage->setDefaultTextColor(Qt::blue);
    winMessage->setFont(QFont("Arial", 20));
    winMessage->setPos(50, 50);
}

// private �����, ������� ��������� �������� �� ��������� ����� � ����������� ���������
bool QtWidgetsApplication1::isValidText(QString& text, QRegularExpression t_regex) {
    QRegularExpressionValidator t_validator(t_regex);

    int pos = 0;
    auto state = t_validator.validate(text, pos);
    return state == QValidator::Acceptable;
}

bool QtWidgetsApplication1::contains(std::vector<QPair<int, int>> v, QPair<int, int> p)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if (v.at(i) == p) return true;
    }
    return false;
}

void QtWidgetsApplication1::cellsSelected()
{
    std::vector<QPair<int, int>> cells;
    for (int i = 0; i < 40; ++i)
    {
        for (int j = 0; j < 40; ++j)
        {
            if (ui->crosswordTable->item(j, i)->isSelected())
            {
                cells.push_back(qMakePair(j, i));
            }
        }
    }
    cellsSelectedVector.clear();
    cellsSelectedVector = cells;
}

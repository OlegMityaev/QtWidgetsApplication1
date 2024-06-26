#pragma once
#include "LetterOrNumberDelegate.h"
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <qplaintextedit.h>
#include <QBrush>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <fstream>
#include <iostream>
#include <qlabel.h>
#include <qmessagebox.h>
#include <QDir>
#include <QFileDialog>
#include <qtextstream.h>
//#include <vector>
#include <set>

const QString CLASSIC_SAVES_DIR = QDir::currentPath() + "/classic_saves";
const QString REGEX_SAVES_DIR = QDir::currentPath() + "/regex_saves";

namespace Ui { class QtWidgetsApplication1; }

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget* parent = nullptr);
    ~QtWidgetsApplication1();

private slots:
    void onCreateNewCrossword();
    void onEditCrossword();
    void onCellClicked(int row, int column);
    void onFinishCreation();
    void onClearCrossword();
    void onCheck();
    void updateTable();
    void onStartButton();
    void showWinMessage();
    void onSaveButton();
    void saveToFile();
    void onMenuButton();
    void loadCrossword();
    void onClassicMode(int state);
    void onRegexMode(int state);

private:
    bool mode = 0; // 0 - ������������ �����, 1 - regex �����
    bool isValidText(QString& text, QRegularExpression regex); // �������� �� ��������� ����� ��� ���������� ��������� 
    bool contains(std::vector<QPair<int, int>> v, QPair<int, int> p); // ��� ����, ����� ����������, ���� �� ����� ������ � ������� (������������ ��� cellsSelectedVector)
    void cellsSelected(); // ���������� ���������� ������ � cellsSelectedVector;
    void setRegexAnswers(); // ���������� regexAnswer
    LetterOrNumberDelegate* delegate; // ��� ����������� �����
    QString imagePath = "tableItem.png"; // ��� ������ ������
    QPixmap* pixmap;
    QBrush* brush;
    QLineEdit* name_of_save;
    QLabel* textLabel;
    Ui::QtWidgetsApplication1* ui;
    std::vector<QPair<int, int>> cellsSelectedVector; // ������ ���������� ����� ��� regex mode
    QMap<QPair<int, int>, QRegularExpression> regexAnswer; // key - ������ (� ������� ���������), value - ��������� (������������ ������������) (��� regex)
    QMap<QPair<int, int>, QString> correctAnswers; // key - ������, value - ���������� ����� (��� �������)
};


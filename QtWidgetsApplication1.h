#pragma once
#include "SingleLetterDelegate.h"
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

const QString SAVES_DIR = QDir::currentPath() + "/saves";


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
    void onRestartButton();
    void loadCrossword();

private:
    QString imagePath = "tableItem.png";
    QPixmap* pixmap;
    QBrush* brush;
    QLineEdit* name_of_save;
    QLabel* textLabel;
    Ui::QtWidgetsApplication1* ui;
    QMap<QPair<int, int>, QString> correctAnswers; // To store the correct answers
};


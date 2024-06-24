#pragma once
#include "SingleLetterDelegate.h"
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <qplaintextedit.h>
#include <QBrush>
#include <QPixmap>



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
    void onCheckButton();
    void updateTable();

private:
    Ui::QtWidgetsApplication1* ui;
    QMap<QPair<int, int>, QString> correctAnswers; // To store the correct answers
};


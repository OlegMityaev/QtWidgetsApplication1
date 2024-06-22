#pragma once
#define MAINWINDOW_H
#include <QLabel> 
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_createButton_clicked();
    void on_clearButton_clicked();
    void on_confirmButton_clicked();
private:
    Ui::MainWindow* ui;
    void loadCrosswordConfig();
    void loadCrosswordConfigToCreate();
    void checkCrossword();
    bool checkWord(const QString& word, const QRegularExpression& regex);
};


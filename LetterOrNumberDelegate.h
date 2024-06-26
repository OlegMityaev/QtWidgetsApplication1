#pragma once
#include <QTextCodec>
#include <QApplication>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QRegularExpressionValidator>

// ����� �������� ��� ����������� �����
class LetterOrNumberDelegate : public QStyledItemDelegate {
public:
    LetterOrNumberDelegate(const QRegularExpression& regex, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), regex(regex) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setMaxLength(2);  // ������������ ����� ��������� ������

        // ������������� ���������, ��������� ���������� ���������� ���������
        QValidator* validator = new QRegularExpressionValidator(regex, editor);
        editor->setValidator(validator);

        return editor;
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
        lineEdit->setText(value);
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
        QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();

        // ���������, ��� ������� ����� �������� ���������
        if (value.isEmpty()) {
            QTableWidget* table = static_cast<QTableWidget*>(model->parent());
            table->item(index.row(), index.column())->setText("");  // ������������� ������ �������� ��������
        }
        else if ((value.length() == 1 && value.at(0).isLetter()) || (value.length() >= 0 && value.length() <= 2 && value.toInt() > 0 && value.toInt() <= 99)) {
            model->setData(index, value, Qt::EditRole);
        }
    }

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        editor->setGeometry(option.rect);
    }

private:
    QRegularExpression regex;
};


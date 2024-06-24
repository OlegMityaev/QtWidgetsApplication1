#pragma once
#include <QTextCodec>
#include <QApplication>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QRegularExpressionValidator>

// Класс делегата для ограничения ввода
class SingleLetterDelegate : public QStyledItemDelegate {
public:
    SingleLetterDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setMaxLength(2);  // Ограничиваем длину вводимого текста одной буквой

        // Устанавливаем валидатор, чтобы разрешить только буквы
        QRegularExpression regex(QStringLiteral("[А-Яа-яЁё]|\\d{1,2}"));
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

        // Проверяем, что введено ровно одно буквенное или числовое значение
        if ((value.length() == 1 && value.at(0).isLetter()) || (value.length() >= 1 && value.length() <= 2 && value.toInt() >= 0 && value.toInt() <= 99)) {
            model->setData(index, value, Qt::EditRole);
        }
    }

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        editor->setGeometry(option.rect);
    }
};
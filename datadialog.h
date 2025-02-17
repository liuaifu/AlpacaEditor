#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include "alpacadata.h"


class DataDialog : public QDialog {
    Q_OBJECT
public:
    DataDialog(QWidget* parent = nullptr,
             const QString& instruction = "",
             const QString& input = "",
             const QString& output = "");

    AlpacaData getData() const;

private:
    QPlainTextEdit* instructionEdit;
    QLineEdit* inputEdit;
    QPlainTextEdit* outputEdit;
};

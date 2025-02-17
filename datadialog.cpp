#include "datadialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QString>


DataDialog::DataDialog(QWidget* parent,
                     const QString& instruction,
                     const QString& input,
                     const QString& output)
    : QDialog(parent)
{
    setWindowTitle("编辑数据条目");
    setMinimumSize(400, 300);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 指令输入
    QLabel* instructionLabel = new QLabel("指令：");
    instructionEdit = new QPlainTextEdit(instruction);
    instructionEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);  // 自动换行
    instructionEdit->setTabChangesFocus(true);  // Tab键切换焦点
    mainLayout->addWidget(instructionLabel);
    mainLayout->addWidget(instructionEdit);

    // 输入内容
    QLabel* inputLabel = new QLabel("输入：");
    inputEdit = new QLineEdit(input);
    mainLayout->addWidget(inputLabel);
    mainLayout->addWidget(inputEdit);

    // 输出内容
    QLabel* outputLabel = new QLabel("输出：");
    outputEdit = new QPlainTextEdit(output);
    outputEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);  // 自动换行
    outputEdit->setTabChangesFocus(true);  // Tab键切换焦点
    mainLayout->addWidget(outputLabel);
    mainLayout->addWidget(outputEdit);

    // 按钮组
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);

    mainLayout->addWidget(buttonBox);

    // 连接信号
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AlpacaData DataDialog::getData() const {
    return AlpacaData {
        instructionEdit->toPlainText().trimmed(),
        inputEdit->text().trimmed(),
        outputEdit->toPlainText().trimmed()
    };
}

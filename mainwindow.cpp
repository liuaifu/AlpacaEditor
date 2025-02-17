#include "mainwindow.h"
#include "datadialog.h"
#include "alpacadata.h"
#include <QApplication>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFile>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStatusBar>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"指令", "输入", "输出"});
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    setCentralWidget(table);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updatePagination();

    setWindowTitle("Alpaca数据集编辑器 v0.1");
    setMinimumSize(800, 600);
}

void MainWindow::createActions() {
    // 文件菜单动作
    newAct = new QAction("新建(&N)", this);
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction("打开(&O)...", this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction("保存(&S)", this);
    saveAct->setShortcut(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    saveAsAct = new QAction("另存为(&A)...", this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

    exitAct = new QAction("退出(&X)", this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    // 编辑菜单动作
    addAct = new QAction("添加数据(&A)", this);
    connect(addAct, &QAction::triggered, this, &MainWindow::addData);

    editAct = new QAction("编辑数据(&E)", this);
    connect(editAct, &QAction::triggered, this, &MainWindow::editData);

    deleteAct = new QAction("删除数据(&D)", this);
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteData);

    // 帮助菜单动作
    aboutAct = new QAction("关于(&A)", this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    // 分页控制
    prevPageAct = new QAction("上一页", this);
    connect(prevPageAct, &QAction::triggered, this, &MainWindow::prevPage);

    nextPageAct = new QAction("下一页", this);
    connect(nextPageAct, &QAction::triggered, this, &MainWindow::nextPage);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu("文件(&F)");
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu("编辑(&E)");
    editMenu->addAction(addAct);
    editMenu->addAction(editAct);
    editMenu->addAction(deleteAct);

    helpMenu = menuBar()->addMenu("帮助(&H)");
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars() {
    editToolBar = addToolBar("编辑");
    editToolBar->addAction(addAct);
    editToolBar->addAction(editAct);
    editToolBar->addAction(deleteAct);
    editToolBar->addSeparator();
    editToolBar->addAction(prevPageAct);
    editToolBar->addAction(nextPageAct);
}

void MainWindow::createStatusBar() {
    pageLabel = new QLabel(this);
    statusBar()->addPermanentWidget(pageLabel);
}

void MainWindow::updatePagination() {
    int totalPages = qMax(1, (dataset.size() + itemsPerPage - 1) / itemsPerPage);
    pageLabel->setText(QString("第 %1 页/共 %2 页").arg(currentPage).arg(totalPages));

    prevPageAct->setEnabled(currentPage > 1);
    nextPageAct->setEnabled(currentPage < totalPages);
}

// 文件操作实现
void MainWindow::newFile() {
    if (maybeSave()) {
        dataset.clear();
        currentFile.clear();
        updatePagination();
        updateTable();
    }
}

void MainWindow::open() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "Alpaca数据集 (*.json)");
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save() {
    if (currentFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(currentFile);
    }
}

bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "另存为", "", "Alpaca数据集 (*.json)");
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::prevPage() {
    if (currentPage > 1) {
        currentPage--;
        updateTable();
        updatePagination();
    }
}

void MainWindow::nextPage() {
    int totalPages = (dataset.size() + itemsPerPage - 1) / itemsPerPage;
    if (currentPage < totalPages) {
        currentPage++;
        updateTable();
        updatePagination();
    }
}

// 关闭事件处理
void MainWindow::closeEvent(QCloseEvent* event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::maybeSave() {
    if (isWindowModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this, "保存修改",
            "当前修改尚未保存，是否要保存？",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::updateTable() {
    table->setRowCount(0);

    int start = (currentPage - 1) * itemsPerPage;
    int end = qMin(start + itemsPerPage, dataset.size());

    for (int i = start; i < end; ++i) {
        const AlpacaData& data = dataset[i];
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(data.instruction));
        table->setItem(row, 1, new QTableWidgetItem(data.input));
        table->setItem(row, 2, new QTableWidgetItem(data.output));
    }
}

void MainWindow::addData() {
    DataDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        dataset.append(dlg.getData());
        updatePagination();
        updateTable();
    }
}

void MainWindow::editData() {
    int row = table->currentRow();
    if (row == -1) return;

    int actualIndex = (currentPage - 1) * itemsPerPage + row;
    if (actualIndex >= dataset.size()) return;

    AlpacaData data = dataset[actualIndex];
    DataDialog dlg(this, data.instruction, data.input, data.output);
    if (dlg.exec() == QDialog::Accepted) {
        dataset[actualIndex] = dlg.getData();
        updateTable();
    }
}

void MainWindow::deleteData() {
    int row = table->currentRow();
    if (row == -1) return;

    int actualIndex = (currentPage - 1) * itemsPerPage + row;
    if (actualIndex >= dataset.size()) return;

    if (QMessageBox::question(this, "确认删除", "确定要删除这条数据吗？")
        == QMessageBox::Yes) {
        dataset.removeAt(actualIndex);
        updatePagination();
        updateTable();
    }
}

void MainWindow::about() {
    QMessageBox msgbox;
    msgbox.setWindowTitle("关于");
    msgbox.setIcon(QMessageBox::Information);
    msgbox.setText("AlpacaEditor v0.1<br/><br/><a href='http://github.com/liuaifu/AlpacaEditor'>http://github.com/liuaifu/AlpacaEditor</a>");
    msgbox.setTextFormat(Qt::RichText);
    msgbox.exec();
}

void MainWindow::loadFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray array = doc.array();

    dataset.clear();
    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        dataset.append({
            obj["instruction"].toString(),
            obj["input"].toString(),
            obj["output"].toString()
        });
    }

    currentPage = 1;
    updatePagination();
    updateTable();
    currentFile = fileName;
    statusBar()->showMessage("文件已加载", 2000);
}

bool MainWindow::saveFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "错误", "无法保存文件");
        return false;
    }

    QJsonArray array;
    for (const AlpacaData& data : dataset) {
        QJsonObject obj;
        obj["instruction"] = data.instruction;
        obj["input"] = data.input;
        obj["output"] = data.output;
        array.append(obj);
    }

    QJsonDocument doc(array);
    file.write(doc.toJson());
    return true;
}

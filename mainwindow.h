#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QList>
#include <QJsonArray>
#include <QSettings>
#include "alpacadata.h"


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void addData();
    void editData();
    void deleteData();
    void about();
    void prevPage();
    void nextPage();
    void updateTable();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    bool maybeSave();
    void loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    void updatePagination();

    QList<AlpacaData> dataset;
    int currentPage = 1;
    const int itemsPerPage = 50;

    QTableWidget* table;
    QLabel* pageLabel;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;
    QToolBar* editToolBar;
    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;
    QAction* addAct;
    QAction* editAct;
    QAction* deleteAct;
    QAction* prevPageAct;
    QAction* nextPageAct;
    QAction* aboutAct;

    QString currentFile;
    QSettings* settings;
};

#endif // MAINWINDOW_H

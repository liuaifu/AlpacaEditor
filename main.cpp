#include "mainwindow.h"
#include <QRect>
#include <QScreen>
#include <QApplication>


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window;

    // 获取当前屏幕的大小
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // 计算居中位置
    int x = (screenGeometry.width() - window.width()) / 2;
    int y = (screenGeometry.height() - window.height()) / 2;

    window.move(x, y);
    window.showNormal();

    return app.exec();
}

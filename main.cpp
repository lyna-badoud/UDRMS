#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(
        "* { color: black; }"
        "QDialog { background-color: #E3F2FD; }"
        "QInputDialog { background-color: #E3F2FD; }"
        "QLineEdit { background-color: white; border: 1px solid #2196F3; padding: 3px; }"
        "QPushButton { background-color: #2196F3; color: white !important; border-radius: 5px; padding: 5px; min-width: 60px; }"
        "QDialogButtonBox QPushButton { background-color: #2196F3; color: white; }"
        "QLabel { color: black; }"
        );
    MainWindow w;
    w.show();
    return a.exec();
}

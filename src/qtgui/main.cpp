#include <QApplication>
#include <QLocale>
#include "mainwindow.h"

#include "src/Scene.h"


int main(int argc, char**argv) {
    QApplication a(argc, argv);
    QLocale::setDefault(QLocale::C);
    MainWindow w;
    w.show();

    return a.exec();
}

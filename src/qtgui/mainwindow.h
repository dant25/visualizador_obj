#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QThread>
#include <QProgressBar>
#include "glwidget.h"
#include <QProcess>

#include "src/Scene.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;
    Scene *scene;
private slots:
    void on_actionClose_triggered();
    void on_actionOpen_OBJ_File_triggered();
};


#endif // MAINWINDOW_H

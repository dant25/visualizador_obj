#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../importer.h"
#include "../Object.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new Scene();
    ui->glwidget->s = scene;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClose_triggered()
{
    if(scene)
        delete scene;

    close();
}

void MainWindow::on_actionOpen_OBJ_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/jana", tr("OBJ Files (*.obj)"));

    if( fileName.isEmpty() ) return;

    std::string str = fileName.toStdString();
    const char* file = str.c_str();

    ObjImporter importer = ObjImporter(file);

    Object *o = new Object( importer );
    scene->objects.push_back( o );
    scene->adaptBoundingBox();
    ui->glwidget->cameraFit();

}


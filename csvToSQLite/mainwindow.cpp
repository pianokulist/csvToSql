#include "mainwindow.h"
#include "ui_mainwindow.h"

#define CONSTRUCTORS {

// Инициализирует новый экземпляр класса MainWindow
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

#define CONSTRUCTORS_END }



#define DESTRUCTORS {

// Очищает текущий экземпляр класса MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}

#define DESTRUCTORS_END }



#define METHODS {

// Выход из приложения
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

#define METHODS_END }

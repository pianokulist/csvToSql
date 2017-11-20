#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "customtablemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void checkString(QString &temp, QChar character = 0);
    void openFile(const QString& fileName);

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_CSV_triggered();

private:
    Ui::MainWindow *ui;
    QList<QStringList> _csvData;
    CustomTableModel *_customModel;
    QStringList standardItemList;
};

#endif // MAINWINDOW_H

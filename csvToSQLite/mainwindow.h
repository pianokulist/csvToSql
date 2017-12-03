#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardItemModel>
#include <QtSql>
#include "customtablemodel.h"
#include "savetosqldialog.h"

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
    void openCSVFile(const QString& fileName);
    bool saveCSVFile(const QString& fileName);
    void openSql(const QString& fileName);
    void fillFromData(const QAbstractTableModel& model, QSqlDatabase& dataBaseToSave, QString tableName);//(const CustomTableModel& model);
    void fillFromHeader(const QAbstractTableModel& model, QSqlDatabase& dataBaseToSave, QString tableName);//(const CustomTableModel& model);
    QString whatTypeOfAttribute(const QString& str) const;

private slots:
    void on_actionExit_triggered();
    void on_actionOpen_CSV_triggered();
    void on_actionOpen_SQLite_Table_triggered();
    void DataBaseUpdate(QString tableName);
    void UpdateUI(bool openType);
    void on_actionSave_To_CSV_triggered();
    void on_actionSave_To_SQLite_triggered();

private:
    Ui::MainWindow *ui;
    CustomTableModel *_customModel = nullptr;
    QSqlTableModel* _sqlTableModel = nullptr;
    QStringList standardItemList;
    QSqlDatabase db;          // соединение с бд
    QComboBox* comboBox;
    QLabel *textLabel;
    SaveToSqlDialog *saveSqlDialog;
    QStringList columnTypes;

};

#endif // MAINWINDOW_H

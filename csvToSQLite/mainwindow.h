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

//! A Main class.
/*!
    Main class, which contains user interface and open/save functions.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! A constructor.
    /*!
        Initializes a new instanse of MainWindow class.
    */
    explicit MainWindow(QWidget *parent = 0);

    //! A destructor.
    /*!
        Destroy the instanse of MainWindow class.
    */
    ~MainWindow();

    //! A method, that checks string.
    /*!
        A method, that checks string, deletes unnecessary symbols,
        replaces special symbols and add data to the table model.
        \param temp A temp string, that forming by method.
        \param character A symbol, which will be add to string, after check.
    */
    void checkString(QString &temp, QChar character = 0);

    //! A method, that opens a CSV file.
    /*!
        \param fileName A CSV file, which should be opened.
    */
    bool openCSVFile(const QString& fileName);

    //! A method, that saves to CSV file.
    /*!
        \param fileName A CSV file, in which data should be saved.
        \return true - if file saved successfully, otherwise - false.
    */
    bool saveCSVFile(const QString& fileName);

    //! A method, that opens a SQL data base.
    /*!
        \param fileName A SQL data base file, which should be opened.
    */
    bool openSql(const QString& fileName);

    //! A method, that saves data to the SQL data base.
    /*!
        \param model A table model with data.
        \param dataBaseToSave A data base, in which data should be saved.
        \param tableName A table, in which data should be saved.
    */
    void fillFromData(const QAbstractTableModel& model, QSqlDatabase& dataBaseToSave, QString tableName);

    //! A method, that saves table header to the SQL data base.
    /*!
        \param model A table model with data.
        \param dataBaseToSave A data base, in which table header should be saved.
        \param tableName A table, in which table header should be saved.
    */
    void fillFromHeader(const QAbstractTableModel& model, QSqlDatabase& dataBaseToSave, QString tableName);

    //! A method, that determine the attribute type.
    /*!
        \param str A table model with data.
        \return The type of attribute.
    */
    QString whatTypeOfAttribute(const QString& str) const;

private slots:
    //! The triggered event handler of action "Exit".
    void on_actionExit_triggered();
    //! The triggered event handler of action "Open CSV".
    void on_actionOpen_CSV_triggered();
    //! The triggered event handler of action "Open SQLite Table".
    void on_actionOpen_SQLite_Table_triggered();
    //! A method, that opens a new table, chosen in combo box.
    void DataBaseUpdate(QString tableName);
    //! A method, that updates user interface.
    void UpdateUI(bool openType);
    //! The triggered event handler of action "Save To CSV".
    void on_actionSave_To_CSV_triggered();
    //! The triggered event handler of action "Save To SQLite".
    void on_actionSave_To_SQLite_triggered();

    void on_actionRemove_Row_triggered();

    void on_actionMove_To_First_triggered();

    void on_actionAppend_Row_triggered();

    void on_actionCommit_Changes_triggered();

private:
    //! A user interface.
    Ui::MainWindow *ui;
    //! A Table Model
    CustomTableModel *_customModel = nullptr;
    //! A SQL Table Model
    QSqlTableModel* _sqlTableModel = nullptr;
    //! A string list, that used in checkString method.
    /*!
     * \sa checkString(QString &temp, QChar character = 0);
     */
    QStringList standardItemList;
    //! A data base.
    QSqlDatabase db;          // соединение с бд
    //! A Combo box with table names.
    QComboBox* comboBox;
    //! A label of status bar.
    QLabel *textLabel;
    //! A dialog to save SQL data base.
    SaveToSqlDialog *saveSqlDialog;
    //! A column types of SQL data base table.
    QStringList columnTypes;
    //! A symbol which divide the columns in CSV file.
    const char CSV_DIVIDER = ';';
};

#endif // MAINWINDOW_H

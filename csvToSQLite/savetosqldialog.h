#ifndef SAVETOSQLDIALOG_H
#define SAVETOSQLDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class SaveToSqlDialog;
}

//! A save to sql dialog class.
/*!
    Class, which contains UI for choose sql DB to save the open table.
*/
class SaveToSqlDialog : public QDialog
{
    Q_OBJECT

public:
    //! A constructor.
    /*!
        Initializes a new instanse of SaveToSqlDialog class.
    */
    explicit SaveToSqlDialog(QWidget *parent = 0);

    //! A destructor.
    /*!
        Destroy the instanse of SaveToSqlDialog class.
    */
    ~SaveToSqlDialog();

    //! A method, that returns data from dialog.
    /*!
        \param dataBase A database for saving table.
        \return The name of table to save.
    */
    QString GetDataToSave(QSqlDatabase& dataBase);

private slots:
    //! The clicked event handler of pushButton "..."
    void on_pushButton_clicked();

    //! The editTextChanged event handler of table name combo box.
    void on_tableNameComboBox_editTextChanged(const QString &arg1);

private:
    //! A user interface
    Ui::SaveToSqlDialog *ui;
    //! A SQL database for saving table.
    QSqlDatabase db;
};

#endif // SAVETOSQLDIALOG_H

#ifndef SAVETOSQLDIALOG_H
#define SAVETOSQLDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class SaveToSqlDialog;
}

class SaveToSqlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveToSqlDialog(QWidget *parent = 0);
    ~SaveToSqlDialog();
    QString GetDataToSave(QSqlDatabase& dataBase);

private slots:
    void on_pushButton_clicked();

    void on_tableNameComboBox_editTextChanged(const QString &arg1);

private:
    Ui::SaveToSqlDialog *ui;
    QSqlDatabase db;
};

#endif // SAVETOSQLDIALOG_H

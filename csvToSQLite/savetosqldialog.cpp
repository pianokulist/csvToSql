#include "savetosqldialog.h"
#include "ui_savetosqldialog.h"

#include <QFileDialog>

SaveToSqlDialog::SaveToSqlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveToSqlDialog)
{
    ui->setupUi(this);
}

SaveToSqlDialog::~SaveToSqlDialog()
{
    delete ui;
}

void SaveToSqlDialog::on_pushButton_clicked()
{
    QString dbName = QFileDialog::getSaveFileName(this,
                                         tr("Open file"),
                                         QString(), QString(QString::fromLatin1("SQLite files(*.sqlite *.db)")));

    // если выбрали файл
    if (dbName != "")
        ui->pathLineEdit->setText(dbName);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    db.open();

    ui->tableNameComboBox->addItems(db.tables());
}

QString SaveToSqlDialog::GetDataToSave(QSqlDatabase& dataBase)
{
    dataBase = db;
    QString tableName = ui->tableNameComboBox->currentText();

    return tableName;
}


void SaveToSqlDialog::on_tableNameComboBox_editTextChanged(const QString &arg1)
{
    int index = ui->tableNameComboBox->findText(arg1);
    if(index != -1)
    {
        ui->tableNameComboBox->setCurrentIndex(index);
    }
    ui->tableNameComboBox->setItemText(ui->tableNameComboBox->currentIndex(), arg1 );
}

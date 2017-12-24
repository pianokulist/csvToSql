#include "savetosqldialog.h"
#include "ui_savetosqldialog.h"
#include <QFileDialog>
#include <QSqlQuery>
#include <QDebug>


#define CONSTRUCTORS {

// Инициализирует новый экземпляр класса SaveToSqlDialog
SaveToSqlDialog::SaveToSqlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveToSqlDialog)
{
    ui->setupUi(this);
}

#define CONSTRUCTORS_END }



#define DESTRUCTORS {

// Очищает текущий экземпляр класса SaveToSqlDialog
SaveToSqlDialog::~SaveToSqlDialog()
{
    delete ui;
}

#define DESTRUCTORS_END }



#define METHODS {

// Получает имя файла, куда сохранить,
// создает базу данных, если её нет
// открывает - если она есть
void SaveToSqlDialog::on_pushButton_clicked()
{
    QString dbName = QFileDialog::getSaveFileName(this,
                                         tr("Save file"),
                                         QString(), QString(QString::fromLatin1("SQLite files(*.sqlite *.db)")));

    // если выбрали файл
    if (dbName != "")
        ui->pathLineEdit->setText(dbName);

    // открываем базу данных
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    db.open();

    // заносим в comboBox список таблиц
    ui->tableNameComboBox->addItems(db.tables());
}

// Возвращает данные для сохранения: имя таблицы и базу данных по ссылке
QString SaveToSqlDialog::GetDataToSave(QSqlDatabase& dataBase)
{
    // записываем ссылку на базу данных
    dataBase = db;
    // получаем имя таблицы
    QString tableName = ui->tableNameComboBox->currentText();

    // база не пуста, таблица уже есть
    if ((!db.tables().empty()) && db.tables().toSet().contains(tableName))
    {
        // удаляем существующую таблицу
        QSqlQuery query(db);
        bool b = query.exec("DROP TABLE " + tableName + ";");
        qDebug() << b << "\n";
    }

    // возвращаем имя таблицы
    return tableName;
}

// Перезаписывает элементы comboBox, если текст в нем был изменен
void SaveToSqlDialog::on_tableNameComboBox_editTextChanged(const QString &arg1)
{
    int index = ui->tableNameComboBox->findText(arg1);
    if(index != -1)
    {
        ui->tableNameComboBox->setCurrentIndex(index);
    }
    ui->tableNameComboBox->setItemText(ui->tableNameComboBox->currentIndex(), arg1 );
}

#define METHODS_END }

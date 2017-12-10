#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTextStream>


#define CONSTRUCTORS {

// Инициализирует новый экземпляр класса MainWindow
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowIcon(QIcon(QString::fromLatin1(":/resources/icon.png")));
    ui->setupUi(this);

    // создаем combo box и добавляем его в toolBar
    comboBox = new QComboBox();
    connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(DataBaseUpdate(QString)));
    comboBox->setEnabled(false);
    ui->mainToolBar->addWidget(comboBox);

    // создаем label и добавляем его в statusBar
    textLabel = new QLabel();
    ui->statusBar->addWidget(textLabel);
    textLabel->setText("Hello!");
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

// Обновляет интерфейс
void MainWindow::UpdateUI(bool openType)
{
    comboBox->setEnabled(openType);
    comboBox->clear();
    if (_sqlTableModel != nullptr)
        _sqlTableModel->clear();
    if (_customModel != nullptr)
        _customModel->clear();
}

// Выход из приложения
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

#define OPEN {

// Открывает CSV файл
void MainWindow::on_actionOpen_CSV_triggered()
{
    // получаем имя файла из диалога
    QString name = QFileDialog::getOpenFileName(this,
                tr("Open file"),
                QString(), QString(QString::fromLatin1("CSV (*.csv)")));

    // если нет имени файла
    if (name.isEmpty())
    {
        // выходим
        return;
    }
    // обновляем интерфейс
    UpdateUI(false);
    // открываем CSV файл
    openCSVFile(name);

    // выводим информацию в statusBar
    textLabel->setText(
        tr("%1: Data succesfuly loaded").
                arg(QTime::currentTime().toString()));
}

// Открывает CSV файл
void MainWindow::openCSVFile(const QString& fileName)
{
    QFile file{fileName};
    // если не смогли открыть файл
    if (!file.open(QIODevice::ReadOnly))
    {
        // выходим
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // считываем имя колонок
    QString column_names = file.readLine();
    column_names.remove( QRegExp("\r") );
    column_names.remove( QRegExp("\n") );
    QStringList names = column_names.split(";");

    // создаем табличную модель для отображения
    _customModel = new CustomTableModel(this, names);

    // загружаем данные
    QString data = file.readAll();
    data.remove( QRegExp("\r") ); //remove all ocurrences of CR (Carriage Return)
    QString temp;
    QChar character;
    QTextStream textStream(&data);

    // пока не дойдем до конца данных
    while (!textStream.atEnd()) {
        // получаем символ
        textStream >> character;
        if (character == ';') {
            checkString(temp, character);
        } else if (character == '\n') {
            checkString(temp, character);
        } else if (textStream.atEnd()) {
            temp.append(character);
            checkString(temp);
        } else {
            temp.append(character);
        }
    }

    QApplication::restoreOverrideCursor();

    // определяем типы считанных данных
    _customModel->DetermineColumnsTypes();
    // устанавливаем модель в просмотрщик таблиц
    ui->tableView->setModel(_customModel);
}

// Проверяет строку, удаляет лишние символы,
// заменяет спец символы, добавляет обработанные данные в табличную модель
void MainWindow::checkString(QString &temp, QChar character)
{
    if(temp.count("\"")%2 == 0) {
        //if (temp.size() == 0 && character != ',') //problem with line endings
        //    return;
        if (temp.startsWith( QChar('\"')) && temp.endsWith( QChar('\"') ) ) {
             temp.remove( QRegExp("^\"") );
             temp.remove( QRegExp("\"$") );
        }
        //FIXME: will possibly fail if there are 4 or more reapeating double quotes
        temp.replace("\"\"", "\"");

        standardItemList.append(temp);
        if (character != QChar(';')) {
            _customModel->appendRow(standardItemList);
            standardItemList.clear();
        }
        temp.clear();
    } else {
        temp.append(character);
    }
}

// Открывает SQLite базу данных
void MainWindow::on_actionOpen_SQLite_Table_triggered()
{
    QString name = QFileDialog::getOpenFileName(this,
                tr("Open file"),
                QString(), QString(QString::fromLatin1("SQLite files(*.sqlite *.db)")));

    // если нет имени файла
    if (name.isEmpty())
    {
        // выходим
        return;
    }
    // обновляем интерфейс
    UpdateUI(true);
    // открываем SQL базу данных
    openSql(name);

    // выводим информацию в statusBar
    textLabel->setText(
        tr("%1: Data succesfuly loaded").
                arg(QTime::currentTime().toString()));
}

// Открывает SQLite базу данных
void MainWindow::openSql(const QString& fileName)
{
    // открываем базу данных
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    db.open();

    // создаем табличную модель для отображения
    _sqlTableModel = new QSqlTableModel(this);
    // всегда открываем первую таблицу в списке
    _sqlTableModel->setTable(db.tables()[0]);
    _sqlTableModel->select();

    // добавляем список таблиц в comboBox
    comboBox->addItems(db.tables());

    ui->tableView->setModel(_sqlTableModel);
}

// Открывает таблицу выбранную в комбобоксе
void MainWindow::DataBaseUpdate(QString tableName)
{
    _sqlTableModel->setTable(tableName);
    _sqlTableModel->select();
}

#define OPEN_END }


#define SAVE {

// Сохраняет в CSV файл
void MainWindow::on_actionSave_To_CSV_triggered()
{
    // выбираем куда сохранить
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save CSV File"),
                QString(), QString::fromLatin1("CSV (*.csv)"));

    // если не выбрали
    if (fileName.isEmpty())
    {
        // ниче не делаем
        return;
    }

    // если успешно сохранили
    if (saveCSVFile(fileName))
    {
        // пишем это в статус баре
        textLabel->setText(
                    tr("%1: Data saved by path: %2").
                    arg(QTime::currentTime().toString()).
                    arg(fileName));
    }
    // если не сохранили
    else
    {
        // пишем это в статус баре
        textLabel->setText(
                    tr("%1: Data is not saved. Table is empty!").
                    arg(QTime::currentTime().toString()));
    }
}

// Сохраняет в CSV файл
bool MainWindow::saveCSVFile(const QString& fileName)
{
    QList<QStringList>* data;
    QStringList* column_names;

    // показывает пуста ли сохраняемая таблица
    bool isTablesEmpty = true;
    // если csv таблица не пуста
    if (_customModel != nullptr)
        if (!(_customModel->IsEmpty()))
        {
            // показываем, что таблица не пуста
            isTablesEmpty = false;
            // получаем данные
            data = _customModel->getData();
            // получаем заголовки колонок
            column_names = _customModel->getColNames();
        }

    // если sql таблица не пуста
    if (_sqlTableModel != nullptr && isTablesEmpty)
        if (_sqlTableModel->rowCount() > 0)
        {
            // показываем, что таблица не пуста
            isTablesEmpty = false;

            // получаем заголовки колонок
            column_names = new QStringList();
            for (int j = 0; j < _sqlTableModel->columnCount(); ++j)
                column_names->append(_sqlTableModel->headerData(j, Qt::Horizontal).toString());

            // получаем данные
            QStringList* line = new QStringList();
            data = new QList<QStringList>();
            for (int i = 0; i < _sqlTableModel->rowCount(); ++i)
            {
                line->clear();

                for (int j = 0; j < _sqlTableModel->columnCount(); ++j)
                    line->append(_sqlTableModel->data(_sqlTableModel->index(i, j)).toString());

                data->append(*line);
            }
        }

    // если таблица пуста
    if (isTablesEmpty)
    {
        // возвращаем false
        return false;
    }

    // открываем файл для сохранения
    QFile file{fileName};
    file.open(QIODevice::WriteOnly);
    QString data_string {""};
    QTextStream stream{&file};

    // записываем имена колонок
    data_string = column_names->join(";");
    data_string += ("\r\n");
    stream << data_string;
    data_string.clear();

    // записываем данные
    for(int i = 0; i < data->size(); ++i)
    {
        for(int j = 0; j <data->at(i).size(); ++j)
        {

            data_string += data->at(i)[j];
            data_string += (";");

        }
        data_string.remove(data_string.size()-1,1);
        data_string += ("\r\n");
        stream << data_string;
        data_string.clear();
    }
    stream << data_string;

    // закрываем файл
    file.close();

    // возвращаем true
    return true;
}

// Сохраняет в SQLite
void MainWindow::on_actionSave_To_SQLite_triggered()
{
    // открываем диалог
    saveSqlDialog = new SaveToSqlDialog(this);
    int result = saveSqlDialog->exec();

    QString tableName;
    QSqlDatabase temp;
    // если нажали OK
    if (result == QDialog::Accepted)
    {
        // получаем базу данных, в которую будем сохранять и имя таблицы
        tableName = saveSqlDialog->GetDataToSave(temp);
        // получаем текущую модель
        QAbstractTableModel* currentModel = qobject_cast<QAbstractTableModel*>(ui->tableView->model());
        // создаем таблицу, и заполняем заголовок
        fillFromHeader(*currentModel, temp, tableName);
        // заполняем данные
        fillFromData(*currentModel, temp, tableName);

        // пишем в статус баре об успешном сохранении
        textLabel->setText(
                    tr("%1: Data saved in data base: %2").
                    arg(QTime::currentTime().toString()).
                    arg(temp.databaseName()));
    }
}

// создает таблицу и записывает заголовки
// если таблица существует, она будет перезаписана
void MainWindow::fillFromHeader(
        const QAbstractTableModel& model,
        QSqlDatabase& dataBaseToSave,
        QString tableName)
{
    QSqlQuery query(dataBaseToSave);
    QString str = "CREATE TABLE " + tableName + " (";

    for (int i = 0; i < model.columnCount(); i++)
    {
        str += model.headerData(i, Qt::Horizontal).toString() + " ";

        // если в модели есть данные, а не только заголовок. таблица - просто шапка без строк
        if (model.rowCount())
        {
            QString tmp = whatTypeOfAttribute(model.data(model.index(0, i)).toString()); // тип поля
            str += tmp;
            columnTypes.append(tmp);
        }
        // если модель пуста, то в базе все поля будут TEXT
        else
        {
            str += "TEXT";
            columnTypes.append("TEXT");
        }

        if (i != model.columnCount() - 1)
            str += ", ";
    }

    str += ");";
    query.exec(str);
}

// записывает данные в таблицу
void MainWindow::fillFromData(
        const QAbstractTableModel& model,
        QSqlDatabase& dataBaseToSave,
        QString tableName)
{
    QString str;
    QSqlQuery query(dataBaseToSave);

    dataBaseToSave.transaction();

    for (int i = 0; i < model.rowCount(); ++i)
    {
        str = "INSERT INTO " + tableName + " (";

        for (int j = 0; j < model.columnCount(); ++j)
        {
            str += model.headerData(j, Qt::Horizontal).toString();

            if (j != model.columnCount() - 1)
                str += ", ";
        }

        str += ") VALUES (";

        for (int j = 0; j < model.columnCount(); j++)
        {
            if (columnTypes.at(j) == "TEXT")
                str += "\'" + model.data(model.index(i, j)).toString() + "\'"; // вставляем как строку, база сама преобразует
            else
                str += model.data(model.index(i, j)).toString(); // вставляем как строку, база сама преобразует

            if (j != model.columnCount() - 1)
                str += ", ";
        }

        str += ");";
        query.exec(str);
    }

    dataBaseToSave.commit();
}

// определяет тип атрибута
QString MainWindow::whatTypeOfAttribute(const QString& str) const
{
  QRegExp reg("^\\-?\\d+\\.+\\d+$");

  if (str.contains(reg))
    return "REAL";

  reg = QRegExp("^\\-?\\d+$");
  if (str.contains(reg))
    return "INTEGER";

  return "TEXT";
}

#define SAVE_END }

#define METHODS_END }

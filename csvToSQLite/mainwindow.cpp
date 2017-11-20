#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>


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

// Открывает CSV файл
void MainWindow::on_actionOpen_CSV_triggered()
{
    QString name = QFileDialog::getOpenFileName(this,
                tr("Open file"),
                QString(), QString(QString::fromLatin1("CSV (*.csv)")));
    openFile(name);
}

// Открывает CSV файл
void MainWindow::openFile(const QString& fileName)
{
    // если нет имени файла
    if (fileName.isEmpty())
    {
        // выходим
        return;
    }

    QFile file{fileName};
    // если не смогли открыть файл
    if (!file.open(QIODevice::ReadOnly))
    {
        // выходим
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // считываем имя колонок и типы колонок
   // QString column_types = file.readLine();
    QString column_names = file.readLine();
   // column_types.remove( QRegExp("\r") );
   // column_types.remove( QRegExp("\n") );
    column_names.remove( QRegExp("\r") );
    column_names.remove( QRegExp("\n") );
    QStringList names = column_names.split(";");
   // QStringList types = column_types.split(";");
    // создаем табличную модель для отображения
    _customModel = new CustomTableModel(this, names);

    qDebug() << _customModel->m_column_names.size() << "\n";
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
/*    ui->textEdit->append(
        tr("%1: Data succesfuly loaded").
                arg(QTime::currentTime().toString()));*/
}

// Проверяет строку, удаляет лишние символы, заменяет спец символы, добавляет обработанные данные в табличную модель
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

#define METHODS_END }

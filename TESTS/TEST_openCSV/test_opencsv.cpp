#include <QString>
#include <QtTest>
#include "../../csvToSQLite/mainwindow.h"

class Test_openCSV : public QObject
{
    Q_OBJECT

public:
    explicit Test_openCSV(QObject *parent = 0);

private Q_SLOTS:
    void testCheckString();
    QString checkString(QString &temp, QChar character, QStringList standardItemList);
};

Test_openCSV::Test_openCSV(QObject *parent):
    QObject(parent)
{
}

void Test_openCSV::testCheckString()
{
    QStringList* list = new QStringList();
    QString test = "111";
    QString answer1 = "";
    QString answer2 = "\"1111";
    //checkString(test, ';', *list);
    QCOMPARE(checkString(test, ';', *list), answer1);

    test = "\"111";
    QCOMPARE(checkString(test, '1', *list), answer2);
    QVERIFY2(true, "Failure");
}


// Проверяет строку, удаляет лишние символы,
// заменяет спец символы, добавляет обработанные данные в табличную модель
QString Test_openCSV::checkString(QString &temp, QChar character, QStringList standardItemList)
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
            QString returnString = "";
            for(int i =0; i < standardItemList.size(); i++)
            {
                returnString += standardItemList[i];
            }
            return returnString;
            //standardItemList.clear();
        }
        temp.clear();
    } else {
        temp.append(character);
    }
    return temp;
}

QTEST_APPLESS_MAIN(Test_openCSV)

#include "test_opencsv.moc"

#ifndef TABLECUSTOMMODEL_H
#define TABLECUSTOMMODEL_H

#include <QAbstractTableModel>

enum {
    HEADER_INT,
    HEADER_STRING,
    HEADER_LOGIC,
    HEADER_FLOAT
};

class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CustomTableModel(QObject * parent , QStringList& column_names, QStringList& types);
    explicit CustomTableModel(QObject * parent, QStringList& column_names);
    virtual ~CustomTableModel();

    // Обязательные методы
    // количество строк. Устанавливаем так, чтобы скроллер отображался корректно
    virtual int rowCount(const QModelIndex &rcParent = QModelIndex()) const;
    // устанавливаем количество столбцов. Поскольку они не меняются - указываем константу
    virtual int columnCount(const QModelIndex &rcParent = QModelIndex()) const;
    // функция для передачи данных пользователю
    virtual QVariant data(const QModelIndex &index, int role = Qt::UserRole) const;
    // Функция для приёма данных от пользователя
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::UserRole);
    // Описание заголовков таблицы
    virtual QVariant headerData(int nSection, Qt::Orientation nOrientation, int nRole  = Qt::UserRole) const;
    // Определение возможностей ячейки таблицы (отображение, редактируемость...)
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    void clear();
    bool IsEmpty();
    bool insertRows(int nRow, int nCount, const QModelIndex &rcParent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent_index = QModelIndex());
    bool moveRows(const QModelIndex &rcParentSource, int nRowSource, int nCount,
            const QModelIndex &rcParentDest, int nChildDest);
    bool appendRow(QStringList data);


    Qt::DropActions supportedDropActions() const;

    QList<QStringList>* getData();
    QStringList* getColNames();
    QList<QPair<QString,int>>* getColTypes();

    // определяет тип колонки
    void DetermineColumnsTypes();
QStringList m_column_names;
protected:

    QList<QPair<QString,int>> m_types;
    QList<QStringList> m_data; //< Список для хранения записей

private:
    int InitCustomTableModel(QObject * parent, QStringList& column_names, QStringList& types);
    bool CheckIntType(int indexColumn);
    bool CheckRealType(int indexColumn);
    bool CheckLogicType(int indexColumn);
};

#endif // TABLECUSTOMMODEL_H

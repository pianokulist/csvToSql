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

    //! A constructor.
    /*!
        Initializes a new instanse of CustomTableModel class.
        \param parent A pointer to the parent class.
        \param column_names Names of the columns.
        \param types Types of the columns.
    */
    explicit CustomTableModel(QObject * parent , QStringList& column_names, QStringList& types);

    //! A constructor.
    /*!
        Initializes a new instanse of CustomTableModel class.
        \param parent A pointer to the parent class.
        \param column_names Names of the columns.
    */
    explicit CustomTableModel(QObject * parent, QStringList& column_names);

    //! A destructor.
    /*!
        Destroy the instanse of CustomTableModel class.
    */
    virtual ~CustomTableModel();

    // Обязательные методы
    //! Return a row count.
    virtual int rowCount(const QModelIndex &rcParent = QModelIndex()) const;
    //! Return a column count.
    virtual int columnCount(const QModelIndex &rcParent = QModelIndex()) const;
    //! Gives data to the user.
    virtual QVariant data(const QModelIndex &index, int role = Qt::UserRole) const;
    //! Gets data from user.
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::UserRole);
    //! Sets headers of columns
    virtual QVariant headerData(int nSection, Qt::Orientation nOrientation, int nRole  = Qt::UserRole) const;
    //! Sets flags to the sells.
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Clears the data of table model.
    void clear();
    //! Returns a value, that indicating, that table is empty.
    bool IsEmpty();
    //! Inserts a rows in table.
    bool insertRows(int nRow, int nCount, const QModelIndex &rcParent = QModelIndex());
    //! Remove a rows from table.
    bool removeRows(int row, int count, const QModelIndex &parent_index = QModelIndex());
    //! Moves rows in the table.
    bool moveRows(const QModelIndex &rcParentSource, int nRowSource, int nCount,
            const QModelIndex &rcParentDest, int nChildDest);
    //! Add row to the table.
    bool appendRow(QStringList data);


    Qt::DropActions supportedDropActions() const;

    //! Returns all data from table.
    QList<QStringList>* getData();
    //! Returns columns names.
    QStringList* getColNames();
    //! Returns columns types.
    QList<QPair<QString,int>>* getColTypes();

    //! Determines columns types.
    void DetermineColumnsTypes();


protected:
    //! A column names.
    QStringList m_column_names;
    //! A column types.
    QList<QPair<QString,int>> m_types;
    //! A table data.
    QList<QStringList> m_data; //< Список для хранения записей

private:
    //! Initializes data of table model.
    /*!
        Initializes a new instanse of CustomTableModel class.
        \param column_names Names of the columns.
        \param types Types of the columns.
    */
    void InitCustomTableModel(QStringList& column_names, QStringList& types);

    //! Determines whether column is int type.
    /*!
        Determines whether column is int type.
        \param indexColumn Index of the column.
        \return true if column is int type, otherwise - false.
    */
    bool CheckIntType(int indexColumn);

    //! Determines whether column is real type.
    /*!
        Determines whether column is real type.
        \param indexColumn Index of the column.
        \return true if column is real type, otherwise - false.
    */
    bool CheckRealType(int indexColumn);

    //! Determines whether column is logic type.
    /*!
        Determines whether column is logic type.
        \param indexColumn Index of the column.
        \return true if column is logic type, otherwise - false.
    */
    bool CheckLogicType(int indexColumn);
};

#endif // TABLECUSTOMMODEL_H

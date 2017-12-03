#include "customtablemodel.h"
#include <QString>
#include <QDebug>

#define CONSTRUCTORS {

CustomTableModel::CustomTableModel(QObject * parent, QStringList& column_names, QStringList& types)
    : QAbstractTableModel(parent)
{
    InitCustomTableModel(parent, column_names, types);
}

#define CONSTRUCTORS_END }



#define DESTRUCTORS {

CustomTableModel::CustomTableModel(QObject * parent, QStringList& column_names)
{
    QStringList* types = new QStringList();
    for(int i = 0; i < column_names.size(); ++i)
    {
        types->append("string");
    }
    InitCustomTableModel(parent, column_names, *types);
}

#define DESTRUCTORS_END }



#define METHODS {

int CustomTableModel::InitCustomTableModel(QObject * parent, QStringList& column_names, QStringList& types)
{
    m_column_names = column_names;

    QList<QPair<QString,int>> t_types;
    for(int i = 0; i < types.size(); ++i){
        QPair<QString, int> type;
        type.first = types[i];
        if (type.first.contains("int", Qt::CaseInsensitive))
                type.second = HEADER_INT;
        if (type.first.contains("logic", Qt::CaseInsensitive))
                type.second = HEADER_LOGIC;
        if (type.first.contains("string", Qt::CaseInsensitive))
                type.second = HEADER_STRING;
        if (type.first.contains("float", Qt::CaseInsensitive))
                type.second = HEADER_FLOAT;
        t_types.push_back(type);
    }
    m_types = t_types;
}

CustomTableModel::~CustomTableModel()
{
    clear();
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::EditRole || role == Qt::DisplayRole || role== Qt::CheckStateRole ))
    {
        QStringList &record = m_data[index.row()];
        // определяем соответствие между столбцами и полями структуры
        switch(m_types[index.column()].second) {
        case HEADER_INT:
            record[index.column()] = value.toString();
            break;
        case HEADER_STRING:
            record[index.column()] = value.toString();
            break;
        case HEADER_LOGIC:
            record[index.column()] = QVariant(value.toInt() == Qt::Checked).toString();
            break;
        case HEADER_FLOAT:
            record[index.column()] = value.toString();
            break;
        default:
            return false;
        }
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

int CustomTableModel::rowCount(const QModelIndex &rcParent) const
{
    return m_data.size();
}

int CustomTableModel::columnCount(const QModelIndex &rcParent) const
{
    return m_column_names.size();
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
        if (index.row() < rowCount() && index.column() < columnCount())
        {
            const QString &record = m_data[index.row()][index.column()];
            if (role == Qt::DisplayRole || role == Qt::EditRole)
            {
                // определяем соответствие между столбцами и полями структуры
                switch(m_types[index.column()].second) {
                case HEADER_INT:
                    return record.toInt();
                case HEADER_STRING:
                    return record;
                case HEADER_LOGIC:
                    return QVariant(record).toBool();
                case HEADER_FLOAT:
                    return record.toFloat();
                }
                return record;
            }
            else if (role == Qt::CheckStateRole && m_types[index.column()].second == HEADER_LOGIC)
            {
                return QVariant(record).toBool() ? Qt::Checked : Qt::Unchecked;
            }
            return QString();
        }
    return QString();
}

QVariant CustomTableModel::headerData(int nSection,
                                  Qt::Orientation nOrientation,
                                  int nRole) const
{
    if (nRole == Qt::DisplayRole && nOrientation == Qt::Horizontal)
    {
        return m_column_names[nSection];
    }
    return QVariant();//QAbstractTableModel::headerData(nSection, nOrientation, nRole);
}

bool CustomTableModel::insertRows(int nRow, int nCount, const QModelIndex &rcParent)
{
    if (nRow < 0)
    {
        nRow = rowCount();
    }
    beginInsertRows(QModelIndex(), nRow, nRow + nCount - 1);

    QStringList value_str;
    value_str.reserve(this->columnCount());
    for (int i = 0; i < this->columnCount(); ++i)
    {
        int type = m_types[i].second;
        switch(type) {
        case HEADER_INT:
            value_str.push_back(QString::number(0));
            break;
        case HEADER_STRING:
            value_str.push_back(" ");
            break;
        case HEADER_LOGIC:
            value_str.push_back(QVariant(0 == Qt::Checked).toString());
            break;
        case HEADER_FLOAT:
            value_str.push_back( QString::number(0));
            break;
        }
    }
    m_data.insert(nRow, value_str);
    endInsertRows();
    return true;
}

bool CustomTableModel::removeRows(int row, int count, const QModelIndex &parent_index)
{

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_data.removeAt(row);
    }
    endRemoveRows();
    return true;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &rcIndex) const
{
    Qt::ItemFlags result = QAbstractTableModel::flags(rcIndex);
    if (!rcIndex.isValid())
        return Qt::ItemIsEnabled;

    result |= Qt::ItemIsDragEnabled;
    if (m_types[rcIndex.column()].second == HEADER_LOGIC)
    {
        result |= Qt::ItemIsUserCheckable;
    }
    else
    {
        result |= Qt::ItemIsEditable;
    }
    return result;
}

bool CustomTableModel::appendRow(QStringList data)
{
    if (data.size() != m_column_names.size())
        return false;
    m_data.push_back(data);
    return true;
}

bool CustomTableModel::moveRows(
        const QModelIndex &rcParentSource, int nRowSource, int nCount,
        const QModelIndex &rcParentDest, int nChildDest)
{
    beginMoveRows(
                QModelIndex(), nRowSource, nRowSource + nCount - 1,
                QModelIndex(), nChildDest + nCount - 1);
    //
    for (int i = 0; i < nCount; ++ i)
        m_data.move(nRowSource, nChildDest + i);
    //
    endMoveRows();
    return true;
}

Qt::DropActions CustomTableModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QList<QStringList>* CustomTableModel::getData()
{
    return &m_data;
}

QStringList* CustomTableModel::getColNames()
{
    return &m_column_names;
}

QList<QPair<QString,int>>* CustomTableModel::getColTypes()
{
    return &m_types;
}


void CustomTableModel::DetermineColumnsTypes()
{
    // если список данных пуст
    if (m_data.isEmpty() || m_data.size() == 0)
        return;

    m_types.clear();
    // для всех колонок
    for (int i = 0; i < m_data[0].size(); ++i)
    {
        QPair<QString, int> type;
        // если колонка типа int
        if (CheckIntType(i))
        {
            type.first = "int";
            type.second = HEADER_INT;
            m_types.append(type);
        }
        // если колонка типа real (float)
        else if (CheckRealType(i))
        {
            type.first = "float";
            type.second = HEADER_FLOAT;
            m_types.append(type);
        }
        // если колонка типа bool (logic)
        else if (CheckLogicType(i))
        {
            type.first = "logic";
            type.second = HEADER_LOGIC;
            m_types.append(type);
        }
        // если колонка типа string
        else
        {
            type.first = "string";
            type.second = HEADER_STRING;
            m_types.append(type);
        }

    }

}

bool CustomTableModel::CheckIntType(int indexColumn)
{
    // для всех колонок
    for (int i = 0; i < m_data.size(); ++i)
    {
        bool check = true;
        int temp = m_data[i][indexColumn].toInt(&check);
        // если не смогли привести значение к типу int
        // и строка не пустая
        if (!check && !(m_data[i][indexColumn].isEmpty()))
        {
            // то значит колонка не является типа integer
           return false;
        }
    }

    return true;
}

bool CustomTableModel::CheckRealType(int indexColumn)
{
    // для всех колонок
    for (int i = 0; i < m_data.size(); ++i)
    {
        bool check = true;
        int temp = m_data[i][indexColumn].toFloat(&check);
        // если не смогли привести значение к типу float
        // и строка не пустая
        if (!check && !(m_data[i][indexColumn].isEmpty()))
        {
            // то значит колонка не является типа float
           return false;
        }
    }

    return true;
}

bool CustomTableModel::CheckLogicType(int indexColumn)
{
    // для всех колонок
    for (int i = 0; i < m_data.size(); ++i)
    {
        QString temp = m_data[i][indexColumn];
        // если не смогли привести значение к типу bool
        // и строка не пустая
        if (!(QString::compare(temp, "true", Qt::CaseInsensitive) == 0 ||
              QString::compare(temp, "false", Qt::CaseInsensitive) == 0) &&
             !(m_data[i][indexColumn].isEmpty()))
        {
            // то значит колонка не является типа bool
           return false;
        }
    }

    return true;
}

void CustomTableModel::clear()
{
    m_data.clear();
    m_types.clear();
}

bool CustomTableModel::IsEmpty()
{
    return m_data.empty();
}

#define METHODS_END }

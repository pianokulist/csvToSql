#ifndef CUSTOMSQLTABLEMODEL_H
#define CUSTOMSQLTABLEMODEL_H

#include <QSqlQueryModel>

class CustomSqlTableModel : public QSqlQueryModel
{
public:
    CustomSqlTableModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int /* role */);

private:
    bool setFirstName(int personId, const QString &firstName);
    bool setLastName(int personId, const QString &lastName);
    void refresh();
};

#endif // CUSTOMSQLTABLEMODEL_H

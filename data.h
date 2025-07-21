#ifndef DATA_H
#define DATA_H
#include "QString"
#include <QObject>
#include <QMap>
#include <QVariantList>
#include <QSqlDatabase>
#include <QSqlQuery>


class Data
{
public:
    QSqlDatabase m_db;
    Data();
    ~Data();
    bool openDB(const QString& dbPath);
    QSqlQuery getUserByUsername(const QString &username);
    QSqlQuery query(const QString &sql);


    QSqlDatabase getDatabase() const
    {
        return m_db;
    }



};

#endif // DATA_H

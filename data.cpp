#include "data.h"
#include "QString"
#include <QObject>
#include <QMap>
#include <QVariant>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
Data::Data()
{

}
Data::~Data()
{

}
bool Data::openDB(const QString& dbPath)
{
//    if(m_db.isOpen()) return true;
//    m_db=QSqlDatabase::addDatabase("QSQLITE");
//    m_db.setDatabaseName(dbPath);
//    if(!m_db.open())
//    {
//        qWarning()<<"Failed to open database:"<<m_db.lastError().text();
//        return false;
//    }
//    return true;
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (db.isValid() && db.isOpen()) {
        return true;
    }
    // 若不存在，再重新创建
    db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qWarning() << "Failed to open database" << db.lastError().text();
        return false;
    }
    return true;


}
QSqlQuery Data::getUserByUsername(const QString &username)
{
    QSqlQuery q;
    q.prepare(("SELECT * FROM test WHERE username = ?"));
    q.addBindValue(username);
    q.exec();
    return q;

}
QSqlQuery Data::query(const QString &sql)
{
    QSqlQuery sqlQuery(m_db);
    if(!sqlQuery.exec(sql))
    {
        qWarning()<<"SQL执行失败"<<sqlQuery.lastError().text();
        qWarning()<<"SQL语句"<<sql;

    }
    return sqlQuery;
}





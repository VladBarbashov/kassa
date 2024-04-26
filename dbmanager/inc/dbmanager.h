#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QList>
#include <QSet>
#include <QString>
#include <QVariant>

#include "dbtypes.h"

class QSqlQuery;

namespace db
{
class DBManager final
{
public:
    DBManager();
    DBManager(const QString &connectionName, const QString &host, const QString &user, const QString &passwd, const QString &dbName, DBDriver driver = DBDriver::MYSQL);
    DBManager(const DBManager &other);
    DBManager(DBManager &&other);

    DBManager &operator=(const DBManager &other);
    DBManager &operator=(DBManager &&other);

    ~DBManager();

    db::DBRes addNewConnection(const QString &connectionName, const QString &host, const QString &user, const QString &passwd, const QString &dbName, DBDriver driver = DBDriver::MYSQL);
    db::DBRes removeConnection(const QString &connectionName);

    db::DBRes setActiveConnection(const QString &connectionName);
    db::DBRes closeActiveConnection();
    db::DBState connectionState() const;

    db::QueryRes performQuery(const QString &query);
    QList<QVariant> nextRow();
    QList<QVariant> fields(unsigned int column);

private:
    QString getDriver(DBDriver driver) const;

    db::DBRes checkConnection(const QString &connectionName, const QString &host, const QString &user, const QString &passwd, const QString &dbName, DBDriver driver = DBDriver::MYSQL) const;

    void clearConnections();

    QString activeConnection;
    QSqlQuery *queryManager;

    QSet<QString> connections;
};
}

#endif // DBMANAGER_H

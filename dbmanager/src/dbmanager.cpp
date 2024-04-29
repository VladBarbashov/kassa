#include "dbmanager.h"

#include <QSqlQuery>

db::DBManager::DBManager() {}

db::DBManager::DBManager(const QString &connectionName, const QString &host, const QString &user, const QString &passwd, const QString &dbName, db::DBDriver driver)
{
    addNewConnection(connectionName, host, user, passwd, dbName, driver);
    setActiveConnection(connectionName);
}

db::DBManager::DBManager(const db::DBManager &other) : activeConnection(other.activeConnection), connections(other.connections) {}

db::DBManager::DBManager(db::DBManager &&other) : activeConnection(std::move(other.activeConnection)), connections(std::move(other.connections)) {}

db::DBManager &db::DBManager::operator=(const db::DBManager &other)
{
    if (this != &other)
    {
        clearConnections();
        activeConnection = other.activeConnection;
        connections.unite(other.connections);
    }
    return *this;
}

db::DBManager &db::DBManager::operator=(db::DBManager &&other)
{
    if (this != &other)
    {
        clearConnections();
        qSwap(activeConnection, other.activeConnection);
        connections.swap(other.connections);
    }
    return *this;
}

db::DBManager::~DBManager()
{
    clearConnections();
}

db::DBRes db::DBManager::addNewConnection(const QString &connectionName, const QString &host, const QString &user, const QString &passwd, const QString &dbName, db::DBDriver driver)
{
    switch (checkConnection(connectionName, host, user, passwd, dbName, driver))
    {
    case db::DBRes::OK:
        if (!connections.contains(connectionName))
        {
            connections.insert(connectionName);
            return db::DBRes::OK;
        }
        return db::DBRes::DUBLICATE_ENTRY;
    case db::DBRes::DUBLICATE_ENTRY:
        return db::DBRes::DUBLICATE_ENTRY;
    case db::DBRes::NO_ENTRY:
    {
        QSqlDatabase qdb = QSqlDatabase::addDatabase(getDriver(driver), connectionName);
        qdb.setHostName(host);
        qdb.setUserName(user);
        qdb.setPassword(passwd);
        qdb.setDatabaseName(dbName);
        connections.insert(connectionName);
        return db::DBRes::OK;
    }
    default:
        return db::DBRes::ERROR;
    }
}

db::DBRes db::DBManager::removeConnection(const QString &connectionName)
{
    if (connections.contains(connectionName))
    {
        QSqlDatabase qdb = QSqlDatabase::database(connectionName, false);
        if (activeConnection == connectionName)
        {
            closeActiveConnection();
        }
        if (!qdb.isOpen())
        {
            QSqlDatabase::removeDatabase(connectionName);
        }
        connections.remove(connectionName);
        return db::DBRes::OK;
    }
    return db::DBRes::NO_ENTRY;
}

db::DBRes db::DBManager::setActiveConnection(const QString &connectionName)
{
    if (connections.contains(connectionName))
    {
        closeActiveConnection();
        activeConnection = connectionName;
        if (QSqlDatabase::database(activeConnection, false).open())
        {
            return db::DBRes::OK;
        }
        return db::DBRes::ERROR;
    }
    return db::DBRes::NO_ENTRY;
}

db::DBRes db::DBManager::closeActiveConnection()
{
    if (!activeConnection.isEmpty())
    {
        delete queryManager;
        QSqlDatabase::database(activeConnection, false).close();
        activeConnection.clear();
        return db::DBRes::OK;
    }
    return db::DBRes::NO_ENTRY;
}

db::DBState db::DBManager::connectionState() const
{
    if (!activeConnection.isEmpty())
    {
        if (QSqlDatabase::database(activeConnection, false).isOpen())
        {
            return db::DBState::OK;
        }
        return db::DBState::INVALID_CONNECTION;
    }
    return db::DBState::NO_CONNECTION;
}

db::QueryRes db::DBManager::performQuery(const QString &query)
{
    if (connectionState() == db::DBState::OK)
    {
        delete queryManager;
        queryManager = new QSqlQuery(query, QSqlDatabase::database(activeConnection, false));
        if (queryManager->isActive())
        {
            if (queryManager->size())
            {
                return db::QueryRes::OK;
            }
            return db::QueryRes::EMPTY;
        }
    }
    return db::QueryRes::ERROR;
}

QList<QVariant> db::DBManager::nextRow()
{
    QList<QVariant> list;
    if (queryManager && queryManager->next())
    {
        for (int i = 0; !queryManager->isNull(i); ++i)
        {
            list << queryManager->value(i);
        }
    }
    return list;
}

QList<QVariant> db::DBManager::fields(unsigned int column)
{
    QList<QVariant> list;
    if (queryManager)
    {
        while (queryManager->next())
        {
            list << queryManager->value(column);
        }
    }
    return list;
}

QString db::DBManager::getDriver(db::DBDriver driver) const
{
    switch (driver)
    {
    case db::DBDriver::MYSQL:
        return QString("QMYSQL");
    case db::DBDriver::POSTGRES:
        return QString("QPSQL");
    case db::DBDriver::SQLITE:
        return QString("QSQLITE");
    default:
        return QString();
    }
}

db::DBRes db::DBManager::checkConnection(const QString &connectionName, const QString &host, const QString &user, const QString &passwd, const QString &dbName, db::DBDriver driver) const
{
    if (QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase qdb = QSqlDatabase::database(connectionName, false);
        if ((qdb.hostName() == host) && (qdb.userName() == user) && (qdb.password() == passwd) && (qdb.databaseName() == dbName) && (qdb.driverName() == getDriver(driver)))
        {
            return db::DBRes::OK;
        }
        return db::DBRes::DUBLICATE_ENTRY;
    }
    if (QSqlDatabase::isDriverAvailable(getDriver(driver)))
    {
        return db::DBRes::NO_ENTRY;
    }
    return db::DBRes::ERROR;
}

void db::DBManager::clearConnections()
{
    closeActiveConnection();
    auto it = connections.cbegin();
    while (it != connections.cend())
    {
        removeConnection(*it);
        it = connections.cbegin();
    }
}

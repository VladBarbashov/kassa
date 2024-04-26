#ifndef DBTYPES_H
#define DBTYPES_H

namespace db
{
enum class DBDriver
{
    MYSQL,
    POSTGRES,
    SQLITE
};

enum class DBState
{
    OK,
    NO_CONNECTION,
    INVALID_CONNECTION
};

enum class DBRes
{
    OK,
    DUBLICATE_ENTRY,
    NO_ENTRY,
    ERROR
};

enum class QueryRes
{
    OK,
    EMPTY,
    ERROR
};
}

#endif // DBTYPES_H

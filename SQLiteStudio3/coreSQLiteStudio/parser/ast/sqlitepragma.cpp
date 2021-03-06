#include "sqlitepragma.h"
#include "sqlitequerytype.h"

#include <parser/statementtokenbuilder.h>

SqlitePragma::SqlitePragma()
{
    queryType = SqliteQueryType::Pragma;
}

SqlitePragma::SqlitePragma(const SqlitePragma& other) :
    SqliteQuery(other), database(other.database), pragmaName(other.pragmaName), value(other.value), equalsOp(other.equalsOp), parenthesis(other.parenthesis)
{
}

SqlitePragma::SqlitePragma(const QString &name1, const QString &name2)
    : SqlitePragma()
{
    initName(name1, name2);
}

SqlitePragma::SqlitePragma(const QString &name1, const QString &name2, const QVariant& value, bool equals)
    : SqlitePragma()
{
    initName(name1, name2);
    this->value = value;
    if (equals)
        equalsOp = true;
    else
        parenthesis = true;
}

SqlitePragma::SqlitePragma(const QString &name1, const QString &name2, const QString &value, bool equals)
    : SqlitePragma()
{
    initName(name1, name2);
    this->value = value;
    if (equals)
        equalsOp = true;
    else
        parenthesis = true;
}

SqliteStatement*SqlitePragma::clone()
{
    return new SqlitePragma(*this);
}

QStringList SqlitePragma::getDatabasesInStatement()
{
    return getStrListFromValue(database);
}

TokenList SqlitePragma::getDatabaseTokensInStatement()
{
    if (database.isNull())
        return TokenList();

    return getTokenListFromNamedKey("nm");
}

QList<SqliteStatement::FullObject> SqlitePragma::getFullObjectsInStatement()
{
    QList<FullObject> result;
    if (database.isNull())
        return result;

    // Db object
    FullObject fullObj = getFirstDbFullObject();
    if (fullObj.isValid())
    {
        result << fullObj;
        dbTokenForFullObjects = fullObj.database;
    }

    return result;
}

void SqlitePragma::initName(const QString &name1, const QString &name2)
{
    if (!name2.isNull())
    {
        database = name1;
        pragmaName = name2;
    }
    else
        pragmaName = name1;
}

TokenList SqlitePragma::rebuildTokensFromContents()
{
    StatementTokenBuilder builder;
    builder.withTokens(SqliteQuery::rebuildTokensFromContents());
    builder.withKeyword("PRAGMA").withSpace();

    if (!database.isNull())
        builder.withOther(database).withOperator(".");

    builder.withOther(pragmaName);

    if (equalsOp)
        builder.withSpace().withOperator("=").withSpace().withLiteralValue(value);
    else if (parenthesis)
        builder.withParLeft().withLiteralValue(value).withParRight();

    builder.withOperator(";");

    return builder.build();
}

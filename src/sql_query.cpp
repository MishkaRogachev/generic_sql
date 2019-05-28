#include "sql_query.h"

#include <QDebug>
#include <QSqlError>

using namespace flt::data_source;

SqlQuery::SqlQuery(const QSqlDatabase& database) : ISqlQuery(), m_query(database)
{
}

void SqlQuery::prepare(const QString& query)
{
    m_query.prepare(query);
}

bool SqlQuery::exec(const QString& query)
{
    return query.isEmpty() ? m_query.exec() : m_query.exec(query);
}

bool SqlQuery::first()
{
    return m_query.first();
}

bool SqlQuery::next()
{
    return m_query.next();
}

void SqlQuery::clear()
{
    m_query.clear();
}

void SqlQuery::finish()
{
    m_query.finish();
}

QVariant SqlQuery::value(int i)
{
    return m_query.value(i);
}

QVariant SqlQuery::value(const QString& column)
{
    return m_query.value(column);
}

QVariant SqlQuery::lastInsertId() const
{
    return m_query.lastInsertId();
}

void SqlQuery::bindValue(const QString& placeholer, const QVariant& value)
{
    m_query.bindValue(placeholer, value);
}

QString SqlQuery::lastQuery() const
{
    return m_query.lastQuery();
}

int SqlQuery::size() const
{
    return m_query.size();
}

bool SqlQuery::noError() const
{
    return m_query.lastError().type() == QSqlError::NoError;
}

QString SqlQuery::errorString() const
{
    return m_query.lastError().text();
}

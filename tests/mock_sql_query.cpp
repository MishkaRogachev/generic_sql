#include "mock_sql_query.h"

// Qt
#include <QDebug>

void MockSqlQuery::prepare(const QString& query)
{
    m_preparedQuery = query;
}

bool MockSqlQuery::exec(const QString& query)
{
    if (query.isEmpty())
    {
        if (m_preparedQuery == m_expectedQuery && m_preparedQuery.length()) return true;

        qDebug() << "exec fail:" << m_preparedQuery << m_expectedQuery;
        return false;
    }

    if (query == m_expectedQuery && query.length()) return true;
    qDebug() << "exec fail:" << query << m_expectedQuery;

    return false;
}

bool MockSqlQuery::next()
{
    if (m_expectedValues.isEmpty()) return false;

    m_queryValues = m_expectedValues.takeFirst();
    return true;
}

void MockSqlQuery::clear()
{
}

void MockSqlQuery::finish()
{
}

QVariant MockSqlQuery::value(int i)
{
    if (m_queryValues.count() <= i) return QVariant();

    return m_queryValues.values().at(i);
}

QVariant MockSqlQuery::value(const QString& column)
{
    return m_queryValues.value(column);
}

QVariant MockSqlQuery::lastInsertId() const
{
    return m_lastInsertId;
}

void MockSqlQuery::bindValue(const QString& placeholer, const QVariant& value)
{
    m_preparedQuery.replace(placeholer, value.toString());
}

QString MockSqlQuery::lastQuery() const
{
    return QString();
}

QString MockSqlQuery::errorString() const
{
    return QString();
}

void MockSqlQuery::setExpectedQuery(const QString& expectedQuery)
{
    m_expectedQuery = expectedQuery;
}

void MockSqlQuery::addExpectedValues(const QVariantMap& expectedValues)
{
    m_expectedValues.append(expectedValues);
}

void MockSqlQuery::setLastInsertId(const QVariant& lastInsertId)
{
    m_lastInsertId = lastInsertId;
}

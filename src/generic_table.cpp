#include "generic_table.h"

#include <QDebug>

namespace flt
{
namespace data_source
{
GenericTable::GenericTable(ISqlQuery* query, const QString& tableName) :
    m_query(query),
    m_tableName(tableName)
{
}

GenericTable::~GenericTable()
{
}

bool GenericTable::init()
{
    if (!m_query->exec("PRAGMA table_info(" + m_tableName + ")")) return false;

    while (m_query->next())
        m_columnNames.append(m_query->value(1).toString());
    m_query->finish();
    return true;
}

void GenericTable::init(const QStringList& columnNames)
{
    m_columnNames = columnNames;
}

QVariantList GenericTable::select(const QVariantMap& conditions, const QStringList& resultColumns)
{
    this->prepareSelect(conditions, resultColumns);

    QVariantList list;
    if (!m_query->exec()) return list;
    while (m_query->next())
    {
        if (resultColumns.count() > 1)
        {
            QVariantList valueList;
            for (const QString& column : resultColumns)
            {
                valueList.append(m_query->value(column));
            }
            list.append(QVariant(valueList));
        }
        else
        {
            list.append(m_query->value(resultColumns.first()));
        }
    }

    m_query->finish();

    return list;
}

QVariantList GenericTable::select(const Condition& condition, const QStringList& resultColumns)
{
    return this->select({ { condition.first, condition.second } }, resultColumns);
}

QVariantList GenericTable::select(int id, const QStringList& resultColumns)
{
    return this->select(Condition({ ::id, id }), resultColumns);
}

QList<int> GenericTable::selectIds(const QVariantMap& conditions, const QString& column)
{
    this->prepareSelect(conditions, { column });

    QList<int> list;
    if (!m_query->exec()) return list;
    while (m_query->next())
    {
        list.append(m_query->value(column).toInt());
    }

    m_query->finish();

    return list;
}

QList<int> GenericTable::selectIds(const Condition& condition, const QString& column)
{
    return this->selectIds({ { condition.first, condition.second } }, column);
}

bool GenericTable::insert(const QVariantMap& valueMap, int* id)
{
    QStringList placeholders;

    for (const QString& name : valueMap.keys())
    {
        placeholders.append(::hold + name);
    }

    QString namesJoin = valueMap.keys().join(::comma);
    QString valuesJoin = placeholders.join(::comma);
    m_query->prepare("INSERT INTO " + m_tableName + " (" + namesJoin + ") VALUES (" + valuesJoin +
                     ")");

    for (const QString& name : valueMap.keys())
    {
        m_query->bindValue(::hold + name, valueMap.value(name));
    }

    if (!m_query->exec()) return false;
    if (id) *id = m_query->lastInsertId().toInt();

    m_query->finish();
    return true;
}

bool GenericTable::removeByConditions(const QVariantMap& conditions)
{
    if (conditions.isEmpty()) return false;

    m_query->prepare("DELETE FROM " + m_tableName + this->where(conditions));
    for (const QString& key : conditions.keys())
    {
        m_query->bindValue(::hold + key, conditions.value(key));
    }

    if (!m_query->exec()) return false;

    m_query->finish();
    return true;
}

bool GenericTable::removeByCondition(const Condition& condition)
{
    return this->removeByConditions({ { condition.first, condition.second } });
}

bool GenericTable::removeById(int id)
{
    return this->removeByCondition({ ::id, id });
}

ISqlQuery* GenericTable::query() const
{
    return m_query.data();
}

QString GenericTable::tableName() const
{
    return m_tableName;
}

QStringList GenericTable::columnNames() const
{
    return m_columnNames;
}

void GenericTable::prepareSelect(const QVariantMap& conditions, const QStringList& resultColumns)
{
    QString queryString = "SELECT " +
                          (resultColumns.isEmpty() ? "*" : resultColumns.join(::comma)) + " FROM " +
                          m_tableName;
    if (!conditions.isEmpty()) queryString += this->where(conditions);
    m_query->prepare(queryString);
    for (const QString& key : conditions.keys())
    {
        m_query->bindValue(::hold + key, conditions.value(key));
    }
}

QString GenericTable::where(const QVariantMap& conditions) const
{
    QStringList conditionList;
    for (const QString& key : conditions.keys())
    {
        conditionList.append(key + " = " + ::hold + key);
    }

    return QString(" WHERE ") + conditionList.join(" AND ");
}
} // namespace data_source
} // namespace flt

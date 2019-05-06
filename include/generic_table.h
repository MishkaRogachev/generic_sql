#ifndef GENERIC_TABLE_H
#define GENERIC_TABLE_H


#include "export.h"
#include "i_sql_query.h"

namespace
{
const QString id = "id";
const QString hold = ":";
const QString comma = ", ";
} // namespace

namespace flt
{
namespace data_source
{
using Condition = QPair<QString, QVariant>;

class GENERIC_SQL_EXPORT GenericTable // Table gateway
{
public:
    GenericTable(ISqlQuery* query, const QString& tableName);
    virtual ~GenericTable();

    bool init();
    void init(const QStringList& columnNames);

    QVariantList select(const QVariantMap& conditions,
                        const QStringList& resultColumns = QStringList());
    QVariantList select(const Condition& condition,
                        const QStringList& resultColumns = QStringList());
    QVariantList select(int id, const QStringList& resultColumns = QStringList());

    QList<int> selectIds(const QVariantMap& conditions = QVariantMap(),
                         const QString& column = ::id);
    QList<int> selectIds(const Condition& condition, const QString& column = ::id);

    bool insert(const QVariantMap& valueMap, int* id = nullptr);
    bool removeByConditions(const QVariantMap& conditions);
    bool removeByCondition(const Condition& condition);
    bool removeById(int id);

    ISqlQuery* query() const;
    QString tableName() const;
    QStringList columnNames() const;

protected:
    void prepareSelect(const QVariantMap& conditions,
                       const QStringList& resultColumns = QStringList());
    QString where(const QVariantMap& conditions) const;

    data_source::SqlQueryPtr const m_query;

    const QString m_tableName;
    QStringList m_columnNames;
};

} // namespace data_source
} // namespace flt

#endif // GENERIC_TABLE_H

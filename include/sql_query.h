#ifndef SQL_QUERRY_H
#define SQL_QUERRY_H

// Internal
#include "export.h"
#include "i_sql_query.h"

// Qt
#include <QSqlQuery>

namespace flt
{
namespace data_source
{
class GENERIC_SQL_EXPORT SqlQuery : public ISqlQuery
{
public:
    SqlQuery(const QSqlDatabase& database);

    void prepare(const QString& query) override;
    bool exec(const QString& query = QString()) override;
    bool next() override;

    void clear() override;
    void finish() override;

    QVariant value(int i) override;
    QVariant value(const QString& column) override;
    QVariant lastInsertId() const override;
    void bindValue(const QString& placeholer, const QVariant& value) override;

    QString lastQuery() const override;
    QString errorString() const override;

private:
    QSqlQuery m_query;
};

} // namespace data_source
} // namespace flt

#endif // SQL_QUERRY_H

#ifndef I_SQL_QUERRY_H
#define I_SQL_QUERRY_H

#include "export.h"

#include <QObject>
#include <QVariant>

namespace flt
{
namespace data_source
{
class GENERIC_SQL_EXPORT ISqlQuery
{
public:
    ISqlQuery() = default;
    virtual ~ISqlQuery() = default;

    virtual void prepare(const QString& query) = 0;
    virtual bool exec(const QString& query = QString()) = 0;
    virtual bool first() = 0;
    virtual bool next() = 0;

    virtual void clear() = 0;
    virtual void finish() = 0;

    virtual QVariant value(int i) = 0;
    virtual QVariant value(const QString& column) = 0;
    virtual QVariant lastInsertId() const = 0;
    virtual void bindValue(const QString&, const QVariant& value) = 0;

    virtual QString lastQuery() const = 0;
    virtual bool noError() const = 0;
    virtual int size() const = 0;
    virtual QString errorString() const = 0;
};

using SqlQueryPtr = QScopedPointer<ISqlQuery>;

} // namespace data_source
} // namespace flt

#endif // I_SQL_QUERRY_H

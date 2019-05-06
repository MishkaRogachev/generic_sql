#ifndef GENERIC_REPOSITORY_H
#define GENERIC_REPOSITORY_H

#include <QHash>

#include "generic_table.h"

namespace flt
{
namespace data_source
{
template<class T>
class GENERIC_SQL_EXPORT GenericRepository : public GenericTable
{
public:
    GenericRepository(ISqlQuery* query, const QString& tableName);

    bool read(T* entity, const QVariantMap& conditions);
    bool read(T* entity);
    bool insert(T* entity);
    bool update(T* entity);
    bool remove(T* entity);

protected:
    QVariantMap properties(T* entity);
    void bindQuery(T* entity);
    void updateFromQuery(T* entity);
};

} // namespace data_source
} // namespace flt

#include "generic_repository.hpp"

#endif // GENERIC_REPOSITORY_H

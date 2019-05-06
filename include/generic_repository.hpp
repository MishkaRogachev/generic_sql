#include "generic_repository.h"

#include <QDebug>
#include <QMetaProperty>

namespace
{
const QString separator = ";";
const int inheritedProperty = 1;
} // namespace

namespace flt
{
namespace data_source
{
template<class T>
GenericRepository<T>::GenericRepository(ISqlQuery* query, const QString& tableName) :
    GenericTable(query, tableName)
{
}

template<class T>
bool GenericRepository<T>::read(T* entity, const QVariantMap& conditions)
{
    this->prepareSelect(conditions);
    if (!m_query->exec() || !m_query->next()) return false;

    this->updateFromQuery(entity);
    return true;
}

template<class T>
bool GenericRepository<T>::read(T* entity)
{
    return this->read(entity, { { ::id, entity->id() } });
}

template<class T>
bool GenericRepository<T>::insert(T* entity)
{
    QVariantMap valueMap;
    QVariantMap entityMap = this->properties(entity);
    for (const QString& name : entityMap.keys())
    {
        // Skip inserting zero id
        if (name == ::id && entity->id() == 0) continue;

        valueMap.insert(name, entityMap.value(name));
    }

    int id = 0;
    if (!GenericTable::insert(valueMap, &id)) return false;

    entity->setProperty(::id.toLocal8Bit(), id);
    return true;
}

template<class T>
bool GenericRepository<T>::update(T* entity)
{
    QStringList placeholders;

    for (const QString& name : this->properties(entity).keys())
    {
        if (name == ::id) continue;
        if (m_columnNames.contains(name)) placeholders.append(name + " = :" + name);
    }

    // FIZME: use GenericTable UPDATE
    QString placeholdersJoin = placeholders.join(::comma);
    m_query->prepare("UPDATE " + m_tableName + " SET " + placeholdersJoin + " WHERE id = :id");

    m_query->bindValue(::hold + ::id, entity->id());
    this->bindQuery(entity);

    if (!m_query->exec()) return false;

    m_query->finish();
    return true;
}

template<class T>
bool GenericRepository<T>::remove(T* entity)
{
    if (this->removeById(entity->id()))
    {
        entity->setProperty(::id.toLocal8Bit(), 0);
        return true;
    }
    return false;
}

template<class T>
QVariantMap GenericRepository<T>::properties(T* entity)
{
    const QMetaObject* meta = entity->metaObject();
    QVariantMap map;

    for (int i = ::inheritedProperty; i < meta->propertyCount(); ++i)
    {
        QMetaProperty property = meta->property(i);
        // Skip missing columns
        if (!m_columnNames.contains(property.name())) continue;

        map.insert(property.name(), meta->property(i).read(entity));
    }

    return map;
}

template<class T>
void GenericRepository<T>::bindQuery(T* entity)
{
    const QMetaObject* meta = entity->metaObject();

    for (int i = ::inheritedProperty; i < meta->propertyCount(); ++i)
    {
        QMetaProperty property = meta->property(i);
        QString name = property.name();

        if (!m_columnNames.contains(name)) continue;

        QVariant value = property.read(entity);

        // workaround for StringList
        if (value.type() == QVariant::StringList)
        {
            value = value.toStringList().join(::separator);
        }

        m_query->bindValue(QString(":") + name, value);
    }
}

template<class T>
void GenericRepository<T>::updateFromQuery(T* entity)
{
    const QMetaObject* meta = entity->metaObject();

    for (int i = ::inheritedProperty; i < meta->propertyCount(); ++i)
    {
        QMetaProperty property = meta->property(i);
        QString name = property.name();
        if (!m_columnNames.contains(name)) continue;

        QVariant value = m_query->value(property.name());

        // workaround for StringList
        if (meta->property(i).type() == QVariant::StringList)
        {
            QStringList list = value.toString().split(::separator);
            meta->property(i).write(entity, list);
        }
        // workaround for enums
        else if (!property.write(entity, value) && !value.isNull())
        {
            property.write(entity, value.toInt());
        }
    }
}

} // namespace data_source
} // namespace flt

#ifndef MOCK_SQL_QUERY_H
#define MOCK_SQL_QUERY_H


#include "i_sql_query.h"

// TODO: GMOCK
class MockSqlQuery : public flt::data_source::ISqlQuery
{
public:
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

    void setExpectedQuery(const QString& expectedQuery);
    void addExpectedValues(const QVariantMap& expectedValues);
    void setLastInsertId(const QVariant& lastInsertId);

private:
    QString m_expectedQuery;
    QString m_preparedQuery;
    QVariantMap m_queryValues;
    QList<QVariantMap> m_expectedValues;
    QVariant m_lastInsertId;
};

#endif // MOCK_SQL_QUERY_H

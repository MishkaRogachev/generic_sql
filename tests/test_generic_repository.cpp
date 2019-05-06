#include "test_generic_repository.h"

// Qt
#include <QDebug>
#include <QFile>
#include <QSignalSpy>

// Internal
#include "generic_repository.h"
#include "mock_sql_query.h"
#include "testing_entity.h"

namespace
{
const QString testTable = "test_table";
const QStringList columnNames = { "id", "name", "age", "temperature", "faction" };
} // namespace

void TestGenericRepository::initTestCase()
{
    qRegisterMetaType<TestingEntity::Faction>("TestingEntity::Faction");
}

void TestGenericRepository::testInitRepository()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericRepository<TestingEntity> repository(mock, ::testTable);

    QCOMPARE(repository.tableName(), ::testTable);
    QCOMPARE(repository.columnNames(), QStringList());

    mock->setExpectedQuery("PRAGMA table_info(" + ::testTable + ")");

    for (int i = 0; i < ::columnNames.count(); ++i)
    {
        mock->addExpectedValues({ { "cid", i }, { "name", ::columnNames[i] } });
    }

    QVERIFY(repository.init());
    QCOMPARE(repository.columnNames(), ::columnNames);
}

void TestGenericRepository::testInsertValues()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericTable table(mock, ::testTable);

    mock->setExpectedQuery("INSERT INTO " + ::testTable +
                           " (age, name, surname) VALUES (77, Ivan, Ivanov)");

    QVERIFY(table.insert({ { "age", "77" }, { "name", "Ivan" }, { "surname", "Ivanov" } }));
}

void TestGenericRepository::testInsertEntity()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericRepository<TestingEntity> repository(mock, ::testTable);

    repository.init(::columnNames);

    TestingEntity entity;
    entity.setName("Test Name");
    entity.setAge(13);
    entity.setTemperature(42.1556f);
    entity.setFaction(TestingEntity::Red);

    mock->setExpectedQuery("INSERT INTO " + ::testTable +
                           " (age, faction, name, temperature) VALUES (" +
                           QVariant(entity.age()).toString() + ", " +
                           QVariant::fromValue(entity.faction()).toString() + ", " + entity.name() +
                           ", " + QVariant(entity.temperature()).toString() + ")");
    mock->setLastInsertId(1);

    QCOMPARE(entity.id(), 0);
    QVERIFY(repository.insert(&entity));
    QCOMPARE(entity.id(), 1);
}

void TestGenericRepository::testRead()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericRepository<TestingEntity> repository(mock, ::testTable);

    repository.init(::columnNames);

    TestingEntity entity;
    entity.setId(3);

    mock->setExpectedQuery("SELECT * FROM " + ::testTable + " WHERE id = 3");
    mock->addExpectedValues({ { "name", "Some name" },
                              { "age", 27 },
                              { "temperature", 36.6 },
                              { "faction", TestingEntity::White } });

    QVERIFY(repository.read(&entity));
    QCOMPARE(entity.name(), QString("Some name"));
    QCOMPARE(entity.age(), 27);
    QVERIFY(qFuzzyCompare(entity.temperature(), float(36.6)));
    QCOMPARE(entity.faction(), TestingEntity::White);
}

void TestGenericRepository::testUpdate()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericRepository<TestingEntity> repository(mock, ::testTable);

    repository.init(::columnNames);

    TestingEntity entity;
    entity.setId(2);
    entity.setName("New name");
    entity.setAge(68);
    entity.setTemperature(17.0);
    entity.setFaction(TestingEntity::Green);

    mock->setExpectedQuery("UPDATE " + ::testTable +
                           " SET age = 68, faction = Green, "
                           "name = New name, temperature = 17 "
                           "WHERE id = 2");

    QVERIFY(repository.update(&entity));
}

void TestGenericRepository::testRemove()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericRepository<TestingEntity> repository(mock, ::testTable);

    repository.init(::columnNames);

    TestingEntity entity;
    entity.setId(3);

    mock->setExpectedQuery("DELETE FROM " + ::testTable + " WHERE id = 3");

    QVERIFY(repository.remove(&entity));
    QCOMPARE(entity.id(), 0);
}

void TestGenericRepository::testSelectId()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericRepository<TestingEntity> repository(mock, ::testTable);

    repository.init(::columnNames);

    mock->setExpectedQuery("SELECT id FROM " + ::testTable + " WHERE nothing = everything");
    mock->addExpectedValues({ { "id", 3 } });
    mock->addExpectedValues({ { "id", 5 } });

    QCOMPARE(repository.selectIds(flt::data_source::Condition({ "nothing", "everything" })),
             QList<int>({ 3, 5 }));
}

void TestGenericRepository::testSelect()
{
    MockSqlQuery* mock = new MockSqlQuery();
    flt::data_source::GenericRepository<TestingEntity> repository(mock, ::testTable);

    repository.init(::columnNames);

    QVariantMap expectedMap = { { "name", "Ivan" },
                                { "surname", "Petrov" },
                                { "age", 33 },
                                { "temperature", 36.6 },
                                { "faction", TestingEntity::Red } };

    mock->setExpectedQuery("SELECT " + expectedMap.keys().join(", ") + " FROM " + ::testTable +
                           " WHERE id = 7");
    mock->addExpectedValues(expectedMap);

    QVariantList result = repository.select(7, expectedMap.keys());
    QCOMPARE(result.count(), 1);
    QCOMPARE(result.first().toList(), expectedMap.values());
}

QTEST_GUILESS_MAIN(TestGenericRepository)

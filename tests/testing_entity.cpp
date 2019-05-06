#include "testing_entity.h"

TestingEntity::TestingEntity(QObject* parent) : QObject(parent)
{
}

int TestingEntity::id() const
{
    return m_id;
}

QString TestingEntity::name() const
{
    return m_name;
}

int TestingEntity::age() const
{
    return m_age;
}

float TestingEntity::temperature() const
{
    return m_temperature;
}

TestingEntity::Faction TestingEntity::faction() const
{
    return m_faction;
}

void TestingEntity::setId(int id)
{
    if (m_id == id) return;

    m_id = id;
    emit idChanged(m_id);
}

void TestingEntity::setName(QString name)
{
    if (m_name == name) return;

    m_name = name;
    emit nameChanged(m_name);
}

void TestingEntity::setAge(int age)
{
    if (m_age == age) return;

    m_age = age;
    emit ageChanged(m_age);
}

void TestingEntity::setTemperature(float temperature)
{
    if (qFuzzyCompare(m_temperature, temperature)) return;

    m_temperature = temperature;
    emit temperatureChanged(m_temperature);
}

void TestingEntity::setFaction(TestingEntity::Faction faction)
{
    if (m_faction == faction) return;

    m_faction = faction;
    emit factionChanged(m_faction);
}

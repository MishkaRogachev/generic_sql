#ifndef TESTING_ENTITY_H
#define TESTING_ENTITY_H


#include <QObject>

class TestingEntity : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
    Q_PROPERTY(float temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(Faction faction READ faction WRITE setFaction NOTIFY factionChanged)

public:
    enum Faction
    {
        WithoutFaction,
        Red,
        White,
        Green
    };

    TestingEntity(QObject* parent = nullptr);

    int id() const;
    QString name() const;
    int age() const;
    float temperature() const;
    Faction faction() const;

public slots:
    void setId(int id);
    void setName(QString name);
    void setAge(int age);
    void setTemperature(float temperature);
    void setFaction(Faction faction);

signals:
    void idChanged(int id);
    void nameChanged(QString name);
    void ageChanged(int age);
    void temperatureChanged(float temperature);
    void factionChanged(Faction faction);

private:
    int m_id = 0;
    QString m_name;
    int m_age = 0;
    float m_temperature = 0;
    Faction m_faction = WithoutFaction;

    Q_ENUM(Faction)
};

#endif // TESTING_ENTITY_H

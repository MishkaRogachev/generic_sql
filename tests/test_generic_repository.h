#ifndef TEST_GENERIC_REPOSITORY_H
#define TEST_GENERIC_REPOSITORY_H


#include <QTest>

// TODO: separate on TestGenericTable and TestGenericRepository
class TestGenericRepository : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testInitRepository();
    void testInsertValues();
    void testInsertEntity();
    void testRead();
    void testUpdate();
    void testRemove();
    void testSelectId();
    void testSelect();
};

#endif // TEST_GENERIC_REPOSITORY_H

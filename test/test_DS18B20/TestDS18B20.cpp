#include <QtCore>
#include <QtTest>

#include "DS18B20Helper.h"

class TestDS18B20 : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void test_DS18B20Helper();
        void test_DS18B20Helper_data();
};


/*
 * Datasheet has this table:
 * TEMPERATURE (°C)   DIGITAL OUTPUT (HEX)
 * ----------------------------------------
 *        +125        07D0h
 *         +85*       0550h
 *         +25.0625   0191h
 *         +10.125    00A2h
 *          +0.5      0008h
 *           0        0000h
 *          -0.5      FFF8h
 *         -10.125    FF5Eh
 *         -25.0625   FE6Fh
 *         -55        FC90h
 */
void TestDS18B20::test_DS18B20Helper_data()
{
    QTest::addColumn<double>("value");
    QTest::addColumn<unsigned int>("hex");

    QTest::newRow(" 23.0") <<  23.0 <<  (unsigned int)0x170;

    QTest::newRow("125.0") <<  125.0    << (unsigned int)0x07D0;
    QTest::newRow(" 85.0") <<   85.0    << (unsigned int)0x0550;
    QTest::newRow(" 25.1") <<   25.0625 << (unsigned int)0x0191;
    QTest::newRow(" 10.1") <<   10.125  << (unsigned int)0x00A2;
    QTest::newRow("  0.5") <<    0.5    << (unsigned int)0x0008;
    QTest::newRow("  0.0") <<    0.0    << (unsigned int)0x0000;
    QTest::newRow(" -0.5") <<   -0.5    << (unsigned int)0xFFF8;
    QTest::newRow("-10.1") <<  -10.125  << (unsigned int)0xFF5E;
    QTest::newRow("-25.1") <<  -25.0625 << (unsigned int)0xFE6F;
    QTest::newRow("-55.0") <<  -55.0    << (unsigned int)0xFC90;
}

void TestDS18B20::test_DS18B20Helper()
{
    QFETCH(double,       value);
    QFETCH(unsigned int, hex  );


    double result = DS18B20Helper::convertTemperature((uint16_t)hex);

    QCOMPARE( result, value );
}

QTEST_MAIN(TestDS18B20)
#include "TestDS18B20.moc"

#include <QtCore>
#include <QtTest>

#include "TemperatureSensor.h"
#include "SensorTypes.h"


class TestTemperatureSensor : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void test_Basic();
};

extern double my_global_ds18b20;
extern double my_global_lm34;
extern double my_global_lm35;

void TestTemperatureSensor::test_Basic()
{
    TemperatureSensor sensor[3];

    sensor[0].init(1, SENSOR_DS18B20);
    sensor[1].init(1, SENSOR_LVTS_LM34);
    sensor[2].init(1, SENSOR_LVTS_LM35);

    if( (sensor[0].ds) == NULL)
        QFAIL("Why still null?");
    if( (sensor[0].lvts) != NULL)
        QFAIL("Why not null?");
    
    if( (sensor[1].lvts) == NULL)
        QFAIL("Why still null?");
    if( (sensor[1].ds) != NULL)
        QFAIL("Why not null?");

    if( (sensor[2].lvts) == NULL)
        QFAIL("Why still null?");
    if( (sensor[2].ds) != NULL)
        QFAIL("Why not null?");

    double value;
    my_global_ds18b20 = 18.20;
    QVERIFY( sensor[0].getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );

    my_global_lm34 = 34.34;
    QVERIFY( sensor[1].getTemperature(&value) );
    QCOMPARE( value, my_global_lm34 );

    my_global_lm35 = 35.35;
    QVERIFY( sensor[2].getTemperature(&value) );
    QCOMPARE( value, my_global_lm35 );

    my_global_ds18b20 = -1.00; //Test value, for bas sensor
    QVERIFY( !sensor[0].getTemperature(&value) );
    QCOMPARE( sensor[0].failcnt, (unsigned int)1);

    my_global_ds18b20 = 30.00;
    QVERIFY( sensor[0].getTemperature(&value) );
    QVERIFY( sensor[0].getTemperature(&value) );
    QCOMPARE( sensor[0].failcnt, (unsigned int)0);

    my_global_ds18b20 = -1.00; //Test value, for bas sensor
    QVERIFY( !sensor[0].getTemperature(&value) );
    QVERIFY( !sensor[0].getTemperature(&value) );
    QCOMPARE( sensor[0].failcnt, (unsigned int)2);

    my_global_ds18b20 = 30.00;
    QVERIFY( sensor[0].getTemperature(&value) );
    QCOMPARE( sensor[0].failcnt, (unsigned int)0);
}




QTEST_MAIN(TestTemperatureSensor)
#include "TestTemperatureSensor.moc"

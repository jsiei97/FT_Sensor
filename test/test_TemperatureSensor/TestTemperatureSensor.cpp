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
        void test_AlarmSensor();
        void test_AlarmLow();
        void test_AlarmHigh();

        void test_AlarmAll();
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
}


/**
 * Test the sensor error alarm logic.
 */
void TestTemperatureSensor::test_AlarmSensor()
{
    TemperatureSensor sensor;

    sensor.init(1, SENSOR_DS18B20);

    double value;

    my_global_ds18b20 = 18.20;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );

    //No alarms at this point.
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //But let's get a sensor error alarm
    my_global_ds18b20 = -1.00; //Test value, for bas sensor
    for( int i=0 ; i<15 ; i++ )
    {
        //Read a lot of times.
        QVERIFY( !sensor.getTemperature(&value) );
    }

    //And now we should have a alarm...
    QCOMPARE(sensor.alarmCheck(), SENSOR_ALARM_SENSOR);

    //Ack so we dont get any more
    sensor.alarmAck(SENSOR_ALARM_SENSOR);
    QCOMPARE(sensor.alarmCheck(), SENSOR_ALARM_NO);


    //Then let's have a correct sensor reading again (no alarm)
    my_global_ds18b20 = 30.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //But let's get a sensor error alarm
    my_global_ds18b20 = -1.00; //Test value, for bas sensor
    for( int i=0 ; i<15 ; i++ )
    {
        //Read a lot of times.
        QVERIFY( !sensor.getTemperature(&value) );
    }
    QCOMPARE(sensor.alarmCheck(), SENSOR_ALARM_SENSOR);

    //Then let's have a correct sensor reading again (no alarm)
    my_global_ds18b20 = 30.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //But let's get a sensor error alarm
    my_global_ds18b20 = -1.00; //Test value, for bas sensor
    for( int i=0 ; i<15 ; i++ )
    {
        //Read a lot of times.
        QVERIFY( !sensor.getTemperature(&value) );
    }
    QCOMPARE(sensor.alarmCheck(), SENSOR_ALARM_SENSOR);
    QCOMPARE(sensor.alarmCheck(), SENSOR_ALARM_SENSOR);
    QCOMPARE(sensor.alarmCheck(), SENSOR_ALARM_SENSOR);
    QCOMPARE(sensor.alarmCheck(), SENSOR_ALARM_SENSOR);

    //Then let's have a correct sensor reading again (no alarm)
    my_global_ds18b20 = 30.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
}


/**
 * Test low alarm logic.
 */
void TestTemperatureSensor::test_AlarmLow()
{
    TemperatureSensor sensor;

    sensor.init(1, SENSOR_DS18B20);

    double value;

    //18.20 > 10 = no alarm
    my_global_ds18b20 = 18.20;
    sensor.setAlarmLevels(5.0, true, 10.0, false, 80.0);
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //18.20 < 30 = alarm
    sensor.setAlarmLevels(5.0, true, 30.0, false, 80.0);
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );

    //40 > 30 = no alarm
    my_global_ds18b20 = 40.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //28 > 30-5 = no alarm
    my_global_ds18b20 = 28.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //22 < 30-5 = alarm
    my_global_ds18b20 = 22.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );

    //Ack so I dont get any more alarm at this time!
    sensor.alarmAck(SENSOR_ALARM_LOW);
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //40 > 30 = no alarm
    my_global_ds18b20 = 40.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
}


/**
 * Test high alarm logic.
 */
void TestTemperatureSensor::test_AlarmHigh()
{
    TemperatureSensor sensor;

    sensor.init(1, SENSOR_DS18B20);

    double value;

    //18.20 < 30 = no alarm
    my_global_ds18b20 = 18.20;
    sensor.setAlarmLevels(5.0, false, 10.0, true, 30.0);
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //32 < 30+5 = no alarm
    my_global_ds18b20 = 32.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //40 > 30+5 = alarm
    my_global_ds18b20 = 40.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );

    //32 < 30+5 = alarm since hyst!
    my_global_ds18b20 = 32.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );

    //28 < 30-5 = no alarm
    my_global_ds18b20 = 28.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //40 > 30+5 = alarm
    my_global_ds18b20 = 40.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );

    //Ack so I dont get any more alarm at this time!
    sensor.alarmAck(SENSOR_ALARM_HIGH);
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //20 < 30 = no alarm
    my_global_ds18b20 = 20.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //40 > 30+5 = alarm
    my_global_ds18b20 = 40.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
}


/**
 * Test with all alarm at the same time.
 */
void TestTemperatureSensor::test_AlarmAll()
{
    TemperatureSensor sensor;

    sensor.init(1, SENSOR_DS18B20);

    double value;

    //Active both alarms:
    //low since 80 > 50
    //high since 20 < 50
    my_global_ds18b20 = 50.00;
    sensor.setAlarmLevels(1.0, true, 80.0, true, 20.0);
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );

    //And then have the sensor to break!
    my_global_ds18b20 = -1.0; //test magic
    for( int i=0 ; i<15 ; i++ )
    {
        QVERIFY( !sensor.getTemperature(&value) );
    }

    bool foundAlarmSensor = false;
    bool foundAlarmLow  = false;
    bool foundAlarmHigh = false;

    //Now loop and Ack them...
    int maxCnt = 20;
    SensorAlarmNumber num;
    do
    {
        num = sensor.alarmCheck();
        switch ( num )
        {
            case SENSOR_ALARM_SENSOR:
                foundAlarmSensor = true;
                sensor.alarmAck(num);
                break;
            case SENSOR_ALARM_LOW:
                foundAlarmLow = true;
                sensor.alarmAck(num);
                break;
            case SENSOR_ALARM_HIGH:
                foundAlarmHigh = true;
                sensor.alarmAck(num);
                break;
            default :
                qDebug() << __func__ << __LINE__ << "maxCnt" << maxCnt;
                //QFAIL("There should not be any other...");
                break;
        }
        maxCnt--;
    }
    while( num != SENSOR_ALARM_NO && maxCnt != 0);


    QVERIFY(foundAlarmSensor);
    QVERIFY(foundAlarmLow);
    QVERIFY(foundAlarmHigh);

    if(maxCnt==0)
    {
        QFAIL("MaxCnt == 0");
    }
}


QTEST_MAIN(TestTemperatureSensor)
#include "TestTemperatureSensor.moc"

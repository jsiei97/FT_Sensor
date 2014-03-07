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
        void test_Diff();
        void test_MaxCnt();
        void test_Offset();

        void test_AlarmSensor();
        void test_AlarmLow();
        void test_AlarmHigh();

        void test_AlarmAll();
        void test_Alarm();
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


void TestTemperatureSensor::test_Diff()
{
    TemperatureSensor sensor;

    sensor.init(1, SENSOR_DS18B20);
    sensor.setValueDiff(1.0);

    double value;

    my_global_ds18b20 = 18.20;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );

    //Same value => false
    QVERIFY( !sensor.getTemperature(&value) );

    //0.5 < 1.0 => false
    my_global_ds18b20 += 0.5;
    QVERIFY( !sensor.getTemperature(&value) );

    //1.5 > 1.0 => true
    my_global_ds18b20 += 1.0;
    QVERIFY( sensor.getTemperature(&value) );

    //Same value => false
    QVERIFY( !sensor.getTemperature(&value) );

    //1.5 > 1.0 => true
    my_global_ds18b20 -= 1.5;
    QVERIFY( sensor.getTemperature(&value) );

    //Same value => false
    QVERIFY( !sensor.getTemperature(&value) );
}


/**
 * Test the timeout count so we always get a value even if there is no new data.
 */
void TestTemperatureSensor::test_MaxCnt()
{
    TemperatureSensor sensor;

    sensor.init(1, SENSOR_DS18B20);
    int cntMax = 60;
    sensor.setValueMaxCnt(cntMax);
    sensor.setValueDiff(1.0);

    double value;

    my_global_ds18b20 = 18.20;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );


    //Test with the same value a lot of times.
    for( int k=0 ; k<5 ; k++ )
    {
        //Same value => false
        //but only 60 times
        for( int i=cntMax ; i>0 ; i-- )
        {
            //Here we shall get false back since there is no new data!
            if( sensor.getTemperature(&value) )
            {
                qDebug() << "Error" << k << i;
                QFAIL("We should no fail");
            }
        }

        //Now we shall get a timeout value => true
        QVERIFY( sensor.getTemperature(&value) );
    }

    //Now test that we count correct when value does change!
    for( int k=0 ; k<5 ; k++ )
    {
        //Same value => false
        //but only 60 times
        for( int i=(cntMax/2) ; i>0 ; i-- )
        {
            QVERIFY( !sensor.getTemperature(&value) );
        }

        my_global_ds18b20 += 2.0;
        QVERIFY( sensor.getTemperature(&value) );

        //Same value => false
        //but only 60 times
        for( int i=cntMax ; i>0 ; i-- )
        {
            //Here we shall get false back since there is no new data!
            if( sensor.getTemperature(&value) )
            {
                qDebug() << "Error" << k << i;
                QFAIL("We should no fail");
            }
        }

        //Now we shall get a timeout value => true
        QVERIFY( sensor.getTemperature(&value) );
    }
}


void TestTemperatureSensor::test_Offset()
{
    TemperatureSensor sensor;

    sensor.init(1, SENSOR_DS18B20);
    double offset = 2.0;
    sensor.setValueOffset(offset);

    double value;

    my_global_ds18b20 = 18.20;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20+offset );

    my_global_ds18b20 = 34.20;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20+offset );

    offset = -2.0;
    sensor.setValueOffset(offset);

    my_global_ds18b20 = 24.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20+offset );

    my_global_ds18b20 = -12.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20+offset );
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

    //31 > 30 = no alarm
    my_global_ds18b20 = 31.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //22 < 30 = alarm
    my_global_ds18b20 = 22.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );

    //31 < 30+5 = alarm
    my_global_ds18b20 = 31.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_LOW );

    //36 > 30+5 = alarm
    my_global_ds18b20 = 36.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //29 < 30 = alarm
    my_global_ds18b20 = 29.00;
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

    //29 < 30 = no alarm
    my_global_ds18b20 = 29.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //31 > 30 = alarm
    my_global_ds18b20 = 31.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );

    //29 > 30-5 = alarm since hyst!
    my_global_ds18b20 = 29.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_HIGH );

    //24 < 30-5 = no alarm
    my_global_ds18b20 = 24.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //31 > 30 = alarm
    my_global_ds18b20 = 31.00;
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

    //20 < 30-5 = no alarm
    my_global_ds18b20 = 20.00;
    QVERIFY( sensor.getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );
    QCOMPARE( sensor.alarmCheck(), SENSOR_ALARM_NO );

    //31 > 30 = alarm
    my_global_ds18b20 = 31.00;
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
            case SENSOR_ALARM_NO:
                //no more alarms...
                break;
        }
        maxCnt--;
    }
    while( num != SENSOR_ALARM_NO && maxCnt != 0);

    QVERIFY( (maxCnt>0) );

    QVERIFY(foundAlarmSensor);
    QVERIFY(foundAlarmLow);
    QVERIFY(foundAlarmHigh);

    if(maxCnt==0)
    {
        QFAIL("MaxCnt == 0");
    }
}

void TestTemperatureSensor::test_Alarm()
{
    TemperatureSensor sensor[2];

    sensor[0].init(1, SENSOR_DS18B20);
    sensor[1].init(1, SENSOR_LVTS_LM35);

    sensor[0].setAlarmLevels(1.0, true, 20.0, true, 25.0);
    sensor[1].setAlarmLevels(1.0, true, 20.0, true, 25.0);

    double value;

    //One high and one low
    my_global_ds18b20 = 10.00;
    QVERIFY( sensor[0].getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );

    my_global_lm35 = 30.00;
    QVERIFY( sensor[1].getTemperature(&value) );
    QCOMPARE( value, my_global_lm35 );

    QCOMPARE( sensor[0].alarmCheck(), SENSOR_ALARM_LOW);
    QCOMPARE( sensor[1].alarmCheck(), SENSOR_ALARM_HIGH);

    //Then the other way
    my_global_ds18b20 = 30.00;
    QVERIFY( sensor[0].getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );

    my_global_lm35 = 10.00;
    QVERIFY( sensor[1].getTemperature(&value) );
    QCOMPARE( value, my_global_lm35 );

    QCOMPARE( sensor[0].alarmCheck(), SENSOR_ALARM_HIGH);
    QCOMPARE( sensor[1].alarmCheck(), SENSOR_ALARM_LOW);

    //Then back to normal
    my_global_ds18b20 = 22.00;
    QVERIFY( sensor[0].getTemperature(&value) );
    QCOMPARE( value, my_global_ds18b20 );

    my_global_lm35 = 22.00;
    QVERIFY( sensor[1].getTemperature(&value) );
    QCOMPARE( value, my_global_lm35 );

    QCOMPARE( sensor[0].alarmCheck(), SENSOR_ALARM_NO);
    QCOMPARE( sensor[1].alarmCheck(), SENSOR_ALARM_NO);
}

QTEST_MAIN(TestTemperatureSensor)
#include "TestTemperatureSensor.moc"

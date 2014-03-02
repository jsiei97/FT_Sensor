
#include "TemperatureSensor.h"
#include "SensorTypes.h"

#include <QDebug>


extern double my_global_ds18b20;
extern double my_global_lm34;
extern double my_global_lm35;

int main()
{
    double value;

    //qDebug() << "Test TemperatureSensor";
    TemperatureSensor* sensor = new TemperatureSensor[3];

    sensor[0].init(1, SENSOR_DS18B20);
    sensor[1].init(1, SENSOR_LVTS_LM34);
    sensor[2].init(1, SENSOR_LVTS_LM35);

    my_global_ds18b20 = 8.20;
    my_global_lm34    = 3.34;
    my_global_lm35    = 3.35;

    for( int i=0 ; i<500; i++ )
    {
        my_global_ds18b20 += 0.1;
        my_global_lm34    += 0.1;
        my_global_lm35    += 0.1;

        sensor[0].getTemperature(&value);
        if( value != my_global_ds18b20 )
            qDebug() << __func__ << __LINE__ << value << my_global_ds18b20;

        sensor[1].getTemperature(&value) ;
        if( value != my_global_lm34 )
            qDebug() << value << my_global_lm34;

        sensor[2].getTemperature(&value);
        if( value != my_global_lm35 )
            qDebug() << value << my_global_lm35;

    }

    delete[] sensor;


    TemperatureSensor *s1 = new TemperatureSensor();

    s1->init(1, SENSOR_DS18B20);
    my_global_ds18b20 = 18.20;
    s1->getTemperature(&value);
    //qDebug() << value << my_global_ds18b20;

    delete s1;

    return 0;
}



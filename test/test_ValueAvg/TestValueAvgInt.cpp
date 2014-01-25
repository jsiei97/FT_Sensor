#include <QtCore>
#include <QtTest>

#include "ValueAvgInt.h"

class TestValueAvgInt : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void test_ValueAvgInt();
};

void TestValueAvgInt::test_ValueAvgInt()
{
    ValueAvgInt filter;

    //Avg on the same num will always be the same
    for(int i=0; i<500; i++)
    {
        filter.addValue(10);
    }
    QCOMPARE(10, filter.getValue());

    //Test that reset works
    filter.init();
    for(int i=0; i<10; i++)
    {
        filter.addValue(5);
    }
    QCOMPARE(5, filter.getValue());


    //Test the max and min is removed
    //if there is more than X values
    filter.init();

    filter.addValue(-2);   //min value
    filter.addValue(400); //max value

    filter.addValue(13);
    filter.addValue(14);
    filter.addValue(14);
    filter.addValue(14);
    filter.addValue(15);

    //No 2 and 400
    //13+(14*3)+15=70/5=14
    QCOMPARE(14, filter.getValue());


    //Check that no values is removed if there is
    //less than 7 values
    filter.init();

    filter.addValue(5);
    filter.addValue(8);
    filter.addValue(14);

    //5+8+14=27/3=9
    QCOMPARE(9, filter.getValue());

}

QTEST_MAIN(TestValueAvgInt)
#include "TestValueAvgInt.moc"

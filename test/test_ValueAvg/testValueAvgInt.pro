CONFIG += qtestlib debug
TEMPLATE = app
TARGET = 
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestValueAvgInt.cpp

# Code to test
DEPENDPATH  += ../../FT_Sensor/
INCLUDEPATH += ../../FT_Sensor/
SOURCES += ValueAvgInt.cpp



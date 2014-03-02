CONFIG += qtestlib debug
QT     -= gui
TEMPLATE = app
TARGET =
DEFINES += private=public
DEFINES += protected=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestTemperatureSensor.cpp
SOURCES += stub/DS18B20.cpp stub/LVTS.cpp

# Code to test
DEPENDPATH  += ../../FT_Sensor/
INCLUDEPATH += ../../FT_Sensor/
SOURCES += TemperatureSensor.cpp



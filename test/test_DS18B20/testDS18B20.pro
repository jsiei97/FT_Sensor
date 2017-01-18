CONFIG += qtestlib debug
QT     -= gui
TEMPLATE = app
TARGET =
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestDS18B20.cpp

# Code to test
DEPENDPATH  += ../../FT_Sensor/
INCLUDEPATH += ../../FT_Sensor/
SOURCES += ../../FT_Sensor/DS18B20Helper.cpp



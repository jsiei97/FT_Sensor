CONFIG += qtestlib debug
QT     -= gui
TEMPLATE = app
TARGET =
DEFINES += private=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestLVTS.cpp Arduino.c

# Code to test
DEPENDPATH  += ../../FT_Sensor/
INCLUDEPATH += ../../FT_Sensor/
SOURCES += LVTS.cpp ValueAvgInt.cpp


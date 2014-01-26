#ifndef  __ARDUINO_H
#define  __ARDUINO_H

#ifdef __cplusplus
extern "C"{
#endif

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2


unsigned long millis(void);
int analogRead(unsigned int);

void pinMode(unsigned int, unsigned int);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // __ARDUINO_H

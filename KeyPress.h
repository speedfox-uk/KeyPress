#ifndef KEYPRESS_h
#define KEYPRESS_h
#include "HID.h"

//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class KeyPress_ 
{
private:
    KeyReport _keyReport;
    void sendReport(KeyReport* keys);
    bool isModifier(uint8_t k);
public:
    KeyPress_(void);
    void begin(void);
    void end(void);
    void keyDown(uint8_t k);
    void keyUp(uint8_t k);
    void allUp(void);
};

extern KeyPress_ KeyPress;

#endif

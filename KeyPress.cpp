#include "KeyPress.h"


static const uint8_t _hidReportDescriptor[] PROGMEM = {

    //  Keyboard
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
   
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x73,                    //   LOGICAL_MAXIMUM (115)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION
};

 
KeyPress_::KeyPress_(void){
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
    allUp();
}


void KeyPress_::sendReport(KeyReport* keys)
{
	HID().SendReport(2,keys,sizeof(KeyReport));
}


void KeyPress_::begin(void){

}

void KeyPress_::end(void){

}

bool KeyPress_::isModifier(uint8_t k){
    //0xE0 -> 0xE7
    return((k & 0xE0) == 0xE0 && 8 > (0xE0^k));
}

void KeyPress_::keyUp(uint8_t k){
    if(isModifier(k)){
        _keyReport.modifiers &= ~(1<<(k-0xE0));
    }
    else{
        for (int i=0; i<6; i++) {
            if(k == _keyReport.keys[i])
                _keyReport.keys[i] = 0x00;
        }

    }
    sendReport(&_keyReport);
}

void KeyPress_::keyDown(uint8_t k){

    int firstZero = -1;

    if(isModifier(k)){
        _keyReport.modifiers |= (1<<(k-0xE0));
    }
    else{
        for (int i=0; i<6; i++) {
            if (-1 == firstZero && _keyReport.keys[i] == 0x00) {
                firstZero = i;
            }
            else if(k == _keyReport.keys[i])
                return; 
        }
    
        if(0 > firstZero)
            return;//Nowhere to put the character
        _keyReport.keys[firstZero] = k;
    }

    sendReport(&_keyReport);
}

void KeyPress_::allUp(void){
	_keyReport.keys[0] = 0;
	_keyReport.keys[1] = 0;	
	_keyReport.keys[2] = 0;
	_keyReport.keys[3] = 0;	
	_keyReport.keys[4] = 0;
	_keyReport.keys[5] = 0;	
	_keyReport.modifiers = 0;
	sendReport(&_keyReport);
}


KeyPress_ KeyPress;

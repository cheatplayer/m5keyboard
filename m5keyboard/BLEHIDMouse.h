// 2019/3/28 by cp
#ifndef BLE_HID_MOUSE_H
#define BLE_HID_MOUSE_H

namespace Mouse{
    void clickLeft();
    void clickRight();
    void wheel(uint8_t wheel);
    void move(uint8_t x,uint8_t y);
    void test();
}

#endif
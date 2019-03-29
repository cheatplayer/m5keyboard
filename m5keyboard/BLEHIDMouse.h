// 2019/3/28 by cp
#ifndef BLE_HID_MOUSE_H
#define BLE_HID_MOUSE_H

namespace Mouse{
    void clickLeft();
    void clickRight();
    void wheel(int wheel);
    void move(int x,int y);
    void test();
}

#endif
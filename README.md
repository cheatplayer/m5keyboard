# M5KeyBoard

**M5Stack Bluetooth Keyboard**

### Menu

```
+---------------------------------------------+
|     +-----+    +-----+                      |
|     |STA  |    |AP   |                      |
|     +-----+    +-----+                      |
|     |halt |    |stop |                      |
|     +-----+    +-----+                      |
|     |run  |    |loop |                      |
|     +-----+    +-----+                      |
|     |load |    |find |                      |
|     +-----+    +-----+                      |
|     |save |    |clear|                      |
|     +-----+    +------+    +----+           |
|     |BLE  |    |Client|    |menu|           |
|     +-----+    +------+    +----+           |
+---------------------------------------------+
| ▪ ▪  menu       info       result           |
+-+-+-----------------------------------------+
  | |
  | |  +------+   +------+   +------+
  | |  | Btn  |   | Btn  |   | Btn  |
  | |  |  A   |   |  B   |   |  C   |
  | |  +------+   +------+   +------+
  | |
  | +--->NETWORK
  |      disconnect  RED
  |      AP          BLE
  |      STA start   YELLOW
  |      STA connect GREEN
  |
  +----->BLUETOOTH
         disconnect RED
         connect    BLUE

```

### Keyboard

```
       alt
    +-------+
    |sym    |
    |       |
    |Fn  key|
    +-------+
    
   LCTRL LSHIFT LALT LCMD         RCTRL RSHIFT RALT RCMD
    +-------------------------------------------------+
    |#   |1   |2   |3   |(   |)   |_   |-   |+   |@   |
    |~  q|^  w|&  e|`  r|<  t|>  y|{  u|}  i|[  o|]  p|
    +-------------------------------------------------+
     F1   F2   F3   F4   F5   F6   F7   F8   F9
    +-------------------------------------------------+
    |*   |4   |5   |6   |/   |:   |;   |'   |"   |DEL |
    ||  a|=  s|\  d|%  f|PS g|SL h|CL j|↑  k|INSl|  BS|
    +-------------------------------------------------+
           F10  F11  F12                     time
    +---------------------------------------+----+----+
    |    |7   |8   |9   |?   |!   |,   |.   |    |    |
    |ALT |TABz|HOMx|ENDc|PU v|PD b|←  n|↓  m|→  $|  LF|
    +----+--------------------------------------------+
               ESC    clear mult key
       +------+----+-------------------+----+------+
       |      |    |                   |    |      |
       |    aA|   0|       SPACE       | sym|    Fn|
       +------+----+-------------------+----+------+
```

### HID Script

line split by `\r`

#### basic

`character`
    
#### mult and special key

`>>> KEYCODE OPTIONAL_KEYCODE...`

`::: KEYCODE`

**keycode**

`CTRL` `LCTRL` `RCTRL`

`SHIFT` `LSHIFT` `RSHIFT`

`ALT` `LALT` `RALT`

`CMD` `META` `GUI` `LMETA` `RMETA`

`ENTER` `DEL` `ESC` `CAPS` `SPACE`

`UP` `LEFT` `DOWN` `RIGHT`

`F1` ~ `F12`

**example**

```
>>> CTRL SPACE
>>> SHIFT ALT f
```
```
::: ESC
```

#### delay

`$$$ time`

**example**

`$$$ 1000`


### Refenerce

- https://github.com/nkolban/esp32-snippets
- https://github.com/spacehuhn/wifi_ducky
- https://github.com/mhama/M5StackHIDCtrlAltDel
- https://github.com/Seytonic/Duckduino-microSD

/*
 * 2019/1/19 by cp
 */
const KEYMAP m5keymap[194]={
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0x2a, 0},/* backspace */
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0x28, 0}, /*enter 13*/
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0x2c, 0},          /*   32*/
    {0x1e, KEY_SHIFT},      /* ! */
    {0x34, KEY_SHIFT},      /* " */
    {0x20, KEY_SHIFT},      /* # */
    {0x21, KEY_SHIFT},      /* $ */
    {0x22, KEY_SHIFT},      /* % */
    {0x24, KEY_SHIFT},      /* & */
    {0x34, 0},          /* ' */
    {0x26, KEY_SHIFT},      /* ( */
    {0x27, KEY_SHIFT},      /* ) */
    {0x25, KEY_SHIFT},      /* * */
    {0x2e, KEY_SHIFT},      /* + */
    {0x36, 0},          /* , */
    {0x2d, 0},          /* - */
    {0x37, 0},          /* . */
    {0x38, 0},          /* / */
    {0x27, 0},          /* 0 */
    {0x1e, 0},          /* 1 */
    {0x1f, 0},          /* 2 */
    {0x20, 0},          /* 3 */
    {0x21, 0},          /* 4 */
    {0x22, 0},          /* 5 */
    {0x23, 0},          /* 6 */
    {0x24, 0},          /* 7 */
    {0x25, 0},          /* 8 */
    {0x26, 0},          /* 9 */
    {0x33, KEY_SHIFT},      /* : */
    {0x33, 0},          /* ; */
    {0x36, KEY_SHIFT},      /* < */
    {0x2e, 0},          /* = */
    {0x37, KEY_SHIFT},      /* > */
    {0x38, KEY_SHIFT},      /* ? */
    {0x1f, KEY_SHIFT},      /* @ */
    {0x04, KEY_SHIFT},      /* A */
    {0x05, KEY_SHIFT},      /* B */
    {0x06, KEY_SHIFT},      /* C */
    {0x07, KEY_SHIFT},      /* D */
    {0x08, KEY_SHIFT},      /* E */
    {0x09, KEY_SHIFT},      /* F */
    {0x0a, KEY_SHIFT},      /* G */
    {0x0b, KEY_SHIFT},      /* H */
    {0x0c, KEY_SHIFT},      /* I */
    {0x0d, KEY_SHIFT},      /* J */
    {0x0e, KEY_SHIFT},      /* K */
    {0x0f, KEY_SHIFT},      /* L */
    {0x10, KEY_SHIFT},      /* M */
    {0x11, KEY_SHIFT},      /* N */
    {0x12, KEY_SHIFT},      /* O */
    {0x13, KEY_SHIFT},      /* P */
    {0x14, KEY_SHIFT},      /* Q */
    {0x15, KEY_SHIFT},      /* R */
    {0x16, KEY_SHIFT},      /* S */
    {0x17, KEY_SHIFT},      /* T */
    {0x18, KEY_SHIFT},      /* U */
    {0x19, KEY_SHIFT},      /* V */
    {0x1a, KEY_SHIFT},      /* W */
    {0x1b, KEY_SHIFT},      /* X */
    {0x1c, KEY_SHIFT},      /* Y */
    {0x1d, KEY_SHIFT},      /* Z */
    {0x2f, 0},          /* [ */
    {0x31, 0},          /* \ */
    {0x30, 0},          /* ] */
    {0x23, KEY_SHIFT},      /* ^ */
    {0x2d, KEY_SHIFT},      /* _ */
    {0x35, 0},          /* ` */
    {0x04, 0},          /* a */
    {0x05, 0},          /* b */
    {0x06, 0},          /* c */
    {0x07, 0},          /* d */
    {0x08, 0},          /* e */
    {0x09, 0},          /* f */
    {0x0a, 0},          /* g */
    {0x0b, 0},          /* h */
    {0x0c, 0},          /* i */
    {0x0d, 0},          /* j */
    {0x0e, 0},          /* k */
    {0x0f, 0},          /* l */
    {0x10, 0},          /* m */
    {0x11, 0},          /* n */
    {0x12, 0},          /* o */
    {0x13, 0},          /* p */
    {0x14, 0},          /* q */
    {0x15, 0},          /* r */
    {0x16, 0},          /* s */
    {0x17, 0},          /* t */
    {0x18, 0},          /* u */
    {0x19, 0},          /* v */
    {0x1a, 0},          /* w */
    {0x1b, 0},          /* x */
    {0x1c, 0},          /* y */
    {0x1d, 0},          /* z */
    {0x2f, KEY_SHIFT},      /* { */
    {0x31, KEY_SHIFT},      /* | */
    {0x30, KEY_SHIFT},      /* } */
    {0x35, KEY_SHIFT},      /* ~ */
    {0x4c, 0},              /* DEL 127*/
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},/* q alt */
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},/* p alt */
    {0,0},/* a alt */
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},/* enter alt*/
    {0,0},
    {0,0},/* x alt*/
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},/*$ alt*/
    {0,0},
    {0,0},
    {0,0},/* alt 0 */
    {0,0},/* alt space */
    {0,0},
    {0,0},
    {0,0},
    {0x46, 0},/*gg print 180*/
    {0x47, 0},/*hh scroll lock*/
    {0x39, 0},/*jj caps*/
    {0x52, 0}, /*up */
    {0x49, 0},/*ins */
    {0,0},
    {0x2b, 0}, /*tab */
    {0x4a, 0},/*home */
    {0,0}, /*end */
    {0x4b, 0},/*pu */
    {0x4e, 0},/*pd */
    {0x50, 0},/*left */
    {0x51, 0}, /*down */
    {0x4f, 0},/*right */
};

const uint8_t reportMap[] = {
  USAGE_PAGE(1),      0x01,       // Generic Desktop Ctrls
  USAGE(1),           0x06,       // Keyboard
  COLLECTION(1),      0x01,       // Application
  REPORT_ID(1),   0x01,   // REPORTID
  USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
  USAGE_MINIMUM(1),   0xE0,
  USAGE_MAXIMUM(1),   0xE7,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x01,
  REPORT_SIZE(1),     0x01,       //   1 byte (Modifier)
  REPORT_COUNT(1),    0x08,
  HIDINPUT(1),           0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
  REPORT_COUNT(1),    0x01,       //   1 byte (Reserved)
  REPORT_SIZE(1),     0x08,
  HIDINPUT(1),           0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
  REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
  REPORT_SIZE(1),     0x01,
  USAGE_PAGE(1),      0x08,       //   LEDs
  USAGE_MINIMUM(1),   0x01,       //   Num Lock
  USAGE_MAXIMUM(1),   0x05,       //   Kana
  HIDOUTPUT(1),          0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
  REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
  REPORT_SIZE(1),     0x03,
  HIDOUTPUT(1),          0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
  REPORT_COUNT(1),    0x06,       //   6 bytes (Keys)
  REPORT_SIZE(1),     0x08,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
  USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
  USAGE_MINIMUM(1),   0x00,
  USAGE_MAXIMUM(1),   0x65,
  HIDINPUT(1),           0x00,       //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
  END_COLLECTION(0)
};

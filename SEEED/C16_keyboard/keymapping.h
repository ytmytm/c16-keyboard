
/* Maps are positional in relation to keyboard USB codes.
Place C64 key byte (Chart is found in KEYBIN spreadsheet)
into the equivalent keyboard USB code code position.
For C64 keyboards normally USB code set 2.

Each map has a normal (without shift) and shifted (with shift) array.

In situations where there is a shift difference, add 64 to the C64 key byte. 
This is when a key is shifted on keyboard, but not shifted on the C64 or vise-versa.

Unused USB codes need to have the IGNORE_KEYCODE value in place.(Default 170)

https://deskthority.net/wiki/Scancode#USB_HID_Usage_Codes

https://sites.google.com/site/h2obsession/CBM/C128/keyboard-scan

*/

//  Modified for C16 by Maciej 'YTM/Elysium' Witkowiak

// order of this table is so weird (if you look into C64/C128 keyboard matrix) because
// of the way how original (PS/2) code had MTxxxx lines attached
// it's [AX0..2,AY0..2] (AX*8+AY) but 3 bits of AX and 3 bits of AY are both in reverse order
// there are 6 bits of key address total,
// AX3 is used for logic table gap on MT8812/16 for X6/X7 and for C128 keys (lines X8/X9/X10)
// anything above $00ff is a special modifier or action key

enum C16_Key {
   // C16 8x8 keymap
   KEY_DEL = 0x00           // 0 DEL
  ,KEY_F1 = 0x01            // 1 F1
  ,KEY_POUND = 0x02         // 2 POUND
  ,KEY_F3 = 0x03            // 3 F3
  ,KEY_RETURN = 0x04        // 4 RETURN
  ,KEY_F2 = 0x05            // 5 F2
  ,KEY_HELP = 0x06          // 6 HELP
  ,KEY_AT = 0x07            // 7 @
  ,KEY_9 = 0x08             // 8 9
  ,KEY_M = 0x09             // 9 M
  ,KEY_J = 0x0A             // 10 J
  ,KEY_O = 0x0B             // 11 O
  ,KEY_I = 0x0C             // 12 I
  ,KEY_K = 0x0D             // 13 K
  ,KEY_ZERO = 0x0E          // 14 0
  ,KEY_N = 0x0F             // 15 N
  ,KEY_5 = 0x10             // 16 5
  ,KEY_C = 0x11             // 17 C
  ,KEY_D = 0x12             // 18 D
  ,KEY_T = 0x13             // 19 T
  ,KEY_R = 0x14             // 20 R
  ,KEY_F = 0x15             // 21 F
  ,KEY_6 = 0x16             // 22 6
  ,KEY_X = 0x17             // 23 X
  ,KEY_CRSR_LEFT = 0x18     // 24 CURSOR LEFT
  ,KEY_ESC = 0x19           // 25 ESC
  ,KEY_SEMICOLON = 0x1A     // 26 ;
  ,KEY_PLUS = 0x1B          // 27 +
  ,KEY_STAR = 0x1C          // 28 *
  ,KEY_EQUALS = 0x1D        // 29 =
  ,KEY_CRSR_RIGHT = 0x1E    // 29 CURSOR RIGHT
  ,KEY_SLASH = 0x1F         // 31 /
  ,KEY_3 = 0x20             // 32 3
  ,KEY_Z = 0x21             // 33 Z
  ,KEY_A = 0x22             // 34 A
  ,KEY_E = 0x23             // 35 E
  ,KEY_W = 0x24             // 36 W
  ,KEY_S = 0x25             // 37 S
  ,KEY_4 = 0x26             // 38 4
  ,KEY_LSHIFT = 0x27        // 39 LSHIFT
  ,KEY_CRSR_DOWN = 0x28     // 40 CURSOR DOWN
//  ,KEY_COMMA = 0x29         // 41 ,
  ,KEY_DOT = 0x29           // 41 .
  ,KEY_L = 0x2A             // 42 L
  ,KEY_MINUS = 0x2B         // 43 -
  ,KEY_P = 0x2C             // 44 P
  ,KEY_COLON = 0x2D         // 45 :
  ,KEY_CRSR_UP = 0x2E       // 46 CURSOR UP
//  ,KEY_DOT = 0x2F           // 47 .
  ,KEY_COMMA = 0x2F         // 47 ,
  ,KEY_7 = 0x30             // 48 7
  ,KEY_B = 0x31             // 49 B
  ,KEY_G = 0x32             // 50 G
  ,KEY_U = 0x33             // 51 U
  ,KEY_Y = 0x34             // 52 Y
  ,KEY_H = 0x35             // 53 H
  ,KEY_8 = 0x36             // 54 8
  ,KEY_V = 0x37             // 55 V
  ,KEY_1 = 0x38             // 56 1
  ,KEY_SPACE = 0x39         // 57 SPACE
  ,KEY_CTRL = 0x3A          // 58 CTRL
  ,KEY_Q = 0x3B             // 59 Q
  ,KEY_CLR = 0x3C           // 60 CLR
  ,KEY_CBM = 0x3D           // 61 CBM
  ,KEY_2 = 0x3E             // 62 2
  ,KEY_RUN_STOP = 0x3F      // 63 RUN/STOP

  // modifiers
  ,MOD_SHIFT = 0x100        // virtual shift modifier (add shift on base map, remove shift on shifted map) (this is used as a bit modifier)
  // special actions
  ,KEY_KEYMAP = 0x401       // special action: switch to next keymap
  ,KEY_MT_RESET = 0x402     // special action: reset MT88XX chip
  ,KEY_SHIFT_LOCK = 0x403   // special action: shift lock (left shift toggle)
  //
  ,IGNORE_KEYCODE = 0xFFF   // special: ignore keycode
};

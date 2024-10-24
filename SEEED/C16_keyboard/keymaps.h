
//  C16 key maps: symbolic and positional
//
//  Maciej 'YTM/Elysium' Witkowiak

// mapped key information
constexpr static C64KeyInfo key_infos[] = {
   { KEY_DEL, "INST/DEL" }
  ,{ KEY_F1, "F1" }
  ,{ KEY_POUND, "POUND" }
  ,{ KEY_F3, "F3" }
  ,{ KEY_RETURN, "RETURN" }
  ,{ KEY_F2, "F2" }
  ,{ KEY_HELP, "HELP" }
  ,{ KEY_AT, "@" }
  ,{ KEY_9, "9" }
  ,{ KEY_M, "M" }
  ,{ KEY_J, "J" }
  ,{ KEY_O, "O" }
  ,{ KEY_I, "I" }
  ,{ KEY_K, "K" }
  ,{ KEY_ZERO, "0" }
  ,{ KEY_N, "N" }
  ,{ KEY_5, "5" }
  ,{ KEY_C, "C" }
  ,{ KEY_D, "D" }
  ,{ KEY_T, "T" }
  ,{ KEY_R, "R" }
  ,{ KEY_F, "F" }
  ,{ KEY_6, "6" }
  ,{ KEY_X, "X" }
  ,{ KEY_CRSR_LEFT, "crsr <-" }
  ,{ KEY_ESC, "ESC" }
  ,{ KEY_SEMICOLON, ";" }
  ,{ KEY_PLUS, "+" }
  ,{ KEY_STAR, "*" }
  ,{ KEY_EQUALS, "=" }
  ,{ KEY_CRSR_RIGHT, "crsr ->" }
  ,{ KEY_COMMA, "," }
  ,{ KEY_3, "3" }
  ,{ KEY_Z, "Z" }
  ,{ KEY_A, "A" }
  ,{ KEY_E, "E" }
  ,{ KEY_W, "W" }
  ,{ KEY_S, "S" }
  ,{ KEY_4, "4" }
  ,{ KEY_LSHIFT, "left SHIFT" }
  ,{ KEY_CRSR_DOWN, "crsr v" }
  ,{ KEY_DOT, "." }
  ,{ KEY_L, "L" }
  ,{ KEY_MINUS, "-" }
  ,{ KEY_P, "P" }
  ,{ KEY_COLON, ":" }
  ,{ KEY_CRSR_UP, "crsr ^" }
  ,{ KEY_SLASH, "/" }
  ,{ KEY_7, "7" }
  ,{ KEY_B, "B" }
  ,{ KEY_G, "G" }
  ,{ KEY_U, "U" }
  ,{ KEY_Y, "Y" }
  ,{ KEY_H, "H" }
  ,{ KEY_8, "8" }
  ,{ KEY_V, "V" }
  ,{ KEY_1, "1" }
  ,{ KEY_SPACE, "SPACE" }
  ,{ KEY_CTRL, "CTRL" }
  ,{ KEY_Q, "Q" }
  ,{ KEY_CLR, "CLEAR/HOME" }
  ,{ KEY_CBM, "C=" }
  ,{ KEY_2, "2" }
  ,{ KEY_RUN_STOP, "RUN/STOP" }

  ,{ MOD_SHIFT, "virtual SHIFT" }
  ,{ KEY_KEYMAP, "switch keymap" }
  ,{ KEY_MT_RESET, "reset MT88XX" }
  ,{ KEY_SHIFT_LOCK, "SHIFT LOCK" }
  ,{ IGNORE_KEYCODE, "ignore key" }
};

constexpr static uint16_t key_infos_nsize = sizeof(key_infos) / sizeof(key_infos[0]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//static const uint16_t MODKEYS[8] {KEY_CTRL,KEY_LSHIFT,KEY_RUN_STOP,KEY_CBM,KEY_CTRL,KEY_RSHIFT,KEY_RUN_STOP,0};
// MODKEYS bits
// 0 - 0x01 - left CTRL
// 1 - 0x02 - left SHIFT
// 2 - 0x04 - left ALT
// 3 - 0x08 - left Windows
// 4 - 0x10 - right CTRL
// 5 - 0x20 - right SHIFT(?)
// 6 - 0x40 - right ALT (AltGr)
// 7 - 0x80 - right Windows

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// C16 positional
// c16 positional (left CTRL is C=, left WIN = C= both shifts unchanged, right WIN = right ALT = pound, right CTRL = equals)
constexpr static uint16_t C16_pos_mods[8] = {KEY_CBM, KEY_LSHIFT, IGNORE_KEYCODE, IGNORE_KEYCODE, KEY_EQUALS, KEY_LSHIFT, KEY_POUND, KEY_POUND};
constexpr static C64Key C16_pos_keys[] = {
   { 0x00, IGNORE_KEYCODE, IGNORE_KEYCODE }  // Reserved
  ,{ 0x01, IGNORE_KEYCODE, IGNORE_KEYCODE }  // ErrorRollOver
  ,{ 0x02, IGNORE_KEYCODE, IGNORE_KEYCODE }  // POSTFail
  ,{ 0x03, IGNORE_KEYCODE, IGNORE_KEYCODE }  // ErrorUndefined
  ,{ 0x04, KEY_A,          KEY_A }  // a and A
  ,{ 0x05, KEY_B,          KEY_B }
  ,{ 0x06, KEY_C,          KEY_C }
  ,{ 0x07, KEY_D,          KEY_D }
  ,{ 0x08, KEY_E,          KEY_E }
  ,{ 0x09, KEY_F,          KEY_F }
  ,{ 0x0a, KEY_G,          KEY_G }
  ,{ 0x0b, KEY_H,          KEY_H }
  ,{ 0x0c, KEY_I,          KEY_I }
  ,{ 0x0d, KEY_J,          KEY_J }
  ,{ 0x0e, KEY_K,          KEY_K }
  ,{ 0x0f, KEY_L,          KEY_L }
  ,{ 0x10, KEY_M,          KEY_M }
  ,{ 0x11, KEY_N,          KEY_N }
  ,{ 0x12, KEY_O,          KEY_O }
  ,{ 0x13, KEY_P,          KEY_P }
  ,{ 0x14, KEY_Q,          KEY_Q }
  ,{ 0x15, KEY_R,          KEY_R }
  ,{ 0x16, KEY_S,          KEY_S }
  ,{ 0x17, KEY_T,          KEY_T }
  ,{ 0x18, KEY_U,          KEY_U }
  ,{ 0x19, KEY_V,          KEY_V }
  ,{ 0x1a, KEY_W,          KEY_W }
  ,{ 0x1b, KEY_X,          KEY_X }
  ,{ 0x1c, KEY_Y,          KEY_Y }
  ,{ 0x1d, KEY_Z,          KEY_Z }  // z and Z
  ,{ 0x1e, KEY_1,          KEY_1 }  // 1 and !
  ,{ 0x1f, KEY_2,          KEY_2 }  // 2 and @
  ,{ 0x20, KEY_3,          KEY_3 }  // 3 and #
  ,{ 0x21, KEY_4,          KEY_4 }  // 4 and $
  ,{ 0x22, KEY_5,          KEY_5 }  // 5 and %
  ,{ 0x23, KEY_6,          KEY_6 }  // 6 and ^
  ,{ 0x24, KEY_7,          KEY_7 }  // 7 and &
  ,{ 0x25, KEY_8,          KEY_8 }  // 8 and *
  ,{ 0x26, KEY_9,          KEY_9 }  // 9 and (
  ,{ 0x27, KEY_ZERO,       KEY_ZERO }		  // 0 and )
  ,{ 0x28, KEY_RETURN,     KEY_RETURN }		  // Return
  ,{ 0x29, KEY_RUN_STOP,   KEY_RUN_STOP }  	  // Escape
  ,{ 0x2a, KEY_DEL,        KEY_DEL }          // Delete (Backspace)
  ,{ 0x2b, KEY_CTRL,       KEY_CTRL }         // Tab
  ,{ 0x2c, KEY_SPACE,      KEY_SPACE }        // Spacebar
  ,{ 0x2d, KEY_PLUS,       KEY_PLUS }         // - and _
  ,{ 0x2e, KEY_MINUS,      KEY_MINUS }        // = and +
  ,{ 0x2f, KEY_AT,         KEY_AT }           // [ and {
  ,{ 0x30, KEY_STAR,       KEY_STAR }         // ] and }
  ,{ 0x31, KEY_EQUALS,     KEY_EQUALS }       // \ and |
  ,{ 0x32, KEY_ESC,		   KEY_ESC }          // Int' # and ~
  ,{ 0x33, KEY_COLON,      KEY_COLON }        // ; and :
  ,{ 0x34, KEY_SEMICOLON,  KEY_SEMICOLON }    // ‘ and “
  ,{ 0x35, KEY_ESC,        KEY_ESC }          // Grave Accent and Tilde
  ,{ 0x36, KEY_COMMA,      KEY_COMMA }        // , and <
  ,{ 0x37, KEY_DOT,        KEY_DOT }          // . and >
  ,{ 0x38, KEY_SLASH,      KEY_SLASH }        // / and ?
  ,{ 0x39, KEY_RUN_STOP,   KEY_RUN_STOP }     // Caps Lock
  ,{ 0x3a, KEY_F1,         KEY_F1 }           // F1
  ,{ 0x3b, KEY_F1 | MOD_SHIFT, KEY_F1 }       // F2
  ,{ 0x3c, KEY_F2,         KEY_F2 }           // F3
  ,{ 0x3d, KEY_F2 | MOD_SHIFT,  KEY_F2 }      // F4
  ,{ 0x3e, KEY_F3,         KEY_F3 }           // F5
  ,{ 0x3f, KEY_F3 | MOD_SHIFT, KEY_F3 }       // F6
  ,{ 0x40, KEY_HELP,       KEY_HELP }         // F7
  ,{ 0x41, KEY_HELP | MOD_SHIFT, KEY_HELP }   // F8
  ,{ 0x42, IGNORE_KEYCODE, IGNORE_KEYCODE }   // F9
  ,{ 0x43, IGNORE_KEYCODE, IGNORE_KEYCODE }   // F10
  ,{ 0x44, IGNORE_KEYCODE, IGNORE_KEYCODE }   // F11
  ,{ 0x45, IGNORE_KEYCODE, IGNORE_KEYCODE }   // F12
  ,{ 0x46, KEY_MT_RESET,   KEY_MT_RESET }     // PrintScreen
  ,{ 0x47, KEY_SHIFT_LOCK, KEY_SHIFT_LOCK }   // Scroll Lock
  ,{ 0x48, KEY_KEYMAP,     KEY_KEYMAP }       // Pause
  ,{ 0x49, KEY_POUND,      KEY_POUND }        // Insert
  ,{ 0x4a, KEY_CLR,        KEY_CLR }          // Home
  ,{ 0x4b, KEY_ZERO | MOD_SHIFT, IGNORE_KEYCODE }     // PageUp
  ,{ 0x4c, KEY_DEL,        KEY_DEL }          // Delete(forward)
  ,{ 0x4d, KEY_RUN_STOP,   KEY_RUN_STOP }     // End
  ,{ 0x4e, KEY_EQUALS | MOD_SHIFT, IGNORE_KEYCODE }   // PageDown
  ,{ 0x4f, KEY_CRSR_RIGHT, KEY_CRSR_RIGHT }   // RightArrow
  ,{ 0x50, KEY_CRSR_LEFT,  KEY_CRSR_LEFT }    // LeftArrow
  ,{ 0x51, KEY_CRSR_DOWN,  KEY_CRSR_DOWN }    // DownArrow
  ,{ 0x52, KEY_CRSR_UP,    KEY_CRSR_UP }      // UpArrow
  ,{ 0x53, IGNORE_KEYCODE, IGNORE_KEYCODE }   // Keypad Num Lock and Clear
  ,{ 0x54, KEY_SLASH,      KEY_SLASH }        // Keypad /
  ,{ 0x55, KEY_STAR,       KEY_STAR }         // Keypad *
  ,{ 0x56, KEY_MINUS,      KEY_MINUS }        // Keypad -
  ,{ 0x57, KEY_PLUS,       KEY_PLUS }         // Keypad +
  ,{ 0x58, KEY_RETURN,     KEY_RETURN }       // Keypad ENTER
  ,{ 0x59, KEY_1,  KEY_1 }                    // Keypad 1 and End
  ,{ 0x5a, KEY_2,  KEY_2 }                    // Keypad 2 and Down Arrow
  ,{ 0x5b, KEY_3,  KEY_3 }                    // Keypad 3 and PageDn
  ,{ 0x5c, KEY_4,  KEY_4 }                    // Keypad 4 and Left Arrow
  ,{ 0x5d, KEY_5,  KEY_5 }                    // Keypad 5
  ,{ 0x5e, KEY_6,  KEY_6 }                    // Keypad 6 and Right Arrow
  ,{ 0x5f, KEY_7,  KEY_7 }                    // Keypad 7 and Home
  ,{ 0x60, KEY_8,  KEY_8 }                    // Keypad 8 and Up Arrow
  ,{ 0x61, KEY_9,  KEY_9 }                    // Keypad 9 and PageUp
  ,{ 0x62, KEY_ZERO, KEY_ZERO }               // Keypad 0 and Insert
  ,{ 0x63, KEY_DOT, KEY_DOT }                 // Keypad . and Delete
  ,{ 0x64, KEY_EQUALS, KEY_EQUALS }           // \ and | (non-US keyboard)
  ,{ 0x65, IGNORE_KEYCODE,  IGNORE_KEYCODE }  // Application (Menu)

  // symbolic mapping of extra keys that possibly exist on some keyboards
  ,{ 0x75, KEY_HELP,  KEY_HELP }    // Help

  ,{ 0xb6, KEY_8 | MOD_SHIFT, KEY_8 }         // Keypad (
  ,{ 0xb7, KEY_9 | MOD_SHIFT, KEY_9 }         // Keypad )

  ,{ 0xba, IGNORE_KEYCODE, IGNORE_KEYCODE }   // Keypad Tab
  ,{ 0xbb, KEY_DEL,        KEY_DEL }          // Keypad Backspace
  ,{ 0xbc, KEY_A,          KEY_A }            // Keypad A
  ,{ 0xbd, KEY_B,          KEY_B }            // Keypad B
  ,{ 0xbe, KEY_C,          KEY_C }            // Keypad C
  ,{ 0xbf, KEY_D,          KEY_D }            // Keypad D
  ,{ 0xc0, KEY_E,          KEY_E }            // Keypad E
  ,{ 0xc1, KEY_F,          KEY_F }            // Keypad F

  ,{ 0xc3, KEY_ZERO | MOD_SHIFT, KEY_ZERO }   // Keypad ^
  ,{ 0xc4, KEY_5 | MOD_SHIFT, KEY_5 }         // Keypad %
  ,{ 0xc5, KEY_COMMA | MOD_SHIFT, KEY_COMMA } // Keypad <
  ,{ 0xc6, KEY_DOT | MOD_SHIFT, KEY_DOT }     // Keypad >
  ,{ 0xc7, KEY_6 | MOD_SHIFT, KEY_6 }         // Keypad &

  ,{ 0xc9, KEY_POUND,      KEY_POUND }        // Keypad |

  ,{ 0xcb, KEY_COLON,      KEY_COLON }        // Keypad :
  ,{ 0xcc, KEY_3 | MOD_SHIFT, KEY_3 }         // Keypad #
  ,{ 0xcd, KEY_SPACE,      KEY_SPACE }        // Keypad Space
  ,{ 0xce, KEY_AT,         KEY_AT }           // Keypad @
  ,{ 0xcf, KEY_1 | MOD_SHIFT, KEY_1 }         // Keypad !

  ,{ 0xd7, KEY_PLUS,      KEY_MINUS }         // Keypad +/-

};

constexpr static uint16_t C16_pos_nkeys = sizeof(C16_pos_keys) / sizeof(C16_pos_keys[0]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// C16 symbolic
// c16 symbolic (both controls are CTRL, both shifts unchanged, left/right WIN is CBM, the rest ignored)
constexpr static uint16_t C16_sym_mods[8] = {KEY_CTRL, KEY_LSHIFT, IGNORE_KEYCODE, KEY_CBM, KEY_CTRL, KEY_LSHIFT, IGNORE_KEYCODE, KEY_CBM};
constexpr static C64Key C16_sym_keys[] = {
   { 0x00, IGNORE_KEYCODE, IGNORE_KEYCODE }  // Reserved
  ,{ 0x01, IGNORE_KEYCODE, IGNORE_KEYCODE }  // ErrorRollOver
  ,{ 0x02, IGNORE_KEYCODE, IGNORE_KEYCODE }  // POSTFail
  ,{ 0x03, IGNORE_KEYCODE, IGNORE_KEYCODE }  // ErrorUndefined
  ,{ 0x04, KEY_A,          KEY_A }  // a and A
  ,{ 0x05, KEY_B,          KEY_B }
  ,{ 0x06, KEY_C,          KEY_C }
  ,{ 0x07, KEY_D,          KEY_D }
  ,{ 0x08, KEY_E,          KEY_E }
  ,{ 0x09, KEY_F,          KEY_F }
  ,{ 0x0a, KEY_G,          KEY_G }
  ,{ 0x0b, KEY_H,          KEY_H }
  ,{ 0x0c, KEY_I,          KEY_I }
  ,{ 0x0d, KEY_J,          KEY_J }
  ,{ 0x0e, KEY_K,          KEY_K }
  ,{ 0x0f, KEY_L,          KEY_L }
  ,{ 0x10, KEY_M,          KEY_M }
  ,{ 0x11, KEY_N,          KEY_N }
  ,{ 0x12, KEY_O,          KEY_O }
  ,{ 0x13, KEY_P,          KEY_P }
  ,{ 0x14, KEY_Q,          KEY_Q }
  ,{ 0x15, KEY_R,          KEY_R }
  ,{ 0x16, KEY_S,          KEY_S }
  ,{ 0x17, KEY_T,          KEY_T }
  ,{ 0x18, KEY_U,          KEY_U }
  ,{ 0x19, KEY_V,          KEY_V }
  ,{ 0x1a, KEY_W,          KEY_W }
  ,{ 0x1b, KEY_X,          KEY_X }
  ,{ 0x1c, KEY_Y,          KEY_Y }
  ,{ 0x1d, KEY_Z,          KEY_Z }  // z and Z
  ,{ 0x1e, KEY_1,          KEY_1 }  // 1 and !
  ,{ 0x1f, KEY_2,          KEY_AT | MOD_SHIFT } // 2 and @
  ,{ 0x20, KEY_3,          KEY_3 }  // 3 and #
  ,{ 0x21, KEY_4,          KEY_4 }  // 4 and $
  ,{ 0x22, KEY_5,          KEY_5 }  // 5 and %
  ,{ 0x23, KEY_6,          KEY_ZERO } // 6 and ^
  ,{ 0x24, KEY_7,          KEY_6 }  // 7 and &
  ,{ 0x25, KEY_8,          KEY_STAR | MOD_SHIFT } // 8 and *
  ,{ 0x26, KEY_9,          KEY_8 }  // 9 and (
  ,{ 0x27, KEY_ZERO,       KEY_9 }  // 0 and )
  ,{ 0x28, KEY_RETURN,     KEY_RETURN } // Return
  ,{ 0x29, KEY_ESC,        KEY_ESC }     // Escape
  ,{ 0x2a, KEY_DEL,        KEY_DEL }          // Delete (Backspace)
  ,{ 0x2b, KEY_CTRL,       KEY_CTRL }     // Tab
  ,{ 0x2c, KEY_SPACE,      KEY_SPACE }        // Spacebar
  ,{ 0x2d, KEY_MINUS,      KEY_MINUS }        // - and _
  ,{ 0x2e, KEY_EQUALS,     KEY_PLUS | MOD_SHIFT } // = and +
  ,{ 0x2f, KEY_COLON | MOD_SHIFT, KEY_COLON } // [ and {
  ,{ 0x30, KEY_SEMICOLON | MOD_SHIFT, KEY_SEMICOLON } // ] and }
  ,{ 0x31, KEY_POUND,      KEY_POUND }        // \ and |
  ,{ 0x32, KEY_EQUALS | MOD_SHIFT, IGNORE_KEYCODE }   // Int' # and ~
  ,{ 0x33, KEY_SEMICOLON,  KEY_COLON | MOD_SHIFT } // ; and :
  ,{ 0x34, KEY_7 | MOD_SHIFT,  KEY_2 }        // ‘ and “
  ,{ 0x35, KEY_EQUALS | MOD_SHIFT, IGNORE_KEYCODE }     // Grave Accent and Tilde
  ,{ 0x36, KEY_COMMA,      KEY_COMMA }        // , and <
  ,{ 0x37, KEY_DOT,        KEY_DOT }          // . and >
  ,{ 0x38, KEY_SLASH,      KEY_SLASH }        // / and ?
  ,{ 0x39, KEY_RUN_STOP,   KEY_RUN_STOP }     // Caps Lock
  ,{ 0x3a, KEY_F1,         KEY_F1 }           // F1
  ,{ 0x3b, KEY_F1 | MOD_SHIFT, KEY_F1 }       // F2
  ,{ 0x3c, KEY_F2,         KEY_F2 }           // F3
  ,{ 0x3d, KEY_F2 | MOD_SHIFT,  KEY_F2 }      // F4
  ,{ 0x3e, KEY_F3,         KEY_F3 }           // F5
  ,{ 0x3f, KEY_F3 | MOD_SHIFT, KEY_F3 }       // F6
  ,{ 0x40, KEY_HELP,       KEY_HELP }           // F7
  ,{ 0x41, KEY_HELP | MOD_SHIFT, KEY_HELP }       // F8
  ,{ 0x42, IGNORE_KEYCODE, IGNORE_KEYCODE }    // F9
  ,{ 0x43, IGNORE_KEYCODE, IGNORE_KEYCODE } // F10
  ,{ 0x44, IGNORE_KEYCODE, IGNORE_KEYCODE }   // F11
  ,{ 0x45, IGNORE_KEYCODE, IGNORE_KEYCODE }    // F12
  ,{ 0x46, KEY_MT_RESET,   KEY_MT_RESET }     // PrintScreen
  ,{ 0x47, KEY_SHIFT_LOCK, KEY_SHIFT_LOCK }   // Scroll Lock
  ,{ 0x48, KEY_KEYMAP,     KEY_KEYMAP }       // Pause
  ,{ 0x49, KEY_DEL | MOD_SHIFT, KEY_DEL }     // Insert
  ,{ 0x4a, KEY_CLR,        KEY_CLR }         // Home
  ,{ 0x4b, IGNORE_KEYCODE, IGNORE_KEYCODE }      // PageUp
  ,{ 0x4c, KEY_DEL,        KEY_DEL }          // Delete(forward)
  ,{ 0x4d, KEY_RUN_STOP,   KEY_RUN_STOP }     // End
  ,{ 0x4e, IGNORE_KEYCODE, IGNORE_KEYCODE }   // PageDown
  ,{ 0x4f, KEY_CRSR_RIGHT, KEY_CRSR_RIGHT } // RightArrow
  ,{ 0x50, KEY_CRSR_LEFT,  KEY_CRSR_LEFT } // LeftArrow
  ,{ 0x51, KEY_CRSR_DOWN,  KEY_CRSR_DOWN } // DownArrow
  ,{ 0x52, KEY_CRSR_UP,    KEY_CRSR_UP } // UpArrow
  ,{ 0x53, IGNORE_KEYCODE, IGNORE_KEYCODE }    // Keypad Num Lock and Clear
  ,{ 0x54, KEY_SLASH,      KEY_SLASH }        // Keypad /
  ,{ 0x55, KEY_STAR,       KEY_STAR }         // Keypad *
  ,{ 0x56, KEY_MINUS,      KEY_MINUS }        // Keypad -
  ,{ 0x57, KEY_PLUS,       KEY_PLUS }         // Keypad +
  ,{ 0x58, KEY_RETURN,     KEY_RETURN }       // Keypad ENTER
  ,{ 0x59, KEY_1,  KEY_1 }                    // Keypad 1 and End
  ,{ 0x5a, KEY_2,  KEY_2 }                    // Keypad 2 and Down Arrow
  ,{ 0x5b, KEY_3,  KEY_3 }                    // Keypad 3 and PageDn
  ,{ 0x5c, KEY_4,  KEY_4 }                    // Keypad 4 and Left Arrow
  ,{ 0x5d, KEY_5,  KEY_5 }                    // Keypad 5
  ,{ 0x5e, KEY_6,  KEY_6 }                    // Keypad 6 and Right Arrow
  ,{ 0x5f, KEY_7,  KEY_7 }                    // Keypad 7 and Home
  ,{ 0x60, KEY_8,  KEY_8 }                    // Keypad 8 and Up Arrow
  ,{ 0x61, KEY_9,  KEY_9 }                    // Keypad 9 and PageUp
  ,{ 0x62, KEY_ZERO, KEY_ZERO }               // Keypad 0 and Insert
  ,{ 0x63, KEY_DOT, KEY_DOT }                 // Keypad . and Delete
  ,{ 0x64, KEY_POUND, KEY_POUND }             // \ and | (non-US keyboard)
  ,{ 0x65, IGNORE_KEYCODE,  IGNORE_KEYCODE }  // Application (Menu)

  // symbolic mapping of extra keys that possibly exist on some keyboards
  ,{ 0x75, KEY_HELP,  KEY_HELP }    // Help

  ,{ 0xb6, KEY_8 | MOD_SHIFT, KEY_8 }         // Keypad (
  ,{ 0xb7, KEY_9 | MOD_SHIFT, KEY_9 }         // Keypad )

  ,{ 0xba, IGNORE_KEYCODE,   IGNORE_KEYCODE }     // Keypad Tab
  ,{ 0xbb, KEY_DEL,        KEY_DEL }          // Keypad Backspace
  ,{ 0xbc, KEY_A,          KEY_A }            // Keypad A
  ,{ 0xbd, KEY_B,          KEY_B }            // Keypad B
  ,{ 0xbe, KEY_C,          KEY_C }            // Keypad C
  ,{ 0xbf, KEY_D,          KEY_D }            // Keypad D
  ,{ 0xc0, KEY_E,          KEY_E }            // Keypad E
  ,{ 0xc1, KEY_F,          KEY_F }            // Keypad F

  ,{ 0xc3, KEY_ZERO | MOD_SHIFT, KEY_ZERO }   // Keypad ^
  ,{ 0xc4, KEY_5 | MOD_SHIFT, KEY_5 }         // Keypad %
  ,{ 0xc5, KEY_COMMA | MOD_SHIFT, KEY_COMMA } // Keypad <
  ,{ 0xc6, KEY_DOT | MOD_SHIFT, KEY_DOT }     // Keypad >
  ,{ 0xc7, KEY_6 | MOD_SHIFT, KEY_6 }         // Keypad &

  ,{ 0xc9, KEY_POUND,      KEY_POUND }        // Keypad |

  ,{ 0xcb, KEY_COLON,      KEY_COLON }        // Keypad :
  ,{ 0xcc, KEY_3 | MOD_SHIFT, KEY_3 }         // Keypad #
  ,{ 0xcd, KEY_SPACE,      KEY_SPACE }        // Keypad Space
  ,{ 0xce, KEY_AT,         KEY_AT }           // Keypad @
  ,{ 0xcf, KEY_1 | MOD_SHIFT, KEY_1 }         // Keypad !

  ,{ 0xd7, KEY_PLUS,      KEY_MINUS }         // Keypad +/-

};

constexpr static uint16_t C16_sym_nkeys = sizeof(C16_sym_keys) / sizeof(C16_sym_keys[0]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr static C64KeyMap keymaps[] = {
    C64KeyMap("C16 positional", C16_pos_keys, C16_pos_nkeys, C16_pos_mods )
   ,C64KeyMap("C16 symbolic", C16_sym_keys, C16_sym_nkeys, C16_sym_mods )
};

constexpr static uint8_t nkeymaps = sizeof(keymaps) / sizeof(keymaps[0]);

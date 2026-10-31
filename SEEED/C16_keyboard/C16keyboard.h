/*
  C16keyboard - Commodore Keyboard library

  Copyright (c) 2022 Hartland PC LLC
  Written by Robert VanHazinga

  Modified for C16 by Maciej 'YTM/Elysium' Witkowiak


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/


#ifndef C16keyboard_h
#define C16keyboard_h

#include <array>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//MT88xx control pins
#define ANALOG_SW_DATA      3 // Sets selected cross switch on/off
#define ANALOG_SW_STROBE    6 // Strobe timing pulse
#define ANALOG_SW_RESET     7 // Reset all MT88XX cross switches to off

// I2C DISPLAY
#define SDA_PIN             4 // just for reference, native SEEDUINO XIAO doesn't need these defines
#define SCL_PIN             5

// 74HC595 shift register pins
#define SH_CP_PIN           0 // SRCLK // 11th pin // clock
#define ST_CP_PIN           1 // RCLK  // 12th pin // latch
#define DS_PIN              2 // SER   // 14th pin // data

// Six address bits from the shift register to the MT8808 (8 by 8).
#define ANALOG_SH_AX0     0 // QA (15)
#define ANALOG_SH_AX1     1 // QB (1)
#define ANALOG_SH_AX2     2 // ...
#define ANALOG_SH_AY0     3
#define ANALOG_SH_AY1     4
#define ANALOG_SH_AY2     5
const uint8_t ANALOG_SH_ARRAY[] = { ANALOG_SH_AY2, ANALOG_SH_AY1, ANALOG_SH_AY0, ANALOG_SH_AX2, ANALOG_SH_AX1, ANALOG_SH_AX0 };
// Shift-register outputs QG and QH are unused.

// C16 matrix addresses and adapter actions.
#include "keymapping.h"

class C16KeyInfo {
  public:
    constexpr C16KeyInfo(const uint16_t c, const char* name) :
    m_keycode(c), m_name(name)
    {};
    constexpr auto get_keycode() const { return m_keycode; }
    constexpr const char* get_name() const { return m_name; };
  private:
    const uint16_t m_keycode;
    const char *m_name;
};

// HID boot reports hold at most six ordinary keys and eight modifiers.
struct HeldKey {
    uint8_t usage = 0;
    uint16_t mapped = IGNORE_KEYCODE;
};

class C16keyboard {
  public:
    void begin(void);
    void setKey(uint16_t c, bool kpress, uint8_t kc);
    uint16_t mapkeycode(uint8_t kc);
    void clearAllKeys(void);
    void updateShiftSwitches(void);
    void restoreModifiers(uint8_t modifiers);
    constexpr uint8_t getNumKeyMaps(void);
    const char* getCurrentKeyMapName(void);
};

class C16Key {
  public:
    constexpr C16Key(const int16_t keycode, const uint16_t unshifted, const uint16_t shifted) :
    m_keycode(keycode), m_unshifted(unshifted), m_shifted(shifted)
    {};
    constexpr auto get_keycode() const { return m_keycode; }
    constexpr uint16_t get(bool shifted = false) const {
      return shifted ? m_shifted : m_unshifted;
    }
  private:
    const int16_t m_keycode;
    const uint16_t m_unshifted, m_shifted;
};

using ModifierMap = std::array<uint16_t, 8>;

class C16KeyMap {
  public:
    constexpr C16KeyMap(const char* name, const C16Key *map, const uint16_t mapsize, const ModifierMap &mods) :
    m_name(name), m_map(map), m_mapsize(mapsize), m_mods(mods)
    {};
    constexpr const char* getName() const { return m_name; };
    constexpr const ModifierMap &getMods() const { return m_mods; };
    constexpr C16Key getKey(uint8_t keycode) const {
      for (uint16_t i = 0; i < m_mapsize; ++i) {
        if (m_map[i].get_keycode() == keycode) {
          return m_map[i];
        }
      }
      return C16Key(0, IGNORE_KEYCODE, IGNORE_KEYCODE);
    }
  private:
    const char *m_name;
    const C16Key *m_map;
    const uint16_t m_mapsize;
    const ModifierMap &m_mods;
};

#endif

/*
  C128keyboard - Commodore Keyboard library

  Copyright (c) 2022 Hartland PC LLC
  Written by Robert VanHazinga

  Modified for C128 by Maciej 'YTM/Elysium' Witkowiak


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

#include <map>

#include "C64keyboard.h"

// static keymap definitions, include only once
#include "keymaps.h"

#include <ShiftRegister74HC595.h>
ShiftRegister74HC595<1> sr(DS_PIN, SH_CP_PIN, ST_CP_PIN); // data, clock, latch

void resetMT88(void) {

  digitalWrite(NMI_PIN, LOW);

  digitalWrite(ANALOG_SW_DATA , LOW);
  digitalWrite(ANALOG_SW_RESET, HIGH);
  digitalWrite(ANALOG_SW_STROBE, HIGH);
  delay(20);
  digitalWrite(ANALOG_SW_RESET, LOW);
  digitalWrite(ANALOG_SW_STROBE, LOW);
  digitalWrite(ANALOG_SW_DATA , HIGH);

  lshift = false;
  rshift = false;
  shiftlock = false;
  virtshift = false;

  keymapnum = 0;
  kpress = false;
}


void setswitch(uint8_t c,bool kstate) {
  bool state;

  digitalWrite(ANALOG_SW_DATA , kstate);

  if (c < 64) {
    for (uint8_t i = 0; i<(sizeof(ANALOG_SH_ARRAY)/sizeof(ANALOG_SH_ARRAY[0])); i++) {
      state =  bitRead(c, i);
      sr.setNoUpdate(ANALOG_SH_ARRAY[i], state);
    }
    sr.updateRegisters(); // update the pins to the set values
    digitalWrite( ANALOG_SW_STROBE, HIGH);
    digitalWrite( ANALOG_SW_STROBE, LOW);
  }

  // Reset switch state pin to default high state (Key press).
  //digitalWrite(ANALOG_SW_DATA , HIGH);
}

//////////////////////////////////////////////////////////////

const char* get_mapped_key_name(uint16_t keycode) {
    keycode &= ~MOD_SHIFT;
    for (const auto &info : key_infos) {
        if (info.get_keycode() == keycode) return info.get_name();
    }
    return nullptr;
}

//////////////////////////////////////////////////////////////

// This map will store the original keycode for each pressed key
std::map<uint8_t, uint16_t> keyStateTable;

constexpr uint8_t C128keyboard::getNumKeyMaps(void) { return nkeymaps; };

const char* C128keyboard::getCurrentKeyMapName(void) { return keymaps[keymapnum].getName(); };

uint16_t C128keyboard::mapkeycode(uint8_t kc) {

    // Modifiers arrive as individual HID usages, just like normal keys.
    if (kc >= 0xe0 && kc <= 0xe7) {
      return keymaps[keymapnum].getMods()[kc - 0xe0];
    }

    // scan for any other key
    return keymaps[keymapnum].getKey(kc).get(lshift || rshift || shiftlock || virtshift);
}


void C128keyboard::updateShiftSwitches(void) {
    bool inverted = false;
    for (const auto &entry : keyStateTable) {
        if (entry.second & MOD_SHIFT) inverted = true;
    }
    const bool left = lshift || shiftlock || virtshift;
    setswitch(KEY_LSHIFT, inverted ? !(left || rshift) : left);
    setswitch(KEY_RSHIFT, inverted ? false : rshift);
}

void C128keyboard::c64key(uint16_t c, bool kpress, uint8_t kc) {

    // Resolve releases before any side effects: the active keyboard
    // layout may have changed since this physical key was pressed.
    if (kpress) {
        if (c == IGNORE_KEYCODE || keyStateTable.count(kc)) return;
        keyStateTable[kc] = c;
    } else {
        auto it = keyStateTable.find(kc);
        if (it == keyStateTable.end()) return;
        c = it->second;
        keyStateTable.erase(it);
    }

    // Both AltGr and PageUp can hold RESTORE. Releasing either source must
    // leave NMI asserted until the final source is released.
    if (c == KEY_RESTORE) {
      bool held = false;
      for (const auto &entry : keyStateTable) {
        if (entry.second == KEY_RESTORE) held = true;
      }
      digitalWrite(NMI_PIN, held);
      return;
    }

    lshift = false;
    rshift = false;
    for (const auto &entry : keyStateTable) {
      if (entry.second == KEY_LSHIFT) lshift = true;
      if (entry.second == KEY_RSHIFT) rshift = true;
    }

    const bool updateShifts = c == KEY_LSHIFT || c == KEY_RSHIFT || (c & MOD_SHIFT);

    // Establish Shift before closing a key; release a key before restoring Shift.
    if (updateShifts && kpress) updateShiftSwitches();

    const uint16_t target = c & ~MOD_SHIFT;
    if (target < 64 && target != KEY_LSHIFT && target != KEY_RSHIFT) {
      bool held = false;
      for (const auto &entry : keyStateTable) {
        if ((entry.second & ~MOD_SHIFT) == target) held = true;
      }
      setswitch(target, held);
    }
    if (updateShifts && !kpress) updateShiftSwitches();
}


void C128keyboard::clearAllKeys(void) {
  keyStateTable.clear();
  digitalWrite(NMI_PIN, LOW);
  lshift = false;
  rshift = false;
  shiftlock = false;
  virtshift = false;
  // MT88xx RESET substitute, just go through all the switches in the matrix and clear them
  for (uint8_t c=0;c<64;c++) { // MT8808 has 64 switches (8 by 8)
    setswitch(c,LOW);
  }
}


C128keyboard::C128keyboard() {

  // initialize the pins
  pinMode(ANALOG_SW_STROBE, OUTPUT);  // MT88XX strobe
  pinMode(ANALOG_SW_DATA, OUTPUT);   // MT88XX data
  pinMode(ANALOG_SW_RESET, OUTPUT); // MT88XX reset

  pinMode(NMI_PIN, OUTPUT);

  resetMT88();

}

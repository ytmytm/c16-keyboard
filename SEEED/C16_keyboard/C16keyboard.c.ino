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

#include <algorithm>

#include "C16keyboard.h"

// static keymap definitions, include only once
#include "keymaps.h"

#include <ShiftRegister74HC595.h>
ShiftRegister74HC595<1> sr(DS_PIN, SH_CP_PIN, ST_CP_PIN); // data, clock, latch

void resetMT88(void) {

  digitalWrite(ANALOG_SW_DATA , LOW);
  digitalWrite(ANALOG_SW_RESET, HIGH);
  digitalWrite(ANALOG_SW_STROBE, HIGH);
  delay(20);
  digitalWrite(ANALOG_SW_RESET, LOW);
  digitalWrite(ANALOG_SW_STROBE, LOW);
  digitalWrite(ANALOG_SW_DATA , HIGH);

  shiftHeld = false;
  shiftlock = false;

  keymapnum = 0;
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

}

//////////////////////////////////////////////////////////////

const char* get_mapped_key_name(uint16_t keycode) {
    // Only matrix mappings carry differential Shift. Action codes (including
    // IGNORE_KEYCODE) must retain their complete identity.
    if ((keycode & ~MOD_SHIFT) < 64) keycode &= ~MOD_SHIFT;
    for (const auto &info : key_infos) {
        if (info.get_keycode() == keycode) return info.get_name();
    }
    return nullptr;
}

//////////////////////////////////////////////////////////////

// Saved key-down mappings, with no heap allocation.
std::array<HeldKey, 14> keyStateTable;

constexpr uint8_t C16keyboard::getNumKeyMaps(void) { return nkeymaps; };

const char* C16keyboard::getCurrentKeyMapName(void) { return keymaps[keymapnum].getName(); };

uint16_t C16keyboard::mapkeycode(uint8_t kc) {

    // Modifiers arrive as individual HID usages, just like normal keys.
    if (kc >= 0xe0 && kc <= 0xe7) {
      return keymaps[keymapnum].getMods()[kc - 0xe0];
    }

    // scan for any other key
    return keymaps[keymapnum].getKey(kc).get(shiftHeld || shiftlock);
}


void C16keyboard::updateShiftSwitches(void) {
    bool inverted = false;
    for (const auto &entry : keyStateTable) {
        if (entry.mapped != IGNORE_KEYCODE && (entry.mapped & MOD_SHIFT)) inverted = true;
    }
    const bool shifted = shiftHeld || shiftlock;
    setswitch(KEY_SHIFT, inverted ? !shifted : shifted);
}

void C16keyboard::setKey(uint16_t c, bool kpress, uint8_t kc) {

    // Resolve releases before any side effects: the active keyboard
    // layout may have changed since this physical key was pressed.
    auto slot = std::find_if(keyStateTable.begin(), keyStateTable.end(),
        [kc](const HeldKey &entry) {
            return entry.mapped != IGNORE_KEYCODE && entry.usage == kc;
        });
    if (kpress) {
        if (c == IGNORE_KEYCODE || slot != keyStateTable.end()) return;
        slot = std::find_if(keyStateTable.begin(), keyStateTable.end(),
            [](const HeldKey &entry) { return entry.mapped == IGNORE_KEYCODE; });
        if (slot == keyStateTable.end()) return;
        slot->usage = kc;
        slot->mapped = c;
    } else {
        if (slot == keyStateTable.end()) return;
        c = slot->mapped;
        *slot = HeldKey{};
    }

    shiftHeld = false;
    for (const auto &entry : keyStateTable) {
      if (entry.mapped == KEY_SHIFT) shiftHeld = true;
    }

    const bool updateShifts = c == KEY_SHIFT || (c & MOD_SHIFT);

    // Establish Shift before closing a key; release a key before restoring Shift.
    if (updateShifts && kpress) updateShiftSwitches();

    const uint16_t target = c & ~MOD_SHIFT;
    if (target < 64 && target != KEY_SHIFT) {
      bool held = false;
      for (const auto &entry : keyStateTable) {
        if ((entry.mapped & ~MOD_SHIFT) == target) held = true;
      }
      setswitch(target, held);
    }
    if (updateShifts && !kpress) updateShiftSwitches();
}


void C16keyboard::clearAllKeys(void) {
  keyStateTable.fill(HeldKey{});
  shiftHeld = false;
  shiftlock = false;
  // MT88xx RESET substitute, just go through all the switches in the matrix and clear them
  for (uint8_t c=0;c<64;c++) { // MT8808 has 64 switches (8 by 8)
    setswitch(c,LOW);
  }
}


void C16keyboard::begin(void) {

  // initialize the pins
  pinMode(ANALOG_SW_STROBE, OUTPUT);  // MT88XX strobe
  pinMode(ANALOG_SW_DATA, OUTPUT);   // MT88XX data
  pinMode(ANALOG_SW_RESET, OUTPUT); // MT88XX reset

  resetMT88();
  clearAllKeys();

}

// Print Screen resets to the default layout, but a held USB modifier produces
// no new down event until released. Rebuild those sources from the current mask.
void C16keyboard::restoreModifiers(uint8_t modifiers) {
    for (uint8_t bit = 0; bit < 8; ++bit) {
        if (modifiers & (1U << bit)) {
            const uint8_t usage = 0xe0 + bit;
            setKey(mapkeycode(usage), true, usage);
        }
    }
}

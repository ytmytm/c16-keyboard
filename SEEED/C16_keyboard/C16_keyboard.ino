
// IDE: Seeeduino XIAO SAMD

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

#include "C16keyboard.h"
#include "firmware_version.h"


#include <KeyboardController.h>

#if !defined(C128_USB_HOST_EVENT_API_VERSION) || C128_USB_HOST_EVENT_API_VERSION < 1
#error "Outdated USB host library: reinstall SEEED/C64_USB_Host_Library_SAMD.zip from this checkout in Arduino IDE."
#endif



// Initialize USB Controller
USBHost usb;

// Attach keyboard controller to USB
KeyboardController keyboard(usb);


C16keyboard ckey;
static uint8_t kc, mod;
static uint16_t kc_mapped;
static uint8_t keymapnum = 0; // current keymap number

static bool shiftHeld=false, shiftlock=false; // USB Shift sources and Shift Lock

static unsigned long lastPressedMillis = 0, currentMillis = 0;
constexpr const unsigned long inactivePeriodMillis = 5000; // keep the message on OLED for 5 seconds after a keypress

//////////////////////////////////////////////////////////////
// I2C
#include <Wire.h>

//////////////////////////////////////////////////////////////
// OLED display
// 128x32
// https://github.com/olikraus/u8g2

bool displayPresent = false;

#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0);

void displayState(const char* s, bool showVersion = false);

//////////////////////////////////////////////////////////////

void next_keymap() {
    keymapnum++;
    if (keymapnum == ckey.getNumKeyMaps()) keymapnum = 0;
    displayKeyMap();
}

// This function intercepts key press
void keyPressed() {
  lastPressedMillis = currentMillis;
  kc = keyboard.getOemKey();
  mod = keyboard.getModifiers();
  kc_mapped = ckey.mapkeycode(kc);
  displayKey();

  // handle special key functions
  switch (kc_mapped) {
    case IGNORE_KEYCODE:
      break;
    case KEY_MT_RESET:
      resetMT88();
      ckey.clearAllKeys();
      ckey.restoreModifiers(mod);
      displayState("reset MT");
      break;
    case KEY_KEYMAP:
      next_keymap();
      break;
    case KEY_SHIFT_LOCK:
      shiftlock = !shiftlock;
      ckey.updateShiftSwitches();
      if (shiftlock) {
        displayState("shift lock on");
      } else {
        displayState("shift lock off");
      }
      break;
    default:
      ckey.setKey(kc_mapped, true, kc);
      break;
  }
}

// This function intercepts key release
void keyReleased() {
  lastPressedMillis = currentMillis;
  kc = keyboard.getOemKey();
  mod = keyboard.getModifiers();
  kc_mapped = ckey.mapkeycode(kc);
  displayKey();

  // Always release the mapping saved on key-down.
  ckey.setKey(kc_mapped, false, kc);
}

// Release outputs when USB disappears without key-up reports.
void keyDisconnected() {
  ckey.clearAllKeys();
}

//////////////////////////////////////////////////////////////

void displayKeyMap() {
  if (!displayPresent) { return; }
  u8g2.firstPage();
  do {
   u8g2.setFlipMode(true);
   u8g2.setFont(u8g2_font_7x13B_tr);
   u8g2.setCursor(0,14);
   u8g2.print("New keymap: ");
   u8g2.print(keymapnum);
   u8g2.setCursor(0,31);
   u8g2.print(ckey.getCurrentKeyMapName());
  } while ( u8g2.nextPage() );
}


void displayKey() {
  if (!displayPresent) { return; }
  u8g2.firstPage();
  do {
   u8g2.setFlipMode(true);
   u8g2.setFont(u8g2_font_7x13B_tr);
   u8g2.setCursor(0,14);
   u8g2.print("Key: ");
   u8g2.print(kc, HEX);
   u8g2.print(" Mod: ");
   u8g2.print(mod, HEX);
   u8g2.setCursor(0,31);
   u8g2.print("map:");
   u8g2.print(kc_mapped, HEX);
   u8g2.print(" ");
   const char *name = get_mapped_key_name(kc_mapped);
   u8g2.print(name ? name : "unknown");
  } while ( u8g2.nextPage() );
}


void displayState(const char* s, bool showVersion) {
  if (!displayPresent) { return; }
  u8g2.firstPage();
  do {
   u8g2.setFlipMode(true);
   u8g2.setFont(u8g2_font_7x13B_tr);
   u8g2.setCursor(0,14);
   u8g2.print(s);
   if (showVersion) {
     u8g2.setCursor(0,31);
     u8g2.print("FW ");
     u8g2.print(firmwareVersion);
   }
  } while ( u8g2.nextPage() );
}

//////////////////////////////////////////////////////////////

void setup() {
  ckey.begin();

  Wire.begin();

  // detect if display is connected
  Wire.beginTransmission(0x3C);
  displayPresent = (0 == Wire.endTransmission());

  if (displayPresent) {
    u8g2.begin();
  }

  usb.Init();

  displayState("Hello world!", true);

}

uint32_t lastUSBstate = 0;

void loop() {
  currentMillis = millis();
  if (displayPresent && (currentMillis - lastPressedMillis > inactivePeriodMillis)) {
    // clear screen if there was no activity for inactivePeriodMillis
    u8g2.firstPage();
    do { } while ( u8g2.nextPage() );
    lastPressedMillis = currentMillis; // prevent screen clearing and i2c access in every following loop during inactivity
  }

  // Process USB tasks
  usb.Task();

  uint32_t currentUSBstate = usb.getUsbTaskState();
  const char *state = "";
  if (lastUSBstate != currentUSBstate) {
    switch (currentUSBstate) {
      case USB_ATTACHED_SUBSTATE_SETTLE: state="Device Attached"; break;
      case USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE: state="Detached, waiting for Device"; break;
      case USB_ATTACHED_SUBSTATE_RESET_DEVICE: state="Resetting Device"; break;
      case USB_ATTACHED_SUBSTATE_WAIT_RESET_COMPLETE: state="Reset complete"; break;
      case USB_STATE_CONFIGURING: state="USB Configuring"; break;
      case USB_STATE_RUNNING: state="USB Running"; break;
    }
    displayState(state, true);
    lastUSBstate = currentUSBstate;
  }
}

// Uses actual USB parser, controller, mappings and firmware callbacks with mocked I/O.
void report(uint8_t modifiers, std::initializer_list<uint8_t> keys = {}) {
    uint8_t data[8] = {modifiers, 0};
    unsigned i = 2;
    for (auto key : keys) { assert(i < 8); data[i++] = key; }
    keyboard.Parse(nullptr, 0, 8, data);
}
std::map<uint8_t, uint16_t> savedKeys() {
#if TEST_C16
    std::map<uint8_t, uint16_t> result;
    for (const auto &entry : keyStateTable) {
        if (entry.mapped != IGNORE_KEYCODE) result[entry.usage] = entry.mapped;
    }
    return result;
#else
    return keyStateTable;
#endif
}
void clean() {
    assert(savedKeys().empty());
    assert(!lshift);
#if !TEST_C16
    assert(!rshift);
#endif
    for (const auto &entry : matrix) assert(!entry.second);
}
int main() {
#if TEST_C16
    ckey.begin();
#else
    C128keyboard instance;
    ckey = &instance;
#endif
    displayState("USB Running", true);
    assert(u8g2.frames == 0);
    displayPresent = true;
    displayState("USB Running", true);
    assert(u8g2.lines[1] == std::string("FW ") + firmwareVersion);
    displayPresent = false;
    static_assert(keymaps[0].getKey(4).get() == KEY_A, "constexpr lookup");
    static_assert(keymaps[0].getKey(0xff).get() == IGNORE_KEYCODE, "unknown usage");
    static_assert(keymaps[0].getMods().size() == 8, "eight modifiers");
    assert(std::string(get_mapped_key_name(KEY_A | MOD_SHIFT)) == "A");
    assert(get_mapped_key_name(0xffff) == nullptr);
    for (keymapnum = 0; keymapnum < nkeymaps; ++keymapnum) {
        for (uint8_t bit = 0; bit < 8; ++bit) {
            report(1U << bit);
            assert(kc == 0xe0 + bit && mod == (1U << bit));
            assert(!matrix[KEY_SPACE]);
            report(0);
            clean();
        }
        for (uint8_t shift : {2, 32}) {
            const auto shiftKey = keymaps[keymapnum].getMods()[shift == 2 ? 1 : 5];
            for (bool ctrlFirst : {false, true}) {
                for (bool ctrlReleasedFirst : {false, true}) {
                    report(ctrlFirst ? 1 : shift);
                    report(1 | shift, {4});
                    assert(matrix[shiftKey] && matrix[KEY_A]);
                    report(ctrlReleasedFirst ? shift : 1);
                    assert(matrix[shiftKey] == ctrlReleasedFirst);
                    report(0);
                    clean();
                }
            }
            report(1 | shift, {0x20, 0x2c});
            assert(matrix[KEY_3] && matrix[KEY_SPACE] && matrix[shiftKey]);
            const auto writes = matrixWrites;
            report(1 | shift, {0x2c, 0x20});
            assert(matrixWrites == writes);
            report(0);
            clean();
        }
        report(0xff, {4});
        assert(matrix[KEY_A] && matrix[KEY_LSHIFT]);
        report(0);
        clean();
        // Both USB Shifts remain independent even when C16 shares one matrix output.
        for (bool leftFirst : {false, true}) {
            report(0x22);
            report(leftFirst ? 0x20 : 2);
#if TEST_C16
            assert(lshift && matrix[KEY_LSHIFT]);
#else
            assert(matrix[leftFirst ? KEY_RSHIFT : KEY_LSHIFT]);
#endif
            report(0);
            clean();
        }
        // Differential F2 keeps Shift until the last dependent key is released.
        report(0, {0x3b, 0x3d});
        assert(matrix[KEY_LSHIFT]);
        report(0, {0x3d});
        assert(matrix[KEY_LSHIFT]);
        report(0);
        clean();
        // Establish Shift before a differential key; release the key before Shift.
        const auto target = keymaps[keymapnum].getKey(0x3b).get() & ~MOD_SHIFT;
        captureTransitions = true;
        transitions.clear();
        report(0, {0x3b});
        assert(transitions.front() == std::make_pair(uint8_t(KEY_LSHIFT), true));
        assert(transitions.back() == std::make_pair(uint8_t(target), true));
        transitions.clear();
        report(0);
        assert(transitions.front() == std::make_pair(uint8_t(target), false));
        captureTransitions = false;
        clean();
        // Shift Lock and physical Shift must not release one another.
        report(0, {0x47}); report(0);
        assert(shiftlock && matrix[KEY_LSHIFT]);
        report(2, {4}); report(0);
        assert(matrix[KEY_LSHIFT]);
        report(0, {0x47}); report(0);
        assert(!shiftlock);
        clean();
    }
    keymapnum = 0;
    // Shared matrix aliases: DEL is held by Backspace and Delete.
    keymapnum = 1;
    report(0, {0x2a, 0x4c});
    report(0, {0x4c});
    assert(matrix[KEY_DEL]);
    report(0); clean();
    // Saved modifier release survives a change to a map where it is ignored.
    keymapnum = 1;
    report(8);
    assert(matrix[KEY_CBM]);
    keymapnum = 0;
    report(0); clean();
    // Symbolic Shift+2 uses differential Shift; release either source first.
    keymapnum = 1;
    for (bool shiftFirst : {false, true}) {
        report(2, {0x1f});
        assert(matrix[KEY_AT] && !matrix[KEY_LSHIFT]);
        if (shiftFirst) {
            report(0, {0x1f});
            assert(matrix[KEY_AT] && matrix[KEY_LSHIFT]);
        } else {
            report(2);
            assert(!matrix[KEY_AT] && matrix[KEY_LSHIFT]);
        }
        report(0); clean();
    }
#if TEST_C16
    // PageUp becomes ignored when Shift is held, but its saved press must release.
    keymapnum = 0;
    report(0, {0x4b});
    assert(matrix[KEY_ZERO] && matrix[KEY_LSHIFT]);
    report(2);
    assert(!matrix[KEY_ZERO] && matrix[KEY_LSHIFT]);
    report(0); clean();
    // Both USB Shifts still map shifted symbols after releasing just one.
    keymapnum = 1;
    report(0x22); report(0x20, {0x1f});
    assert(matrix[KEY_AT] && !matrix[KEY_LSHIFT]);
    report(0); clean();
#endif
    keymapnum = 0;
    // Six slots, repeats and invalid reports (length and reserved usages).
    report(3, {4,5,6,7,8,9});
    const auto saved = savedKeys();
    uint8_t bad[8] = {0,0,1};
    keyboard.Parse(nullptr,0,8,bad);
    assert(savedKeys() == saved);
    bad[2] = 4;
    keyboard.Parse(nullptr,0,3,bad);
    assert(savedKeys() == saved);
    report(0); clean();
    ckey->c64key(KEY_A,true,4);
    const auto writes = matrixWrites;
    ckey->c64key(KEY_B,true,4);
    ckey->c64key(KEY_B,false,5);
    assert(matrixWrites == writes);
    ckey->c64key(IGNORE_KEYCODE,false,4);
    clean();
    // Reset while keys are held clears software state as well as the matrix.
    report(3,{4}); report(3,{4,0x46}); report(0); clean();
#if TEST_C16
    // Reset clears ordinary keys and Shift Lock, then restores held modifiers
    // according to the default layout, without waiting for new modifier events.
    for (uint8_t mask : {0, 1, 2, 32, 0x22, 3, 0x10, 0x12, 0x40, 0x80, 0xff}) {
        keymapnum = 1;
        report(0, {0x47}); report(mask, {4});
        report(mask, {4, 0x46});
        assert(keymapnum == 0 && !shiftlock);
        assert(!matrix[KEY_A] && matrix[KEY_LSHIFT] == bool(mask & 0x22));
        for (uint8_t bit = 0; bit < 8; ++bit) {
            const auto mapped = keymaps[0].getMods()[bit];
            if ((mask & (1U << bit)) && mapped != IGNORE_KEYCODE) {
                assert(savedKeys().at(0xe0 + bit) == mapped);
            }
        }
        report(mask, {5}); // no modifier transition; B must use the held modifier mask
        assert(matrix[KEY_B] && matrix[KEY_LSHIFT] == bool(mask & 0x22));
        report(0); clean();
    }
    // Reset with no modifiers clears Shift Lock without inventing a Shift press.
    report(0, {0x47}); report(0, {0x46});
    assert(!shiftlock && !matrix[KEY_LSHIFT]);
    report(0); clean();
    // Shared Shift still survives releasing either USB source after reset.
    for (uint8_t remaining : {2, 32}) {
        report(0x22, {0x46}); report(remaining, {5});
        assert(matrix[KEY_LSHIFT] && matrix[KEY_B]);
        report(0); clean();
    }
    // Full boot reports and all modifier bits fit in the fixed storage. Change
    // the complete key set and modifiers in one report to exercise slot reuse.
    report(0xff, {4, 5, 6, 7, 8, 9});
    assert(savedKeys().size() == 12); // two modifier mappings are ignored
    report(0x22, {10, 11, 12, 13, 14, 15});
    assert(savedKeys().size() == 8);
    assert(matrix[KEY_G] && matrix[KEY_LSHIFT] && !matrix[KEY_A]);
    report(0); clean();
    // The low-level API also handles a full table without overwriting held keys.
    for (uint8_t usage = 1; usage <= 14; ++usage) ckey->c64key(KEY_A, true, usage);
    assert(savedKeys().size() == 14);
    const auto full = savedKeys();
    ckey->c64key(KEY_B, true, 15);
    assert(savedKeys() == full && !matrix[KEY_B]);
    ckey->c64key(IGNORE_KEYCODE, false, 1);
    ckey->c64key(KEY_B, true, 15);
    assert(savedKeys().size() == 14 && matrix[KEY_B]);
    ckey->clearAllKeys(); clean();
    // Action names must survive the differential-Shift mask.
    for (const auto &info : key_infos) {
        assert(std::string(get_mapped_key_name(info.get_keycode())) == info.get_name());
    }
    displayPresent = true;
    kc = 0xe2; mod = 4; kc_mapped = IGNORE_KEYCODE;
    displayKey();
    assert(u8g2.lines[0] == "Key: E2 Mod: 4");
    assert(u8g2.lines[1] == "map:FFF ignore key");
    kc_mapped = 0xffff;
    displayKey();
    assert(u8g2.lines[1] == "map:FFFF unknown");
    keymapnum = 1;
    displayKeyMap();
    assert(u8g2.lines[0] == "New keymap: 1");
    assert(u8g2.lines[1] == "C16 symbolic");
    displayPresent = false;
    keymapnum = 0;
#endif
    // Switching and wrapping keymaps only occurs on the initial key-down.
    for (unsigned i = 0; i < nkeymaps; ++i) {
        report(0,{0x48}); report(0,{0x48});
        assert(keymapnum == (i+1)%nkeymaps);
        report(0); clean();
    }
#if !TEST_C16
    // Shared RESTORE sources do not alias DEL or release each other.
    report(0x40,{0x4b,0x2a});
    assert(pins[NMI_PIN] && matrix[KEY_DEL]);
    report(0,{0x4b});
    assert(pins[NMI_PIN] && !matrix[KEY_DEL]);
    report(0); assert(!pins[NMI_PIN]); clean();
#endif
    HIDBoot<HID_PROTOCOL_KEYBOARD> transport(&usb);
    transport.SetReportParser(0,&keyboard);
    report(0,{0x47}); report(3,{4});
    transport.bPollEnable = true;
    transport.Release();
    assert(!shiftlock);
#if !TEST_C16
    assert(!virtshift);
#endif
    assert(keyboard.getModifiers() == 0 && keyboard.getOemKey() == 0);
    clean();
    const auto afterDisconnect = matrixWrites;
    transport.Release();
    assert(matrixWrites == afterDisconnect);
    transport.bPollEnable = true;
    report(3,{4});
    assert(matrix[KEY_A] && matrix[KEY_LSHIFT]);
    transport.Release(); clean();
    report(0); clean();
    std::cout << "PASS: modifier orders, shared outputs, saved releases, Shift Lock, differential Shift, reset, HID reports, disconnect and reconnect\n";
}

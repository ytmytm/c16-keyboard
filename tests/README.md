# Host regression tests

From the repository root, run:

```sh
python3 tests/test_keyboard_events.py
python3 tests/test_keyboard_events.py C64
python3 -m unittest discover -s tests -p 'test_firmware_artifacts.py' -v
```

Requires Python 3 and g++; no Arduino libraries or connected board are needed.
The event runner compiles the actual parser, controller, mappings and callbacks
as C++14 with warnings as errors and AddressSanitizer/UndefinedBehaviorSanitizer.
It mocks the matrix, GPIO and OLED. It also checks the installable library ZIP
and rejection of outdated USB event APIs. Hardware timing and electrical behavior
still require checking on the adapter.

In environments running under ptrace where LeakSanitizer cannot operate, use
`ASAN_OPTIONS=detect_leaks=0 python3 tests/test_keyboard_events.py` (and C64).
Address and undefined-behavior checks remain enabled.

After USB library changes, run `python3 tools/package_usb_library.py` to regenerate
the deterministic Arduino installation ZIP. CI checks it matches the sources.
The artifact tests validate application vectors, UF2 payloads, addresses, family,
block numbering, corruption detection and release tags without Arduino tools.

Ported from the C128 project through commit `2d320bb`, including the USB event
fixes (`413e4e2`), saved/shared Shift state (`1ca1360`, `90b114b`),
disconnect cleanup (`d08b023`) and matching library package (`fe5d293`).
C16 retains its two existing maps and has one shared Shift matrix switch.

C16-specific checks cover keeping each USB modifier held across Print Screen,
restoring the default layout's modifier mappings, releasing one of two Shifts
after reset, a full fixed-capacity table and reuse of freed entries. The runner
also exercises the real C16 OLED formatting for mapped, ignored and unknown keys.

# Checks in Oxygen

## 2026-11-27: scripted, headless

- **Oxygen:** unreleased development build (newer than 8.3), Linux (Ubuntu, x64), simulated
  hardware, `--no_gui`, driven through its Python scripting. **No view was opened.**
- **ODK:** `22a87435b7fa06a902e5e3d03e142653da8fca1b`. **Plugin:** initial version.
- **Not covered:** a released Oxygen 8.3, Windows, and both QML views.

| Rules | Check | Result |
|---|---|---|
| GAIN-1 | One input: channel created. No input and two inputs: nothing created. CAN channel as input: channel created, no output samples | pass |
| GAIN-2 | Sync input at 10 kS/s, 5001 samples each for gain 2 and -3, offset 10: same times, no mismatches. Async input: all 97 output samples compared had a matching input time and value | pass |
| GAIN-4 | Setting gain to NaN and infinity by script: previous value stays. Views not checked | part |
| GAIN-6 | Input in V: output in V | pass |
| GAIN-9 | Save, remove, load: gain and offset restored, input restored (same channel id and matching samples) | pass |
| GAIN-10 | Input -10..10 V: -10..30 V for gain 2, -20..40 V for gain -3. Gain 0 not checked | part |

## 2026-11-27: Add Channel dialog, by hand

- **Oxygen:** unreleased development build (newer than 8.3), Linux (Ubuntu, x64), simulated
  hardware, with window. Checked by the repository owner.
- **ODK:** `22a87435b7fa06a902e5e3d03e142653da8fca1b`.
- **Not covered yet:** layout details, invalid entries, entered values on the new channel,
  channel details, locked editing, save and load.

| Rules | Check | Result |
|---|---|---|
| GAIN-1 | No channel selected: *Add* can be pressed, nothing is created, and there is no message. One selected: the channel is created | pass |
| GAIN-7 | *Sample Gain* is listed and its settings show in the Add Channel dialog. Values and channel details not checked | part |

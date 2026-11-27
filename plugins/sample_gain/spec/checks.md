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

## 2026-11-27: Add Channel dialog and channel details, by hand

- **Oxygen, ODK:** as in the check above. **Plugin:** with the permanent hint.
- **Checked by:** the repository owner.

| Rules | Check | Result |
|---|---|---|
| GAIN-4 | Add Channel: `abc`, `1,5` and an empty gain, and `abc` as offset, disable *Add* and show an error. Details page not checked | part |
| GAIN-7 | The hint is fully visible and wrapped. Gain 2 and offset 10 create a channel, and its details show 2 and 10. Editing on the details page not checked | part |

## 2026-11-27: channel details, by hand

- **Oxygen, ODK:** as above. **Checked by:** the repository owner.

| Rules | Check | Result |
|---|---|---|
| GAIN-7 | Editing gain on the details page is not possible: the field is read-only. Cause: no min/max range on the setting. Fixed after this check, not yet confirmed | fail |

## 2026-11-27: channel details, locked editing, save and load, by hand

- **Oxygen, ODK:** as above. **Plugin:** with the min/max range on gain and offset.
- **Checked by:** the repository owner, who reported that all listed steps work. What `1,5`
  becomes on the details page was not reported.

| Rules | Check | Result |
|---|---|---|
| GAIN-4 | Details page: after `abc` and an empty gain the previous value returns; `1,5` behaved as expected. With the Add Channel check above and the unit tests | pass |
| GAIN-7 | Details page: gain changed to -3 during data acquisition, the output follows; offset changed to 5. With the Add Channel checks above | pass |
| GAIN-8 | While recording, gain and offset cannot be edited | pass |
| GAIN-9 | Gain -3, offset 5: save the setup, delete the channel, load the setup: gain, offset and input are back | pass |

## 2026-11-27: defaults and output range, scripted, headless

- **Oxygen, ODK:** as above, `--no_gui`. **Plugin:** with the min/max range on gain and offset.

| Rules | Check | Result |
|---|---|---|
| GAIN-3 | New channel without settings: gain 1, offset 0. Gain/offset 0/0, 0/5, -2/-7.5 and 1000000/0 are accepted, no output mismatches on a sync input | pass |
| GAIN-10 | Input -10..10 V. Gain 0: -1..1 V for offset 0, 4..6 V for offset 5. Gain -2, offset -7.5: -27.5..12.5 V | pass |

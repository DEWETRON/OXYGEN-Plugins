# Sample Gain

`output = gain * input + offset`. The sample to start new plugins from.

**Status:** sample. **Maintainer:** to be named. **Behaviour:** [spec/gain.md](spec/gain.md),
[checks in Oxygen](spec/checks.md). The QML views were checked in an unreleased Oxygen build on
Linux; the check log shows what was covered.

| File | What it shows |
|---|---|
| `src/gain.h` | Calculation and validation without ODK code, so it can be tested |
| `src/sample_gain_plugin.cpp` | Manifest, channel setup, settings, processing |
| `qml/SampleGainAddChannel.qml` | Settings in the Add Channel dialog |
| `qml/SampleGainChannelDetails.qml` | Details page of an existing channel |
| `qml.qrc` | QML files packed into the plugin |
| `tests/gain_test.cpp` | Unit tests that name the rules they cover |

The details page is selected by the channel property `ODK/ChannelDetailsPage`, which the public
ODK docs do not describe yet ([decision R4](../../docs/decisions.md)).

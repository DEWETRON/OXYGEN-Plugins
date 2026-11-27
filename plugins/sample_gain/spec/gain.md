# Sample Gain

Applies a gain and an offset to one channel. It is the sample to start a new plugin from.
Oxygen's formula channel can do the same calculation; the point here is the structure.

## Rules

- **GAIN-1** `[done]` Takes exactly one numeric scalar input channel and creates one output
  channel. Without exactly one selected channel, nothing is created. A channel with other data
  delivers no output.
- **GAIN-2** `[done]` `output = gain * input + offset`, with one output sample per input sample
  at the same time, for synchronous and asynchronous inputs.
- **GAIN-3** `[built]` Defaults are gain 1 and offset 0. Any finite number is allowed, including
  zero and negative numbers.
- **GAIN-4** `[done]` A value that is not a finite number is not accepted. In the Add Channel
  dialog the *Add* button is disabled; on an existing channel the previous value stays.
- **GAIN-5** `[done]` NaN and infinite input samples follow normal floating-point arithmetic.
- **GAIN-6** `[done]` The output has the unit of the input. The offset is in that unit.
- **GAIN-7** `[done]` The Add Channel dialog sets gain and offset (the input is selected in the
  dialog's channel list). The dialog always shows the hint "Select exactly one input channel.
  Otherwise, Add creates nothing." The channel details show and edit gain and offset.
- **GAIN-8** `[done]` The details page does not allow editing while Oxygen does not allow
  configuration changes.
- **GAIN-9** `[done]` Saving and loading a setup restores input, gain and offset.
- **GAIN-10** `[built]` The output range is the input range with gain and offset applied. If that
  range is empty (gain 0), it extends by 1 in both directions.

## Acceptance tests

- Unit tests: calculation (GAIN-2), defaults (GAIN-3), validation (GAIN-4), NaN and infinity
  (GAIN-5).
- In Oxygen: the hint is shown and readable in the Add Channel dialog (GAIN-7).
- In Oxygen: create a channel through the Add Channel dialog with gain 2 and offset 10 on a
  channel in V; the output follows, in V (GAIN-1, 2, 6, 7).
- Enter `abc`, `1,5` and an empty value in both views (GAIN-4).
- Change gain on the details page during a measurement (GAIN-7).
- Record, or lock the channel list: the fields cannot be edited (GAIN-8).
- Save the setup, remove the channel, load the setup (GAIN-9).

## Decided

- The output range follows GAIN-10. (Owner, 2026-11-27.)
- Without exactly one selected input, *Add* may still be clickable. A permanent hint explains
  that nothing is created then (GAIN-7), because the Add Channel item has no documented way to
  see the selection. A change in Oxygen would be a separate improvement. (Owner, 2026-11-27.)

## Questions

- From which sample on does a changed gain or offset apply? Does Oxygen recalculate older data,
  for example when it analyses a recorded file?
- What should the user see when the input channel disappears? So far observed: the output stops
  and the input setting is cleared.

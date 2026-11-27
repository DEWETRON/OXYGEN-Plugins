# Specifications

A spec says what a plugin should do, in terms of what the user sees and does. It does not
describe how the code works. Each plugin has its own in `plugins/<name>/spec/`. Start from
[TEMPLATE.md](TEMPLATE.md); [sample_gain](../../plugins/sample_gain/spec/gain.md) is a filled-in
example.

## Rules and status

Every rule has an id with a prefix that is unique to the plugin (`GAIN-1`) and a status:

- `[planned]`: agreed, not built.
- `[built]`: the code exists, but nobody has confirmed the whole rule.
- `[gap]`: the code does something else. The rule says what is right.
- `[done]`: the whole rule is confirmed, by an automated test or a recorded check in Oxygen.

Tests name the rules they cover in a comment (`// GAIN-2`). Checks in Oxygen are recorded in
`spec/checks.md` with date, Oxygen version, operating system, ODK commit, rules and result.
`scripts/check_specs.py` verifies that each `[done]` rule is mentioned in one of these. It cannot
tell whether the test or check is good enough: whoever merges the change has to.

If code or the ODK changes in a way that affects a `[done]` rule, set it back to `[built]` until
it is confirmed again. For a new Oxygen version, the recorded version shows what was tested.

## Process

1. Write the rules as `[planned]`, with acceptance tests.
2. Get the maintainer's agreement.
3. Implement and test.
4. Change statuses in the same pull request as the code.

A small bug fix needs a test that fails without it. Change the spec only if it was wrong or
incomplete.

## Plugin documentation (nice to have)

Documentation for the users of a plugin is welcome but not required: what it does, how to set it
up, what the settings mean, and its limitations. It can be AI generated. Whoever submits it
checks it against the spec and the plugin's real behaviour. It must not present `[planned]`,
`[built]` or `[gap]` behaviour as confirmed. A place for it is `plugins/<name>/docs/`, linked
from the plugin's README. The publication rules apply to it too.

## Questions and decisions

Open points go under **Questions**. Agents may add questions. An entry under **Decided** needs
a decision that the maintainer has actually made, with who and when. Decisions about how a
plugin is built go into `spec/decisions.md`, created when first needed; repository-wide ones
into [../decisions.md](../decisions.md). Mark replaced entries as superseded, do not delete them.

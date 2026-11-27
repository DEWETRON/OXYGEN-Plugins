# AGENTS.md

Rules for coding agents and people working on OXYGEN plugins in this repository.

## This repository is PUBLIC

Everything pushed can be read by anyone and cannot reliably be taken back. This includes code,
specs, test data, commit messages, branch names, issues and pull requests.

- Never add customer names, requirements, data or code without the customer's permission.
- Never add internal DEWETRON code, ticket numbers, hostnames, paths or credentials. Do not copy
  from internal repositories, even if you can read them.
- **If in doubt, do not push.** Ask the person you work for.
- Never push, open an issue or pull request, or publish a release unless that person asked for it.

## Build and test

```sh
git submodule update --init          # once per clone or worktree
cmake --preset linux                 # Windows: cmake --preset windows
cmake --build --preset linux         # one plugin: add --target sample_gain
ctest --preset linux
python3 scripts/check_specs.py
python3 scripts/check_odk.py
```

VS Code is the standard editor (Visual Studio 2022 is the second option): write editor
instructions for VS Code first. See `README.md` for what to install. Built plugins are the `*.plugin` files under `build/`.

## Layout

| Path | Content |
|---|---|
| `plugins/<name>/` | One plugin: `spec/`, `src/`, `qml/`, `tests/`. Self-contained |
| `external/odk/` | The ODK (git submodule). **Read-only** |
| `cmake/` | Shared build setup |
| `docs/spec/` | How specs work, and the template |
| `docs/decisions.md` | Repository-wide decisions |

## Where to look

- **Pattern to follow: `plugins/sample_gain`.** The examples in `external/odk/examples` are only
  for looking up the API. Do not copy their structure or style.
- API: the headers in `external/odk/odk/*/inc`. Manual: `external/odk/docs`.
- New plugin: `python3 scripts/new_plugin.py my_filter --prefix FILT --title "My Filter"`.
  Do not copy a plugin folder by hand: names and the plugin UUID must be unique.

## Rules

1. **Spec before code.** For new or changed behaviour, first write `[planned]` rules and
   acceptance tests in the plugin's spec, and get the maintainer's agreement. See
   `docs/spec/README.md`. A small bug fix needs a test; change the spec only if it was wrong or
   incomplete.
2. **Verify, don't guess.** Open a header before using an API. Use only QML types and
   properties that you can find in `external/odk/docs`, its examples or `plugins/sample_gain`.
   Oxygen's QML modules are not in this repository, so a typo there shows up only in Oxygen.
3. **Never edit `external/odk`** to make a plugin work. If the ODK lacks something or has a bug,
   say so. Do not commit a changed submodule pointer unless the task is an ODK upgrade.
4. **Keep logic testable.** Calculations and validation go into code without ODK includes (see
   `src/gain.h`), with unit tests. C++ owns settings, defaults and validation; QML only shows
   and edits them.
   **Pitfall:** give every editable number a range (`setMinMaxConstraint`). Without one, Oxygen
   shows it as read-only, although scripts can still change it.
5. **Narrow, tested changes.** A bug fix comes with a test that fails without it.
6. **No shared code between plugins** by including another plugin's files.
7. **Report only what ran.** Say what you built and tested, and what you did not. A successful
   build says nothing about the QML views: without a check in Oxygen they are not verified.
8. **`main` changes through pull requests**, with passing CI and a review by another person.

## Done means

- [ ] Spec updated, rule statuses honest (`docs/spec/README.md`)
- [ ] Build and tests pass; both check scripts pass
- [ ] Checks in Oxygen recorded in `spec/checks.md`, or reported as not done
- [ ] Nothing confidential or internal in the change, the commit message or the branch name

# Repository decisions

Each entry gives the decision, the reason, and what would make us revisit it. Add new entries at
the end; mark replaced ones as superseded.

## R1: Public repository

- **Why:** unrestricted CI on standard runners, issue tracking and releases.
- **Consequence:** only publishable content. Confidential customer work needs another place.

## R2: The ODK is a git submodule

- **Why:** one pinned version for all plugins, and its headers and docs are in the tree before
  the first build, where people and agents can read them.
- **Consequence:** needs `git submodule update --init`; ZIP downloads are incomplete.
- **Revisit when:** the submodule causes more trouble than it saves.

## R3: One top-level build, self-contained plugins

- **Why:** one set of commands; the ODK compiles once per build directory.
- **Consequence:** an ODK upgrade affects all plugins at once.

## R4: Plugins may use a custom ChannelDetails page

- **What:** the channel property `ODK/ChannelDetailsPage` names a QML item.
- **Risk:** the public ODK docs do not describe it yet. A later Oxygen version could change it.
- **Revisit when:** the ODK docs describe it (then link them), or Oxygen changes it.

## R5: Visual Studio Code is the standard editor

- **What:** instructions and project settings target VS Code on Windows and Linux. Visual
  Studio 2022 is the second option.
- **Why:** it is free, and coding agents integrate well with it.
- **Consequence:** the build is defined by CMake presets, not by editor project files. Windows
  still builds with the MSVC 2022 compiler.

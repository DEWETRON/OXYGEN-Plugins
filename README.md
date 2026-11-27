# OXYGEN-Plugins

Plugins for [DEWETRON OXYGEN](https://www.dewetron.com/products/oxygen-measurement-software/),
built with the [OXYGEN-SDK](https://github.com/DEWETRON/OXYGEN-SDK) (ODK).

This repository is where plugins are developed, mainly with the help of AI coding agents. It has
a sample plugin to start from, a lightweight specification process, unit tests and CI for
Windows and Linux. It is not a second collection of SDK examples. A plugin that proves useful
may later become part of the standard product.

> **This repository is public.** Do not add customer names, data, requirements or code without
> permission, and nothing internal to a company. If in doubt, do not push.

## Plugins

| Plugin | What it does | Status | Maintainer |
|---|---|---|---|
| [sample_gain](plugins/sample_gain/README.md) | `output = gain * input + offset` | sample | to be named |

Status: **sample** shows how to build a plugin; **experimental** may change or break;
**supported** has a maintainer who handles issues. No status implies a warranty (see `LICENSE`).

## What you need

| | Linux (Ubuntu 24.04 or newer) | Windows |
|---|---|---|
| Editor | Visual Studio Code | Visual Studio Code |
| Compiler | `g++` | *Build Tools for Visual Studio 2022* with "Desktop development with C++" |
| Build tools | `cmake` (3.21 or newer), `ninja-build`, `git` | CMake (3.21 or newer, on the `PATH`), Git |
| Qt resource compiler | `qtbase5-dev` | see below |
| Unit tests | `libboost-test-dev` | Boost headers; set `BOOST_ROOT` to their folder |

[Visual Studio Code](https://code.visualstudio.com/) (VS Code) is the editor this repository is
set up for, on both systems: it is free and coding agents integrate well with it. VS Code does
not include a compiler. On Windows the Build Tools provide it without the Visual Studio IDE.
Check that Microsoft's license terms for them cover your use.

On Windows, download `qt_resource_compiler.zip` from the
[OXYGEN-SDK R6.3.0 release](https://github.com/DEWETRON/OXYGEN-SDK/releases/tag/OXYGEN-SDK-R6.3.0),
unpack it, and set the environment variable `SDK_QT_PATH` to its `3rdparty/qt` folder. A full
Qt 5 installation also works.

## Get the code

Clone with git. A ZIP download does not contain the ODK.

```sh
git clone --recurse-submodules https://github.com/DEWETRON/OXYGEN-Plugins.git
```

## Work with Visual Studio Code

1. **Open the project:** *File > Open Folder*, or run `code OXYGEN-Plugins`.
2. **Install the extensions:** accept the prompt to install the recommended ones, or search for
   them in the Extensions view: *CMake Tools* and *C/C++* (both from Microsoft).
3. **Choose a preset:** open the Command Palette (`Ctrl+Shift+P`) and run
   *CMake: Select Configure Preset*. Pick `linux` or `windows`.
4. **Build and test:** run *CMake: Build* (or press `F7`), then *CMake: Run Tests*.
5. **Git:** use the Source Control view for commits, pushes and pulls. If CMake warns about the
   ODK version, run `git submodule update --init` in the integrated terminal.

On Windows, set `SDK_QT_PATH` and `BOOST_ROOT` before you start VS Code, and restart it after
changing them.

### With a coding agent

Install your agent's extension, for example *Claude Code*, or run the agent in the integrated
terminal. Open the repository's top folder so that it finds the instructions: Claude Code reads
`CLAUDE.md`, which imports [AGENTS.md](AGENTS.md). For another agent, check that it reads
`AGENTS.md`, or ask it to.

A good first request:

> Read AGENTS.md. I want a new plugin that does ... Write the spec first and wait for my OK
> before you change code.

Before you accept the result:

- Read the spec rules. Do they say what you meant?
- Check what the agent actually built, tested and tried in Oxygen.
- Review the changes in the Source Control view. **This repository is public:** look for
  customer or internal information before you commit and push.

## Build from the command line

These commands work in the integrated terminal of VS Code. CI uses the same ones.

```sh
cmake --preset linux          # Windows: cmake --preset windows
cmake --build --preset linux
ctest --preset linux
```

## Second option: Visual Studio 2022

If you prefer the Visual Studio 2022 IDE on Windows, install it with "Desktop development with
C++" instead of the Build Tools. Set the same environment variables, open the folder
(*File > Open > Folder*) and pick the `windows` preset. Commit, push and pull work from its Git
window. The rest of this README applies unchanged.

## Try a plugin in Oxygen

Copy the `*.plugin` file from `build/` into `Dewetron/Oxygen/Plugins` in your user documents
folder, then restart Oxygen. The sample appears in the *Add Channel* dialog under *Samples*.

## Add a plugin

```sh
python3 scripts/new_plugin.py my_filter --prefix FILT --title "My Filter"
```

Then write its spec before changing the code: [docs/spec](docs/spec/README.md).
Coding agents follow [AGENTS.md](AGENTS.md); it is worth reading for people too.

## Contributing and support

Anyone may open an issue. Code comes mainly from DEWETRON colleagues. There is no promise to
review or merge pull requests from others. Changes to `main` go through a pull request with
passing CI. A review by another person is welcome but not required. Releases are approved by
named maintainers.

## License

[MIT](LICENSE). The ODK in `external/odk` has its own license (also MIT).

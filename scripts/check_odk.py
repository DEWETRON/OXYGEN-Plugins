#!/usr/bin/env python3
"""Fail if the recorded external/odk commit is not the approved one (accidental upgrades)."""
import pathlib
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent


def main():
    lines = (ROOT / "scripts" / "odk_version.txt").read_text(encoding="utf-8").splitlines()
    approved = next(line.strip() for line in lines if line.strip() and not line.startswith("#"))
    entry = subprocess.run(["git", "ls-files", "--stage", "--", "external/odk"], cwd=ROOT,
                           check=True, capture_output=True, text=True).stdout.split()
    recorded = entry[1] if len(entry) > 1 else "(none)"
    if recorded != approved:
        print(f"error: external/odk records {recorded}, approved is {approved}.\n"
              "Unintended? Run: git checkout main -- external/odk && git submodule update\n"
              "ODK upgrade? Change scripts/odk_version.txt in the same pull request.")
        return 1
    print("ODK version ok:", approved)
    return 0


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""Create a new plugin as a renamed copy of plugins/sample_gain."""
import argparse
import pathlib
import re
import shutil
import sys
import uuid

ROOT = pathlib.Path(__file__).resolve().parent.parent
SAMPLE = ROOT / "plugins" / "sample_gain"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("name", help="folder and target name, e.g. my_filter")
    parser.add_argument("--prefix", required=True, help="spec rule prefix, e.g. FILT")
    parser.add_argument("--title", required=True, help='name shown in Oxygen, e.g. "My Filter"')
    args = parser.parse_args()

    if not re.fullmatch(r"[a-z][a-z0-9]*(_[a-z0-9]+)*", args.name):
        parser.error("name: use lower case letters, digits and '_'")
    if not re.fullmatch(r"[A-Z][A-Z0-9]*", args.prefix):
        parser.error("--prefix: use upper case letters and digits")
    # the title ends up in C++, XML and Markdown
    if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9 _.-]*", args.title):
        parser.error("--title: use letters, digits, space, '_', '.' and '-'")
    target = ROOT / "plugins" / args.name
    if target.exists():
        parser.error(f"{target} already exists")
    for spec in ROOT.glob("plugins/*/spec/*.md"):
        if re.search(rf"\*\*{args.prefix}-\d", spec.read_text(encoding="utf-8")):
            parser.error(f"prefix {args.prefix} is already used in {spec.relative_to(ROOT)}")

    camel = "".join(part.capitalize() for part in args.name.split("_"))
    upper = args.name.upper()
    # one pass, so that a new name is never replaced again
    names = {"sample_gain": args.name, "SAMPLE_GAIN": upper, "SampleGain": camel,
             "Sample Gain": args.title}
    pattern = re.compile("|".join(map(re.escape, names)) + r"|\bGAIN-(?=\d)")

    def rename(text):
        return pattern.sub(lambda m: names.get(m.group(0), args.prefix + "-"), text)

    shutil.copytree(SAMPLE, target)
    for path in sorted(p for p in target.rglob("*") if p.is_file()):
        text = rename(path.read_text(encoding="utf-8"))
        if path.suffix == ".cpp" and "<OxygenPlugin " in text:
            text = re.sub(r'uuid="[0-9A-F-]{36}"', f'uuid="{str(uuid.uuid4()).upper()}"', text)
            text = text.replace('m_display_group = "Samples"', 'm_display_group = "Plugins"')
        if path.suffix == ".md" and path.parent.name == "spec":
            # nothing is confirmed for the new plugin
            text = text.replace("`[done]`", "`[built]`")
        new_path = path.with_name(rename(path.name))
        path.unlink()
        new_path.write_text(text, encoding="utf-8", newline="\n")

    (target / "spec" / "checks.md").write_text("# Checks in Oxygen\n\nNone yet.\n",
                                               encoding="utf-8", newline="\n")
    (target / "README.md").write_text(
        f"# {args.title}\n\nTODO: what it does.\n\n"
        "**Status:** experimental. **Maintainer:** TODO. **Behaviour:** [spec](spec/).\n",
        encoding="utf-8", newline="\n")

    print(f"Created plugins/{args.name}. Next:\n"
          "  1. Replace the copied rules in spec/ with your own, as [planned].\n"
          "  2. Add the plugin to the table in README.md.\n"
          "  3. It still calculates gain and offset: change code and tests after the spec.")
    return 0


if __name__ == "__main__":
    sys.exit(main())

#!/usr/bin/env python3
"""Check the plugin specs: unique rule ids, valid statuses, evidence for every [done] rule."""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
RULE = re.compile(r"^- \*\*([A-Z][A-Z0-9]*-\d+)\*\*(?: `\[(\w+)\]`)?", re.M)
STATUSES = {"planned", "built", "gap", "done"}
NOT_SPECS = {"checks.md", "decisions.md"}


def read(path):
    return path.read_text(encoding="utf-8")


def evidence(plugin):
    """Rule ids named by tests, or by a passed row in spec/checks.md."""
    text = "".join(read(p) for p in sorted((plugin / "tests").rglob("*.cpp")))
    found = set()
    for comment in re.findall(r"//.*", text):
        found.update(expand(comment))
    checks = plugin / "spec" / "checks.md"
    if checks.exists():
        for line in read(checks).splitlines():
            cells = [c.strip() for c in line.strip().strip("|").split("|")]
            if line.startswith("|") and cells[-1] == "pass":
                found.update(expand(cells[0]))
    return found


def expand(text):
    """'GAIN-1, 2' names GAIN-1 and GAIN-2."""
    ids = []
    for prefix, numbers in re.findall(r"\b([A-Z][A-Z0-9]*)-(\d+(?:, *\d+)*)", text):
        ids += [f"{prefix}-{n.strip()}" for n in numbers.split(",")]
    return ids


def main():
    errors = []
    owner = {}
    plugins = sorted(p.parent for p in ROOT.glob("plugins/*/CMakeLists.txt"))
    for plugin in plugins:
        specs = [p for p in sorted((plugin / "spec").glob("*.md")) if p.name not in NOT_SPECS]
        rules = [(spec, m) for spec in specs for m in RULE.finditer(read(spec))]
        if not rules:
            errors.append(f"{plugin.name}: no spec rules in spec/")
        proof = evidence(plugin)
        for spec, match in rules:
            rule, status = match.groups()
            where = f"{spec.relative_to(ROOT)}: {rule}"
            if rule in owner:
                errors.append(f"{where}: id already used in {owner[rule]}")
            owner[rule] = plugin.name
            if status not in STATUSES:
                errors.append(f"{where}: needs a status {sorted(STATUSES)}")
            elif status == "done" and rule not in proof:
                errors.append(f"{where}: [done], but no test comment or passed check names it")
    prefixes = {}
    for rule, plugin in owner.items():
        prefixes.setdefault(rule.split("-")[0], set()).add(plugin)
    errors += [f"prefix {p} is used by several plugins: {sorted(v)}"
               for p, v in prefixes.items() if len(v) > 1]

    for error in errors:
        print("error:", error)
    print(f"{len(plugins)} plugin(s), {len(owner)} rule(s), {len(errors)} error(s)")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())

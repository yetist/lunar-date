#!/usr/bin/python3
# -*- encoding:utf-8 -*-
# FileName: release.py
# SPDX-License-Identifier: GPL-2.0-or-later

__author__ = "yetist"
__copyright__ = "Copyright (c) 2025 yetist <yetist@gmail.com>"
__license__ = "MIT"

import os
import sys
import subprocess
import shlex


def get_git_log(repo_name: str, old_version: str, new_version: str):
    lines = subprocess.getoutput(
        f'git log --pretty="- %s" {old_version}..{new_version}'
    ).splitlines()
    lines.insert(0, f"### {repo_name} {new_version}")
    lines.insert(1, "")
    return lines


def get_news_log(repo_name: str, old_version: str, new_version: str):
    lines = []
    if not os.path.isfile("NEWS"):
        print('"NEWS" file lost.')
        return lines
    old = f"{repo_name} {old_version}"
    new = f"{repo_name} {new_version}"

    found = False
    data = open("NEWS").read().splitlines()
    for i in data:
        line = i.strip()
        if line.startswith("##") and line.endswith(new):
            found = True
    if not found:
        print(f'forgot to update the "NEWS" file for {repo_name}-{new}?')
        return lines

    found = False
    for i in data:
        line = i.strip()
        if line.startswith("##") and line.endswith(new):
            found = True
        if line.startswith("##") and line.endswith(old):
            found = False
        if found:
            lines.append(line)
    return lines


def main():
    old_tag = subprocess.getoutput(
        "gh release ls -L 1 --json tagName --jq '.[0].tagName'"
    ).strip()
    old_version = old_tag[1:]
    new_tag = os.getenv("GITHUB_REF_NAME", "")
    if len(new_tag.strip()) == 0:
        print("no tag")
        sys.exit(1)
    if old_tag == new_tag:
        print(f"{new_tag} already releaed")
        sys.exit(1)
    new_version = new_tag[1:]

    repo = os.getenv("GITHUB_REPOSITORY", "")
    repo_name = repo.split("/")[1]

    logs = get_news_log(repo_name, old_version, new_version)
    if not logs:
        logs = get_git_log(repo_name, old_version, new_version)

    logs.insert(
        0,
        f"Changes since the last release: https://github.com/{repo}/compare/{old_tag}...{new_tag}",
    )
    logs.insert(1, "")

    body = "\n".join(logs)

    with open(".release.note.txt", "w") as f:
        f.write(body)

    title = f"{repo_name} {new_version}"

    # release version meson dist
    cmdline = f'gh release create {new_tag} --title "{title}" -F .release.note.txt _build/meson-dist/*'
    x = subprocess.run(shlex.split(cmdline))
    if x.returncode != 0:
        sys.exit(1)


if __name__ == "__main__":
    main()

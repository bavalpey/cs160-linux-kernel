#!/bin/bash

# This is the tag that changes are based off of
TAG=cs160-base
# The path to the linux directory
TARGET_DIR=linux-5.10.252
# You should change lastname-firstname.patch to your own name. (Or you can rename it later)
DEST=lastname-firstname.patch


# Start the patch with a diff of the target directory against tracked files
git diff $TAG -- $TARGET_DIR > $DEST
# Then add diffs for untracked files (--others) to ensure any files that were added
# but not added to version control are included in the patch
git ls-files --others --exclude-standard -- $TARGET_DIR | while IFS= read -r f; do
  # numstat shows "-   -" for binary diffs. We don't want to include new binaries
  # that may have been added but not .gitignored
  if git diff --no-index --numstat /dev/null "$f" | grep -q '^-'; then
    continue
  fi
  # Append the diff of the untracked file to the patch. We use --no-index to compare
  # the untracked file against an empty file
  # /dev/null is used as the "old" file since it is empty.
  git diff --no-index /dev/null "$f" >> $DEST
done
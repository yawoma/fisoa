#! bin/bash

# Save the current uncommitted works
git stash

# Fetch and update submodules to the latest commit on their respective tags
git submodules updates --remote

# Iterate over each submodule and ensure it's on the correct tag
git submodule foreach --recursive 'git describe --tags --abbrev=0'

# Commit the updates
git add .
git commit -m "Updated submodules to latest commits"

# Reapplies the stashed works
git stash pop
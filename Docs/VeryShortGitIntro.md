### Manual: https://githubtraining.github.io/training-manual/book.pdf
# Install git
$ Depend on OS...

# Check instalation
$ git --version

# (Optional) Config name and email, /--global/ means for your account (so all repositories, branches, ..)
$ git config --global user.name "First Last"
$ git config --global user.email "you@email.com"

# Check configuration
$ git config --list

# Often used command (just for checking)
$ git status

# Clone remote repository by https ( /Clone/ button in main page of repository) /--recursive/ download all submodules needed
$ git clone --recursive https://gitlab.fel.cvut.cz/i3t-diplomky/i3t-bunny.git

### Now change your directory to downloaded repo

# Check all local branches /-a/ means include read-only copies of remote brances (red) - the green one (with *) is the one you actually working in
$ git branch -a 

# Create new local branch (or start working on existing branch - see below)
$ git branch <name-of-local-branch>
### So create /feature/yourbranch/
$ git branch <feature/yourSuperFeature>

# Start working on existing remote branch
# 1) fetch and create local copy of /develop/ branch too is recommended - it is branch where all feature of I3T-bunny is colected
$ git checkout develop
# 2) fetch remote branch /<name-of-branch>/ and create local copy of it
$ git checkout <name-of-branch>
# 3) Synchronize your branche with /develop/ (asuming you are in <your-branch>)
$ git merge develop

# Switch to some branche (so switch to your)
$ git checkout <your-branch>

### Doing some changes - add files, change files -> this files are in /working tree/

# Move selected changes from /working tree/ to /staging tree/ -> represent unit of work
$ git add <file-to-move>
# Move all changes
$ git add --all
# Move all changes, but no removal files changes
$ git add --no-all

# Save /staging tree/ to history (take snapshot)
git commit -m "<short message describing change>"

### No you have your own local version of project on local git (beside of all other branches)

# Push your version (branch given by name) to remote repository (if branch not exist on remote it will create it). /origin/ is "hidden" address of repository - see $ git config --list
$ git push -u origin <branch-to-push>

### Now you go to sleep, but your colaborators work really hard, so some changes on remote repository (and your branch) were made during your rest

# Get changes from remote - do it for <your-branch> and for synchronizing with others colaborators do it for /develop/ - example is for /develop/
# 1) Switch to branch you want to synchronize with remote (main branch is sometimes /master/ - but in I3T /master/ is production version and /develop/ is the branch where all feature are collected)
$ git checkout develop
# 2) Fetch changes from remote
$ git fetch
# 3) Merge fetched-changes with local
$ git merge
# Or 2) and 3) together
# git pull 

### And now (often) disaster comes - merge conflict -> two different changes on same file

# Check where conflict happen
$ git status

### Open that file in editor and check conflict markers /<<<<<<<, =======, >>>>>>/. Select (rewrite) file to form you like (or ask someone who now) and want to keep
# Add, commit and push change (see above) -> conflict resolved!

# Synchronize some branch with other ( /develop/ ) branch -> you will do it at every begining of workday (maybe some shortcut exist, but this is hopefully clear...)
$ git checkout develop
$ git pull
$ git checkout <your-branch>
$ git pull
$ git merge develop

# And push your work to remote -> you will do it at every end of workday
$ git push -u origin <your-branch>


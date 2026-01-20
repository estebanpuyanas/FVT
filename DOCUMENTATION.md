# F[ile] V[ersioning] T[racker] Docs

Some quick yap about the project here. TBD exactly what I'll say, will do after re-doing the README.

## Pre-Command Structures:

### Command Registry:

- [Header file](include/fvt/command_registry.h).
- [Source file](src/command_registry.cpp).

### Command Parsing:

- [Header file](include/fvt/command_parser.h).
- [Source file](src/command_parser.cpp).

## Commands:

### `init` Command:

- [Header file](include/fvt/init.h).
- [Source file](src/init.cpp).

Similar to Git, this command will intialize an FVT repository.

Usage:

```sh
$ fvt init `repo_path` `"repo_name"`
```

> [!NOTE]
> Excluding the `repo_path` flag will result in the initialization of the repository at the current directory.
> The `repo_name` argument must be enclosed within quotes (single/double doesn't matter).

#### Examples:

```sh
$ cd ~
$ pwd
/home/$USER
$ fvt init "my-repo"
Repository initialized at:/home/$USER/my-repo

$ mkdir -p my-projects
$ fvt init my-projects "test-repo"
Repository initialized at:/home/$USER/my-projects/test-repo

$ cd ~
$ pwd
/home/$USER
$ fvt init
Error: Missing repository name
```

### `commit` Command:

- [Header file](include/fvt/commit.h).
- [Source file](src/commit.cpp).

As of now, the FVT command merges the functionality of Git's [add](!https://git-scm.com/docs/git-add) and [commit](!https://git-scm.com/docs/git-commit) into the `commit` command. This means that when you commit files, you explictly define what files are added **AND THEN** commited.

> [!NOTE]
> I know that splitting the commit/add responsibilities into individual commands is better, but for an initial command this is easier.
> As I get more comofrtable with the development of the project, I will expand into more complex behaviors and commands.

Assuming the sample repository `my-repo` created at `/home/$USER/my-repo` seen above, see the usage for `commit`:

```sh
$ fvt commit `'.' for all files/directories or file1 file2 dir1 dir3/ for specific files` `"commit message"`
```

### Examples:

```sh
# Let's add a few files first:
$ touch foo.h foo.cpp main.cpp

# After working on these files we can commit them.
# Like in Git, to commit all files use `.`:
$ fvt commit . "initial commit"
Commit created successfully with hash: 81u32ubibe3i2

# It's also possible to commit specific files or directories:
$ touch bar.h bar.cpp
$ mkdir -p .github
$ mkdir -p tests
$ fvt commit bar.cpp .github tests/
Commit created successfully with hash: ubf38ou2hup2

# Attempting to commit without a commit message will fail:
$ touch README.md
$ fvt commit .
Error: Missing commit message
```

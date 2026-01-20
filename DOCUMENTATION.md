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
fvt init `repo_path` `"repo_name"`
```

> [!NOTE]
> Excluding the `repo_path` flag will result in the initialization of the repository at the current directory.
> The `repo_name` argument must be enclosed within quotes (single/double doesn't matter).

#### Sample usages:

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

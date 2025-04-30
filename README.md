# CS3520 Class Proect: File Versioning Tracker #

For our Programming in `C++` class proect we decided to build a simplified of a Version Control System (VCS). Inspired from famous VCSs like [Git](https://git.kernel.org/pub/scm/git/git.git) and [RCS](https://www.gnu.org/software/rcs/). Our VCS works entierly through the command line interface (CLI) and is 100% written in `C++`!

Like other VCSs, ours allows the user to do anything from creating new directories/files, comitting updated versions of files, logging and revising a repository's history, destroying files/repositories, and managing all repositories. In fact, most of the commands for our VCS are dierect mirrors of their Git equivalent. Here is a table comparing them:

| **Our Command** | **Git Equivalent** | **Description** |
|---------------|-----------------|----------------|
| `create` | `git init` | Initializes a new repository. |
| `destroy` | _N/A_ | Deletes the repository (Git doesn’t have a direct equivalent). |
| `commit` | `git commit` | Records changes to the repository. |
| `log` | `git log` | Displays commit history. |
| `checkout` | `git checkout` | Switches to a specific commit or branch. |
| `quit` | _N/A_ | Exits the program (Git doesn’t require this). |
| `clear` | _N/A_ | Clears the terminal screen (not a Git command). |
| `invalid` | _N/A_ | Represents an invalid command. |

One major difference between Git and us is that while Git does not require flags to specify whether the given command should be executed on a file or directory specifically (i.e. create a file or  destroy a directory), here is the table of flags and their explenation: 

Our VCS works in a more localized manner than git, following the workflow of RCS which simply creates a `commits` folder inside the directory the program is initialized at with the `create` command. This `commits` contains compressed (`.zip`) files which when expanded creates a folder containing the snapshots of the files at the moment the commit was made, alongside a `.txt` file outlining the changes made at that commit relative to the one before it. 

| **Flag** | **Description** |
|-------------|----------------|
| `-repo` | Specifies an operation on a repository. |
| `-file` | Specifies an operation on a file. |
| `-dir` | Specifies an operation on a directory. |
| `-none` | No flag is used. |
| `-invalid` | Represents an invalid flag. |

continue expanding as necessary. 

## Instructions

Will Usher's SDL 2.0 Tutorial can be found here: https://www.willusher.io/pages/sdl2/.

This starter code has done Lesson 0.

The Makefile is set up to build all `.cpp` files in the `src` folder and create executables in `bin/main`.

General structure:
 * src: for source (.cpp) files, everything here will get compiled/linked (src/main.cpp provided)
 * test: for test source code (using CxxText, expected to be in extern/cxxtest-4.4)
 * include: for header (.hpp) files, looked in for #include
 * res: for resources (e.g. images)
 * build: where created intermediate (e.g. object) files go
 * bin: where created executables go
 * doc: where generated documentation (Doxygen) goes

Configurations:
 * rel: for speed
 * deb: for debugging
 * cov: for code coverage (with gcov)
 * prf: for performance analysis (with gprof)

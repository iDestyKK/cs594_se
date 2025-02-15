# Homework Submissions Archive

## Synopsis
Here, you will find my homework for the course. These are the big assignments
that we were given two weeks to do.

## Homework Progression
```diff
+ 01 - DNA/Genome Analysis
+ 02 - Sequence Alignment
+ 03 - Hidden Markov Models (Occasionally Dishonest Casino) & Glimmer
+ 04 - MUMmer & AMOS utilisation
+ 05 - Hamming Distance, UPGMA, PHYLIP
```

## Directory Structure

### Assignment Directories
Each assignment has its own directory, numbered accordingly by assignment
number. `01`, `02`, `03`, etc. Each have the following:

* Their own directory structure
  * `bin` for binaries
  * `src` for source code files (`.c` and `.h` files)
  * `lib` for libraries (`cnds` usually)
  * `data` for misc data given or generated for the assignment
  * `obj` for object files created during compilation (`.o` and `.a` files)
* `makefile` for compiling all programs into `bin`
* `distro.pdf`, containing the original assignment given to us.
* `report.txt`, containing my answers to each question given in `distro.pdf`.

Each submission is treated as standalone. They do not depend on any files
outside of their directories. This is intentional, as each submission should
compile on UTK's Hydra/Tesla computers with a single `make` call. Though, this
means libraries like CNDS (Clara Nguyen's Data Structures) is included multiple
times in this repo. This is, again, intentional so each assignment is
standalone.

### Submission Directory
There is a special directory, named `submit` which contains "master copies" of
submissions in `tar.xz` format. These are the exact archives that I submitted
on Instructure Canvas. These are generated by the `create_submission_tars.sh`
script in this directory. Each assignment directory has a `makefile` with a
special command to make a tar file. This is done via:
```bash
UNIX> make package
```
The script simply takes the `submit.tar.xz` created by `make package` and moves
it into `submit`.

# Challenges Program Archive

## Synopsis
A collection of helper programs written to solve problems on weekly challenges.
Challenges are minor assignments that are completion grades. However, the
content in them are useful study material for homework and exams.

## Directory Structure

### Challenge Directories
Each challenge has its own directory, numbered accordingly by assignment
number. `01`, `02`, `03`, etc. Each have the following:

* Their own directory structure
  * `bin` for binaries
  * `src` for source code file (`.c` and `.h` files)
  * `lib` for libraries (`cnds` usually)
  * `data` for misc data given or generated for the assignment
  * `obj` for object files created during compilation (`.o` and `.a` files)
* `makefile` for compiling all programs into `bin`
* `distro.pdf`, containing the original challenge given to us.

Unlike the homeworks, the challenges do not require a `report.txt`. They also
do not require a submission of anything other than a document containing the
answers to the question. It's a participation grade. Either way, the tools here
aren't made to spit out the right answer (even though they definitely do). They
were made so I can understand *how* it works. And may be used to help guide you
to the correct answer.

Also, like homeworks, they do not depend on any files outside of their
directories. This is intentional. If I were to segment out a single challenge
directory (e.g. `01`) and call `make`, it should just work, because everything
needed is in that single directory. They are all standalone.

## Challenge Progression
```diff
+ 01 - DNA Sequences, Multinomial Model/Markov probabilities, alignment
+ 02 - Alignment via Dynamic Programming. Global, Local, End-gap Free.
+ 03 - Polymorphisms, Lookup Tables, Markov Model Drawing
- 04 - Occasionally Dishonest Casino probabilities, Hidden Markov Models
+ 05 - Suffix Trees, Suffix Arrays, LCP Arrays
- 06 - Genome Assembly
- 07 - Phylogenetics
```
***Note:** I'm pretty sure I did 6 and 7. But the files are not committed and
not found. Oh well.*

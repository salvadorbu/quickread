# Description
Memory efficient multithreaded searcher for large single-byte encoded files (i.e. UTF-8, Latin1, ISO 8859-1). 
Multiple threads search through a memory mapped (mmap) pointer such that the entire file is never loaded into memory. Boyer-Moore algorithm is used for the string search.
Users on 32-bit machines may run into issues with larger files since program is limited by the address space.

Here is an example with a 15GB file:

https://github.com/salvadorbu/quickread/assets/89661726/a26becf9-eb11-445e-8d10-00a195a54418

Under optimal conditions 15GB file search should take <4 seconds. Files under 5GB should search almost instantly.

# Usage
```console
~ ./executable -f "file.ext" -s "term to search" -t <# of threads to use (optional)>
```
Specify file name, pattern to search for, and number of threads (default thread count of 25). It isn't recommended for the number of threads to go over 100.

# Installation (Linux only)

Clone the repo

```console
git clone https://github.com/salvadorbu/quickread.git
```

Run make file (requires ncurses)

```
make
```

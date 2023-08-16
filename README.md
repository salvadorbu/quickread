# Usage
```console
~ ./executable -f "file.ext" -s "term to search" -t <# of threads to use (optional)>
```

Reads memory-mapped pointer with multiple threads to search for term. The addresses
for found terms are stored in a doubly linked list. The output from the list is printed
to a terminal UI where the user can navigaye through the search results.

# Installation (Linux only)

Clone the repo

```console
git clone https://github.com/salvadorbu/quickread.git
```

Run make file

```
make
```

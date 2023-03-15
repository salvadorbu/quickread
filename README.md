# quickread: Fast multithreaded file search for large files

Usage:
```console
~ ./executable -f "file.ext" -s "term to search" -t <# of threads to use (optional)>
```

Reads memory-mapped pointer with multiple threads to search for term. The addresses
for found terms are stored in a doubly linked list. The text surrounding the term
address is then printed to the console for each entry.

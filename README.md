# Dictionary implementation
### hash table that support operation INSERT, SEARCH, DELETE

---

### Hash algorithms implemented
- linear hash: key + i
- geometric hash: (5 * (key + i)) + 1
---

### Comments
A lot of things can be improved...
may be create a init function that produce tabes of size 2^n?
you can rmeove the % operator by & more efficient

---

### Dependencies
For run the tests are necesary [CppUTest](https://cpputest.github.io/)

---
### TODO list
- improve the documentation, add reference to dictionary
- Add makefile to create a library
- init hash_table function?
- Add guthubs actions for precompile
- Add more hash implementations
- Add performance tests

---

### References
- [Introduction to algorithms](https://en.wikipedia.org/wiki/Introduction_to_Algorithms)
- [Old python dict object implementation](svn.python.org/projects/python/trunk/Objects/dictobject.c)

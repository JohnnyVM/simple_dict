# C11 dictionary implementation
### Dictionary that support operation INSERT, SEARCH, DELETE
and other helpers

---

### Hash algorithms implemented
- linear hash: key + i
- geometric hash(DEFAULT): (5 * (key + i)) + 1
---

### Comments
A lot of things can be improved...
may be create a init function that produce tabes of size 2^n?
then you can remove the % operator by &, more efficient

---

### Caveats

Mostly tested but can found bugs, i apreciate if the bug are report

---
### Dependencies
For run the tests are necesary [CppUTest](https://cpputest.github.io/)

---
### TODO list
- improve the documentation, add reference to dictionary
- Add makefile to create a library
- Add githubs actions for precompile
- Add more hash implementations
- Add performance tests
- Add coverage
---


### References
- [Introduction to algorithms](https://en.wikipedia.org/wiki/Introduction_to_Algorithms)
- [Old python dict object implementation](https://github.com/python/cpython/blob/master/Objects/dictobject.c)

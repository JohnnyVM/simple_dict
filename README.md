# C11 dictionary implementation
### Dictionary that support operation INSERT, SEARCH, DELETE
and a small helper for has_key, hash_len

---

### Hash algorithms implemented
- linear hash: key + i
- geometric hash(DEFAULT): (5 * (key + i)) + 1
---

### Comments
A lot of things can be improved...
may be create a init function that produce tabes of size 2^n?
you can rmeove the % operator by & more efficient

---

### Caveats

If the dict is manually initialized wth length 1 fail

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
- [Old python dict object implementation](https://github.com/python/cpython/blob/master/Objects/dictobject.c)

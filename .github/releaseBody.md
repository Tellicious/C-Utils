## Multiple bugfixes and improvements

**New features:**
- Added some configurable parameters (via defines) to `list`, `LKHashTable`, `LPHashTable`, `movingAvg` and `queue`
- Added `queuePopBackArr` function to `queue`
  
**Improvements:**
- Improved performance of `LPHashTable` by removing unnecessary memory releases / allocations
- Extended unit tests coverage to 100% for all objects
  
**Bugfix:**
- Fixed several bugs in `LPHashTable` that were causing relevant functional errors
- Fixed a few bugs in `LKHashTable` that were causing memory leaks in some conditions
- Minor bugfix to `matrixDet` and `matrixDetStatic`
- Minor bugfix to `quaternionToEuler`
- Minor bugfix to `numMethods`
- Other minor improvements and bugfixes

See [Changelog](Changelog.md)
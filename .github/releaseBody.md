## Added unit tests and many bugfixes

**New features:**
- Added function to delete event object
- Added functions to delete hash-table objects
- Added unit tests for all functions

**Improvements:**
- Improved `timer.c` function to detect multiple events
- Added asserts to check input parameters correctness in `IIRFilters.c`
- Changed `fastSqrt` and `fastInvSqrt` to return `NaN` if a negative value is given as input
- Minor change to `PID_calcAeroClamp` to fully stop integration when saturation is reached
- Minor improvement to `PID_calcBackCalc`
- Changed return type of `eventInit`
- Minor change in LPHashTable resizing logic
- Added some more asserts to check input parameters to `matrix.c`

**Bugfix:**
- Fixed a bug causing an endless loop on certain conditions on `lpHashTableGet`
- Removed useless and not working iterator from `LPHashTable`
- Bugfix on `BIT_TOGGLE`, `BIT_TOGGLE_IN_PLACE`, `MAX` and `MIN` macros
- Major bugfix on `PID_calcIntegralClamp`
- Bugfix on `listInsert`
- Minor bugfix on `matrixInversedStatic_rob`
- Minor bugfix to `numMethods.h`
- Minor bugfix on `event.h`
- Fixed descriptions in some headers

See [Changelog](Changelog.md)
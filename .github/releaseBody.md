## Multiple improvements to PID

**Improvements:**
- Major change to `PID_calcBackCalc` to align output with Simulink PID block
- Optimized PID calculation functions to remove 2 unnecessary struct members
- Changed many inline functions to defines in `PID.h`
- Extended PID unit tests to validate also set, get and reset functions

See [Changelog](Changelog.md)
# Changelog

## v1.7.1

**Improvements:**
- Improved robustness of `quaternionNorm`, including check on infinite and nan

## v1.7.0

**New features:**
- Added `CMakeLists.txt`

**Improvements:**
- Fixed some typos

## v1.6.0

**New features:**
- Added `utilsStatus_t` to commonTypes.h

**Breaking changes:**
- Updated all libraries to use `utilsStatus_t` instead of their own status enum

**Improvements:**
- Minor formatting changes

## v1.5.0

**New features:**
- Added commonTypes.h containing common typedefs (`axis3f_t`)

**Breaking changes:**
- Updated quaternion object to use commonTypes.h

## v1.4.0

**Improvements:**
- Added reset functions to IIRFilters

## v1.3.0

**New features:**
- Added IIR filter objects (generic, derivative and integrator)

**Improvements:**
- Fixed snippets
- Renamed a few variables inside PID object


## v1.2.0

**Improvements:**
- Added deadband macro to basicMath and minor changes

## v1.1.1

**Improvements:**
- Reformatted entire code

## v1.1.0

### Contains breaking change on button initialization function

**New features:**
- Added possibility to create a pulse-detection button via BUTTON_TYPE_PULSATING
- Added new very-long press detection
- Added buttonGetStatus to return debounced button status

**Improvements:**
- button resetTicks are now defined during initialization and no longer a macro
- Reduced button_t memory footprint by using bitfields and by using smaller types
- Minor typo correction on LKHashTable and LPHashTable

## v1.0.0

- Initial release
# Changelog

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
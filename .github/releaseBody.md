## Added possibility of using static and/or dynamic memory management

### Contains breaking change on memory management. `ADVUTILS_USE_STATIC_ALLOCATION` and/or `ADVUTILS_USE_DYNAMIC_ALLOCATION` must be defined by user

**New features:**
- Added `*Static` variant of multiple functions with static memory management
- Added possibility of setting static and/or dynamic memory management for ADVUtils library via definition of `ADVUTILS_USE_STATIC_ALLOCATION` and `ADVUTILS_USE_DYNAMIC_ALLOCATION`. At least one of the two must be defined by the user
- Added possibility of using FreeRTOS-specific dynamic memory management functions with `set(ADVUtils_DYN_MEM_MGMT "USE_FREERTOS" CACHE STRING "" FORCE)` in CMake

See [Changelog](Changelog.md)
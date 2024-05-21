<p align="center"> <img src="https://github.com/Tellicious/ADV-utils/assets/9076397/3ec512f1-2de6-4226-bc07-e4bfdd686a28" width=50% height=50%> </p>

# Collection of useful C utilities

## Libraries included:
- ***basicMath:*** math constants, conversion factors, bitwise operators, `fastSqrt`, `fastInvSqrt`, `fastSin` and `fastCos`
- ***matrix:*** handling of matrix objects and matrix operations
- ***numMethods:*** common numerical methods (to solve linear systems and perform Gauss-Newton sphere approximation)
- ***timer:*** user-configurable timers
- ***queue:*** queue structure
- ***list:*** linked list
- ***quaternion:*** quaternion operations and conversion to Euler angles
- ***PID:*** PID controller with several anti-windup options (aero-specific, integral-clamping, back-calculation)
- ***button:*** interrupt-based button object with debounce and multiple press types detection
- ***LPHashTable:*** linear-probing hash-table object with auto-resize capability
- ***LKHashTable:*** dynamic linked hash-table object (based on _list_)
- ***IIRFilters:*** simple discrete-time IIR filters, with on-the-fly conversion continuous -> discrete of derivative, integrator, 2nd order low-pass and 2nd order high-pass filters
- ***commonTypes:*** collection of common type definitions

## Configuration:
- User must define `ADVUTILS_USE_STATIC_ALLOCATION` and/or `ADVUTILS_USE_DYNAMIC_ALLOCATION` to select wheter to use static and/or dynamic memory management. `list`, `LPHashTable` and `LKHashTable` are available only with `ADVUTILS_USE_DYNAMIC_ALLOCATION`
- Functions that use static allocation are defined by `Static` suffix
- To automatically use thread-safe FreeRTOS-specific implementation of dynamic memory management functions (`malloc`, `calloc` and `free`) user can add `set(ADVUtils_DYN_MEM_MGMT "USE_FREERTOS" CACHE STRING "" FORCE)` before `add_subdirectory()` in project CMakeLists. This will automatically add the following defines to ADVUtils compilation:
    - `ADVUTILS_MEMORY_MGMT_HEADER="FreeRTOS.h"`
    - `ADVUTILS_MALLOC=pvPortMalloc`
    - `ADVUTILS_CALLOC=pvPortCalloc`
    - `ADVUTILS_FREE=vPortFree`
    - `target_link_libraries(ADVUtils freertos_kernel)`
- If `ADVUTILS_MEMORY_MGMT_HEADER` is not defined, the library will use `stdlib` version of `malloc`, `calloc` and `free`
- User can use another different implementation of `malloc`, `calloc` and `free` by defining the following four macros:
    - `ADVUTILS_MEMORY_MGMT_HEADER`
    - `ADVUTILS_MALLOC`
    - `ADVUTILS_CALLOC`
    - `ADVUTILS_FREE`
- User can select to use faster math functions from `basicMath` by adding `USE_FAST_MATH` to compile definitions 





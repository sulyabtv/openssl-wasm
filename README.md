Running demo
------------
- emcc test/demo/demo.c -o test/demo/demo.js -sEXPORTED_FUNCTIONS=_int_sqrt -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
- python3 driver.py
emcc test/demo/demo.c \
    -o test/demo/demo.js \
    -sEXPORTED_FUNCTIONS=_int_sqrt \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

emcc test/aes_256/aes_256.c openssl-3.0.8/libssl.a openssl-3.0.8/libcrypto.a \
    -I$(pwd)/openssl-3.0.8/include \
    -o test/aes_256/aes_256.js \
    -sEXPORTED_FUNCTIONS=_main,_encrypt,_decrypt \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

emcc test/sha_256/sha_256.c openssl-3.0.8/libssl.a openssl-3.0.8/libcrypto.a \
    -I$(pwd)/openssl-3.0.8/include \
    -o test/sha_256/sha_256.js \
    -sEXPORTED_FUNCTIONS=_main \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

emcc test/aes_256_file/aes_256_file.c openssl-3.0.8/libssl.a openssl-3.0.8/libcrypto.a \
    -I$(pwd)/openssl-3.0.8/include \
    -o test/aes_256_file/aes_256_file.js \
    -sEXPORTED_FUNCTIONS=_main,_do_crypt \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
    --preload-file files/bigfile.txt

emcc test/sha_256_file/sha_256_file.c openssl-3.0.8/libssl.a openssl-3.0.8/libcrypto.a \
    -I$(pwd)/openssl-3.0.8/include \
    -o test/sha_256_file/sha_256_file.js \
    -sEXPORTED_FUNCTIONS=_main \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
    --preload-file files/bigfile.txt
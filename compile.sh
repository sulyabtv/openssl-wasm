mkdir files
head -c 1M /dev/urandom > files/small.txt
head -c 100M /dev/urandom > files/med.txt
head -c 1G /dev/urandom > files/large.txt

# Demo
emcc test/wasm/demo/demo.c \
    -o test/wasm/demo/demo.js \
    -sEXPORTED_FUNCTIONS=_int_sqrt \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

# String Benchmarks
emcc test/wasm/str/aes_256/aes_256.c openssl-3.0.8/libssl.a openssl-3.0.8/libcrypto.a \
    -I$(pwd)/openssl-3.0.8/include \
    -o test/wasm/str/aes_256/aes_256.js \
    -sEXPORTED_FUNCTIONS=_main,_encrypt,_decrypt \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

emcc test/wasm/str/sha_256/sha_256.c openssl-3.0.8/libssl.a openssl-3.0.8/libcrypto.a \
    -I$(pwd)/openssl-3.0.8/include \
    -o test/wasm/str/sha_256/sha_256.js \
    -sEXPORTED_FUNCTIONS=_main \
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

# File Benchmarks
for filesize in small med large;
do
    emcc test/wasm/file/aes_256_$filesize/aes_256_$filesize.c \
         openssl-3.0.8/libssl.a \
         openssl-3.0.8/libcrypto.a \
        -I$(pwd)/openssl-3.0.8/include \
        -o test/wasm/file/aes_256_$filesize/aes_256_$filesize.js \
        -sEXPORTED_FUNCTIONS=_main,_do_crypt \
        -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
        --preload-file files/$filesize.txt

    emcc test/wasm/file/sha_256_$filesize/sha_256_$filesize.c \
         openssl-3.0.8/libssl.a \
         openssl-3.0.8/libcrypto.a \
        -I$(pwd)/openssl-3.0.8/include \
        -o test/wasm/file/sha_256_$filesize/sha_256_$filesize.js \
        -sEXPORTED_FUNCTIONS=_main \
        -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
        --preload-file files/$filesize.txt
done
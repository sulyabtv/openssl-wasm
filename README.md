## Step 0: Prerequisites
- Install emscripten
- Compile OpenSSL into wasm
    ```
    tar -xvf openssl-3.0.8.tar.gz
    cd openssl-3.0.8/
    emconfigure ./Configure linux-generic64 no-asm no-threads no-engine no-dtls no-shared no-dso --prefix=$EMSCRIPTEN/cache/sysroot
    sed -i 's|^CROSS_COMPILE.*$|CROSS_COMPILE=|g' Makefile
    sed -i '/^CFLAGS/ s/$/ -D__STDC_NO_ATOMICS__=1/' Makefile
    sed -i '/^CXXFLAGS/ s/$/ -D__STDC_NO_ATOMICS__=1/' Makefile
    emmake make -j 1 build_generated libssl.a libcrypto.a
    cd ..
    ```

## Step 1: Compile code into wasm and js
    bash compile.sh

## Step 2: Run the driver
    python driver.py
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define FILENAME "files/med.txt"

int do_crypt(FILE *in, FILE *out, int do_encrypt);

int main() {
    FILE *in = fopen(FILENAME, "rb");
    FILE *out = fopen("enc", "wb");
    if (!do_crypt(in, out, 1)) {
        return 1;
    }
    fclose(in);
    fclose(out);

    in = fopen("enc", "rb");
    out = fopen("dec", "wb");
    if (!do_crypt(in, out, 0))
    {
        return 1;
    }
    fclose(in);
    fclose(out);

    FILE *pt1 = fopen(FILENAME, "rb");
    FILE *pt2 = fopen("dec", "rb");
    int byte1 = 0, byte2 = 0;
    while (byte1 != EOF && byte2 != EOF) {
        byte1 = fgetc(pt1);
        byte2 = fgetc(pt2);
        if (byte1 != byte2) {
            return 1;
        }
    }
    if ((byte1 == EOF && byte2 != EOF) || (byte1 != EOF && byte2 == EOF)) {
        return 1;
    }
    fclose(pt1);
    fclose(pt2);

    return 0;
}

int do_crypt(FILE *in, FILE *out, int do_encrypt)
{
    /* Allow enough space in output buffer for additional block */
    unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
    int inlen, outlen;
    EVP_CIPHER_CTX *ctx;
    /*
     * Bogus key and IV: we'd normally set these from
     * another source.
     */
    unsigned char key[] = "0123456789abcdef0123456789abcdef";
    unsigned char iv[] = "1234567887654321";

    /* Don't set key or IV right away; we want to check lengths */
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_CipherInit_ex2(ctx, EVP_aes_256_cbc(), NULL, NULL,
                            do_encrypt, NULL)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    OPENSSL_assert(EVP_CIPHER_CTX_get_key_length(ctx) == 32);
    OPENSSL_assert(EVP_CIPHER_CTX_get_iv_length(ctx) == 16);

    /* Now we can set key and IV */
    if (!EVP_CipherInit_ex2(ctx, NULL, key, iv, do_encrypt, NULL)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    for (;;) {
        inlen = fread(inbuf, 1, 1024, in);
        if (inlen <= 0)
            break;
        if (!EVP_CipherUpdate(ctx, outbuf, &outlen, inbuf, inlen)) {
            /* Error */
            EVP_CIPHER_CTX_free(ctx);
            return 0;
        }
        fwrite(outbuf, 1, outlen, out);
    }
    if (!EVP_CipherFinal_ex(ctx, outbuf, &outlen)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    fwrite(outbuf, 1, outlen, out);

    EVP_CIPHER_CTX_free(ctx);
    return 1;
}

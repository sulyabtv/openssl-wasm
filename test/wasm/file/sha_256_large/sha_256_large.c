#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

#define FILENAME "files/large.txt"

int main()
{
    FILE *in = fopen(FILENAME, "rb");
    unsigned char inbuf[1024];

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    unsigned char expected_md_value[64] = {0x5c, 0xe6, 0x91, 0x72,
                                           0xf3, 0x1e, 0x0e, 0xae,
                                           0x7e, 0xfd, 0xb4, 0x1d,
                                           0xb7, 0x50, 0x26, 0xbf,
                                           0xc7, 0x47, 0x7f, 0x96,
                                           0x9a, 0x5f, 0x44, 0x21,
                                           0x33, 0x97, 0x15, 0xfa,
                                           0xd0, 0x03, 0x3e, 0x7f};
    unsigned char md_value[64];
    unsigned int md_len;

    for (;;)
    {
        int inlen = fread(inbuf, 1, 1024, in);
        if (inlen <= 0)
            break;
        EVP_DigestUpdate(mdctx, inbuf, inlen);
    }
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);

    for (int i = 0; i < md_len; i++) {
        if (md_value[i] != expected_md_value[i]) return 1;
    }

    return 0;
}
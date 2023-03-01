// Modified from https://github.com/danbev/learning-openssl/blob/master/digest.c

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

int main() {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = NULL;

    char message[] = "The quick brown fox jumps over the lazy dog";
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len = 0;
    md = EVP_get_digestbyname("SHA256");
    int i = 0;

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, message, strlen(message));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);

    printf("Digest is: ");
    for (i = 0; i < md_len; i++) {
      printf("%02x", md_value[i]);
    }
    printf("\n");

    return 0;
}
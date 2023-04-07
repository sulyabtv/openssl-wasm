enc = new TextEncoder();
var raw_key = enc.encode("01234567890123456789012345678901");
var iv = enc.encode("0123456789012345");
var plain_text = "The quick brown fox jumps over the lazy dog";
var plain_text_enc = enc.encode(plain_text);
var imported_key;

async function crypt() {
    return await window.crypto.subtle
        .importKey("raw",
            raw_key,
            "AES-CBC",
            false,
            ["encrypt", "decrypt"])
        .then(function (key) {
            imported_key = key;
            return window.crypto.subtle.encrypt(
                { name: "AES-CBC", iv: iv },
                key,
                plain_text_enc
            );
        })
        .then(function (cipherText) {
            return window.crypto.subtle.decrypt(
                { name: "AES-CBC", iv: iv },
                imported_key,
                cipherText
            )
        })
        .then(function (decryptedVal) {
            return plain_text == new TextDecoder().decode(decryptedVal);
        })
}
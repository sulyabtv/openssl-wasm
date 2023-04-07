enc = new TextEncoder();
var raw_key = enc.encode("0123456789abcdef0123456789abcdef");
var iv = enc.encode("1234567887654321");
var imported_key;

function arrayBufEqual(buf1, buf2) {
    if (buf1.byteLength != buf2.byteLength) return false;
    var dv1 = new Int8Array(buf1);
    var dv2 = new Int8Array(buf2);
    for (var i = 0; i != buf1.byteLength; i++) {
        if (dv1[i] != dv2[i]) return false;
    }
    return true;
}

async function crypt(encoded_data) {
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
                encoded_data
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
            return arrayBufEqual(encoded_data, decryptedVal) ? 0 : 1;
        })
}
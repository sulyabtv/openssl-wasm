valid_hashes = [
    "6e7a00938b91449196519fea7f38ccbc1368bb9656365f0b79500a5f659ce4e7",
    "2319b8191a5571802e12c912b8af947f5438e4331abd454ba2c33a863d87cb24",
    "ac589bc703fe3a4075789a0d4e5fb5125d1a9e4c805daa75162dcf01ee761644"]

function arrayBufEqual(buf1, buf2) {
    if (buf1.byteLength != buf2.byteLength) return false;
    var dv1 = new Int8Array(buf1);
    var dv2 = new Int8Array(buf2);
    for (var i = 0; i != buf1.byteLength; i++) {
        if (dv1[i] != dv2[i]) return false;
    }
    return true;
}

async function sha(encoded_data) {
    dec = new TextDecoder();
    hashBuffer = await window.crypto.subtle.digest("SHA-256", encoded_data);
    hashArray = Array.from(new Uint8Array(hashBuffer));
    hashHex = hashArray
        .map((b) => b.toString(16).padStart(2, "0"))
        .join(""); // convert bytes to hex string
    return valid_hashes.includes(hashHex);
}
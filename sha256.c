#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SIG0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIG1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define OMEGA0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
#define OMEGA1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

// Initial hash values (first 32 bits of the fractional parts of square roots of the first 8 primes)
static const uint32_t H[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

// SHA-256 constants (first 32 bits of the fractional parts of cube roots of the first 64 primes)
static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Padding and processing the message
void sha256_process(const uint8_t *message, uint32_t *hash, size_t len) {
    uint32_t W[64];
    uint32_t a, b, c, d, e, f, g, h;

    // Message padding
    size_t padded_len = ((len + 8) / 64 + 1) * 64; // Length rounded to the next multiple of 64
    uint8_t padded[padded_len];
    memset(padded, 0, padded_len);
    memcpy(padded, message, len);
    padded[len] = 0x80; // Append a single '1' bit
    uint64_t bit_len = len * 8;
    for (int i = 0; i < 8; i++) {
        padded[padded_len - 1 - i] = (bit_len >> (i * 8)) & 0xFF; // Append the message length
    }

    // Process each 512-bit chunk
    for (size_t i = 0; i < padded_len; i += 64) {
        // Prepare the message schedule W
        for (int t = 0; t < 16; t++) {
            W[t] = (padded[i + t * 4] << 24) |
                   (padded[i + t * 4 + 1] << 16) |
                   (padded[i + t * 4 + 2] << 8) |
                   (padded[i + t * 4 + 3]);
        }
        for (int t = 16; t < 64; t++) {
            W[t] = OMEGA1(W[t - 2]) + W[t - 7] + OMEGA0(W[t - 15]) + W[t - 16];
        }

        // Initialize working variables
        a = hash[0];
        b = hash[1];
        c = hash[2];
        d = hash[3];
        e = hash[4];
        f = hash[5];
        g = hash[6];
        h = hash[7];

        // Perform the main hashing loop
        for (int t = 0; t < 64; t++) {
            uint32_t T1 = h + SIG1(e) + CH(e, f, g) + K[t] + W[t];
            uint32_t T2 = SIG0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        // Add the working variables back into the hash
        hash[0] += a;
        hash[1] += b;
        hash[2] += c;
        hash[3] += d;
        hash[4] += e;
        hash[5] += f;
        hash[6] += g;
        hash[7] += h;
    }
}

// Compute the SHA-256 hash of a message
void sha256(const uint8_t *message, size_t len, uint8_t *digest) {
    uint32_t hash[8];
    memcpy(hash, H, sizeof(H));

    sha256_process(message, hash, len);

    // Convert the hash to bytes
    for (int i = 0; i < 8; i++) {
        digest[i * 4] = (hash[i] >> 24) & 0xFF;
        digest[i * 4 + 1] = (hash[i] >> 16) & 0xFF;
        digest[i * 4 + 2] = (hash[i] >> 8) & 0xFF;
        digest[i * 4 + 3] = hash[i] & 0xFF;
    }
}

int main() {
    const char *message = "Hello, SHA-256!";
    uint8_t digest[32];

    sha256((const uint8_t *)message, strlen(message), digest);

    printf("SHA-256 Digest: ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");

    return 0;
}
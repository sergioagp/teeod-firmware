#include <stdint.h>
#include <tee_api.h>

static uint32_t xorshift_state = 0x12345678;

static uint32_t xorshift32(void)
{
    uint32_t x = xorshift_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    xorshift_state = x;
    return x;
}

void TEE_GenerateRandom(void *randomBuffer, uint32_t randomBufferLen)
{
    uint8_t *buffer = (uint8_t *)randomBuffer;
    uint32_t remainingBytes = randomBufferLen;

    while (remainingBytes > 0) {
        uint32_t randomWord = xorshift32();
        uint32_t copyLen = (remainingBytes < sizeof(uint32_t)) ? remainingBytes : sizeof(uint32_t);
        for (int i = 0; i < copyLen; i++) {
            *buffer++ = (uint8_t)(randomWord >> (i * 8));
        }
        remainingBytes -= copyLen;
    }
}


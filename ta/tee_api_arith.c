#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tee_api.h>


void TEE_BigIntInit(TEE_BigInt *bigInt, uint32_t len)
{
    // Allocate memory for the big integer
    bigInt = (uint32_t *) malloc(len * sizeof(uint32_t));
    if (!bigInt == NULL) {
        return;
    }

}

void TEE_BigIntDiv(TEE_BigInt *dest_q, TEE_BigInt *dest_r,
                   const TEE_BigInt *op1, const TEE_BigInt *op2)
{
    // Check for special cases
    if (*op2 == 0 || *op1 < *op2) {
        *dest_q = 0;
        *dest_r = *op1;
        return;
    }

    // Normalize the divisor
    int shift = 0;
    TEE_BigInt norm_div = *op2;
    while ((norm_div & 0x80000000) == 0) {
        norm_div <<= 1;
        shift++;
    }

    // Perform long division
    TEE_BigInt dividend = *op1;
    TEE_BigInt quotient = 0;
    TEE_BigInt remainder = 0;
    for (int i = 31; i >= shift; i--) {
        remainder <<= 1;
        remainder |= (dividend >> i) & 1;
        if (remainder >= norm_div) {
            remainder -= norm_div;
            quotient |= 1 << (i - shift);
        }
    }

    // Denormalize the remainder
    remainder >>= shift;

    // Return the quotient and remainder
    *dest_q = quotient;
    *dest_r = remainder;
}

TEE_Result TEE_BigIntConvertFromOctetString(TEE_BigInt *dest,
                                            const uint8_t *buffer,
                                            uint32_t bufferLen,
                                            int32_t sign)
{
    // Check input parameters
    if (!dest || !buffer || bufferLen == 0) {
        return TEE_ERROR_BAD_PARAMETERS;
    }

    // Allocate memory for the big integer
    uint32_t *ptr = (uint32_t *) malloc((bufferLen + 3) / 4 * sizeof(uint32_t));
    if (ptr == NULL) {
        return TEE_ERROR_OUT_OF_MEMORY;
    }

    // Initialize the big integer to zero
    memset(ptr, 0, (bufferLen + 3) / 4 * sizeof(uint32_t));

    // Copy the octet string to the big integer in little-endian format
    for (int i = 0; i < bufferLen; i++) {
        ptr[i / 4] |= (uint32_t) buffer[i] << (8 * (i % 4));
    }

    // Set the sign of the big integer
    if (sign < 0) {
        // If sign is negative, set the most significant bit
        ptr[(bufferLen + 3) / 4 - 1] |= 0x80000000;
    }

    // Set the pointer to the allocated memory
    *dest = (TEE_BigInt) ptr;

    return TEE_SUCCESS;
}

TEE_Result TEE_BigIntConvertToS32(int32_t *dest, const TEE_BigInt *src)
{
    // Check input parameters
    if (!dest || !src) {
        return TEE_ERROR_BAD_PARAMETERS;
    }

    // Cast the source pointer to uint32_t*
    uint32_t *ptr = (uint32_t *) *src;

    // Check if the big integer fits into a 32-bit signed integer
    if (ptr[1] != 0 || ptr[0] > 0x7FFFFFFF) {
        return TEE_ERROR_OVERFLOW;
    }

    // Copy the least significant 32 bits of the big integer to the destination
    *dest = (int32_t) ptr[0];

    return TEE_SUCCESS;
}

#include <stdint.h>

typedef uint32_t TEE_BigInt;

int32_t TEE_BigIntCmpS32(const TEE_BigInt *op, int32_t shortVal)
{
    // Cast the big integer pointer to uint32_t*
    uint32_t *ptr = (uint32_t *) *op;

    // Compare the big integer with the short value
    if (ptr[1] != 0) {
        // The big integer has more than 32 bits
        if (shortVal >= 0) {
            return 1;
        } else {
            return -1;
        }
    } else {
        // The big integer has 32 bits or less
        if (ptr[0] > (uint32_t) shortVal) {
            return 1;
        } else if (ptr[0] < (uint32_t) shortVal) {
            return -1;
        } else {
            return 0;
        }
    }
}

#include <stdio.h>
#include "oxorany_include.h"

static OXORANY_FORCEINLINE constexpr _lxy__uint8_t encrypt_byte(_lxy__uint8_t c, _lxy__size_t i, _lxy__size_t key) {
    return static_cast<_lxy__uint8_t>(((c + (key * 7)) ^ (i + key)));
}

static OXORANY_FORCEINLINE constexpr _lxy__uint8_t decrypt_byte(_lxy__uint8_t c, _lxy__size_t i, _lxy__size_t key) {
    //a ^ b == (a + b) - 2 * (a & b)
    _lxy__size_t a = c;
    _lxy__size_t b = i + key;
    //_lxy__size_t a_xor_b = (a + b) - 2 * (a & b);
    _lxy__size_t a_xor_b = (a + b) - ((a & b) + (b & a));
    return static_cast<_lxy__uint8_t>((a_xor_b)-(key * 7));
}

void encrypt_buffer(uint8_t* buffer, size_t buffer_size, uint8_t key)
{
    for (size_t i = 0; i < buffer_size; i++)
    {
        buffer[i] += key;
    }
}

void decrypt_buffer(uint8_t* buffer, size_t buffer_size, uint8_t key)
{
    for (size_t i = 0; i < buffer_size; i++)
    {
        buffer[i] -= key;
    }
}

void encrypt(uint8_t* value, uint8_t key)
{
    *value = encrypt_byte(*value, key, key);
    //encrypt(value, 1, key);
}

void decrypt(uint8_t* value, uint8_t key)
{
    *value = decrypt_byte(*value, key, key);
    //decrypt(value, 1, key);
}

int main() 
{
    for (size_t i = 0; i <= 255; i++)
    {
        for (size_t j = 0; j <= 255; j++)
        {
            uint8_t v = i;
            uint8_t r = v;
            encrypt(&v, j);
            decrypt(&v, j);
            if (r != v)
            {
                printf("err\n");
            }
        }
    }
    return 0;
}

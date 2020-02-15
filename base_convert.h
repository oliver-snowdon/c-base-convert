/* 
 * Copyright (c) 2020 Oliver Snowdon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdint.h>

uint8_t add_in_base(uint8_t *a,
                    uint8_t *b,
                    uint8_t length,
                    uint8_t base)
{
    uint8_t carry = 0;
    for (uint8_t i = 0; i < length; i++) {
        uint16_t sum = a[i] + b[i] + carry;
        a[i] = sum % base;
        carry = sum / base;
    }
    return carry;
}

uint8_t multiply_by_two_in_base(uint8_t *a,
                                uint8_t length,
                                uint8_t base)
{
    return add_in_base(a, a, length, base);
}

void multiply_in_base(uint8_t *a,
                      uint8_t length,
                      uint8_t b,
                      uint8_t base,
                      uint8_t *tmp)
{
    memcpy(tmp, a, length);
    if (b % 2 == 0) {
        memset(a, 0, length);
    }
    while (b > 0) {
        b /= 2;
        multiply_by_two_in_base(tmp, length, base);
        if (b % 2 == 1) {
            add_in_base(a, tmp, length, base);
        }
    }
}

void convert_to_base(uint8_t input,
                     uint8_t base,
                     uint8_t *digits,
                     uint8_t length)
{
    memset(digits, 0, length);
    int c = 0;
    while (input > 0) {
        digits[c] = input % base;
        input /= base;
        ++c;
    }
}

uint8_t get_output_length_for_base_conversion(uint8_t input_length,
                                              uint8_t base,
                                              uint8_t *overflow)
{
    int f = 1;
    int tmp = 1;
    while (tmp < 256) {
        tmp *= base;
        ++f;
    }
    if (input_length * (f - 1) > 255) {
      *overflow = 1;
    }
    return input_length * (f - 1);
}

uint8_t get_output_length_for_base_conversion_from_base(uint8_t input_length,
                                                        uint8_t base_to,
                                                        int base_from,
                                                        uint8_t *overflow)
{
    int f = 1;
    int tmp = 1;
    while (tmp < base_from) {
        tmp *= base_to;
        ++f;
    }
    if (input_length * (f - 1) > 255) {
      *overflow = 1;
    }
    return input_length * (f - 1);
}

void convert_array_to_base(uint8_t *input,
                           uint8_t length,
                           uint8_t base,
                           uint8_t *digits,
                           uint8_t output_length,
                           uint8_t *tmp)
{
    memset(tmp, 0, output_length);
    memset(digits, 0, output_length);
    uint8_t i;
    for (i = 0; i < length; i++) {
        convert_to_base(input[i], base, tmp, output_length);
        uint8_t j;
        for (j = 0; j < i; j++) {
            multiply_in_base(tmp, output_length, 16, base, tmp+output_length);
            multiply_in_base(tmp, output_length, 16, base, tmp+output_length);
        }
        add_in_base(digits, tmp, output_length, base);
    }
}

void convert_array_to_base_from_base(uint8_t *input,
                                     uint8_t input_length,
                                     uint8_t base_to,
                                     uint8_t base_from,
                                     uint8_t *output,
                                     uint8_t output_length,
                                     uint8_t *tmp)
{
    memset(tmp, 0, output_length);
    memset(output, 0, output_length);
    uint8_t i;
    for (i = 0; i < input_length; i++) {
        convert_to_base(input[i], base_to, tmp, output_length);
        uint8_t j;
        for (j = 0; j < i; j++) {
            multiply_in_base(tmp, output_length, base_from, base_to, tmp+output_length);
        }
        add_in_base(output, tmp, output_length, base_to);
    }
}

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

#include <stdio.h>
#include <string.h>
#include "base_convert.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: convert <number> <base>\n");
        return 1;
    }
    int input_length = strlen(argv[1]);

    uint8_t *input = malloc(input_length);
    int i;
    for (i = 0; i < input_length; ++i) {
        if (argv[1][i] >= '0' && argv[1][i] <= '9') {
            input[input_length-i-1] = argv[1][i] - '0';
        } else {
            printf("non-numeric character found in <number> parameter\n");
            return 2;
        }
    }

    int base;
    sscanf(argv[2], "%d", &base);
    if (base < 1 || base > 255) {
        printf("base out of range\n");
        return 3;
    }

    uint8_t overflow = 0;
    uint8_t output_length = get_output_length_for_base_conversion_from_base(input_length,
                                                                            base,
                                                                            10,
                                                                            &overflow);

    if (overflow) {
        printf("input too large\n");
        return 4;
    }

    uint8_t *output = malloc(output_length);
    uint8_t *tmp = malloc(output_length * 2);

    convert_array_to_base_from_base(input,
                                    input_length,
                                    base,
                                    10,
                                    output,
                                    output_length,
                                    tmp);

    for (i = 0; i < output_length; ++i) {
        printf("%d ", output[output_length-i-1]);
    }
    printf("\n");

    free(output);
    free(tmp);
    free(input);
}

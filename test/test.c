/**
 * @file test.c
 * @brief This file contains the functions needed to test the correct execution
 *        of the main program.
 * @author Marco Plaitano
 * @date 13 Oct 2021
 *
 * COUNTING SORT OpenMP
 * Parallelize and Evaluate Performances of "Counting Sort" Algorithm, by using
 * OpenMP.
 *
 * Copyright (C) 2022 Plaitano Marco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "counting_sort.h"
#include "util.h"

/** @brief Number of array sizes the program is tested with. */
#define NUM_SIZES 5


/**
 * @brief Check that all the elements in the array are in the range [min; max].
 * @param array: The array.
 * @param size:  Number of elements in the array.
 * @param min:   Mininum value accepted.
 * @param max:   Maximum value accepted.
 * @return `true` if all elements are in given range; `false` otherwise.
 */
bool elements_in_range(const int *array, long long size, int min, int max);

/**
 * @brief Test the inizialization of the array.
 * @param array:       The array to initialize.
 * @param size:        Size of the array to inizialize.
 * @param num_threads: Number of threads to use.
 */
void test_initialization(int *array, long long size, int num_threads);

/**
 * @brief Test the correctness of the sorting algorithm.
 * @param array: The array to sort.
 * @param size:  Size of the array.
 * @param num_threads: Number of threads to use.
 */
void test_sort(int *array, long long size, int num_threads);



int main(int argc, char **argv) {
    int num_threads = 0;
    if (argc == 2)
        num_threads = atoi(argv[1]);
    if (num_threads < 0) {
        fprintf(stderr, "Can not launch program with a negative number of "
                        "threads (%d).\n", num_threads);
        return EXIT_FAILURE;
    }

    long long sizes[NUM_SIZES] = {10, 6053, 30000, 500009, 20000000};

    for (int i = 0; i < NUM_SIZES; i++) {
        printf("Testing size %lld (%d/%d) with %d threads...\n", sizes[i],
               i + 1, NUM_SIZES, num_threads);
        fflush(stdout);

        int *array = (int *)safe_alloc(sizes[i] * sizeof(int));
        test_initialization(array, sizes[i], num_threads);
        test_sort(array, sizes[i], num_threads);

        free(array);
    }

    return EXIT_SUCCESS;
}


bool elements_in_range(const int *array, long long size, int min, int max) {
    for (long long i = 0; i < size; i++)
        if (array[i] < min || array[i] > max)
            return false;
    return true;
}


void test_initialization(int *array, long long size, int num_threads) {
    for (long long i = 0; i < size; i++)
        array[i] = RANGE_MIN - 1;

    array_init_random(array, size, RANGE_MIN, RANGE_MAX, num_threads);

    /* Check that all elements are constrained in the range [min; max]. */
    if (!elements_in_range(array, size, RANGE_MIN, RANGE_MAX)) {
        fprintf(stderr, "FAILED Initialization!\n"
                        "The array elements are not in the range "
                        "[%d, %d]\n", RANGE_MIN, RANGE_MAX);
        free(array);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "OK Initialization.\n");
}


void test_sort(int *array, long long size, int num_threads) {
    counting_sort(array, size, num_threads);

    /* Check that no element has lesser value than its predecessor. */
    for (long long i = size - 1; i > 0; i--)
        if (array[i] < array[i - 1]) {
            fprintf(stderr, "FAILED Sorting!\n"
                            "array[%lld] %d > %d array[%lld]\n",
                            i - 1, array[i - 1], array[i], i);
            free(array);
            exit(EXIT_FAILURE);
        }
    fprintf(stdout, "OK Sorting.\n");
}

/**
 * @file main.c
 * @brief Main file of the source program used to generate and sort an array
 *        using Counting Sort Algorithm.
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

#include <stdio.h>
#include <stdlib.h>

#include "counting_sort.h"
#include "util.h"


int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: main.out (int)array_size (int)num_threads\n");
        return EXIT_FAILURE;
    }

    const long long size = atoll(argv[1]);
    int num_threads = atoi(argv[2]);
    int *array = (int *)safe_alloc(size * sizeof(int));
    double time_init = 0;
    double time_sort = 0;

    /* Fill the array with random values. */
    START_TIME(time_init);
    array_init_random(array, size, RANGE_MIN, RANGE_MAX, num_threads);
    END_TIME(time_init);

    /* Sort the array. */
    START_TIME(time_sort);
    counting_sort(array, size, num_threads);
    END_TIME(time_sort);

    /*
     * This is the program's only output; it is meant to be redirected to a CSV
     * file.
     */
    printf("%lld;%d;%.5f;%.5f;%.5f\n", size, num_threads, time_init, time_sort,
                                       time_init + time_sort);

    free(array);
    return EXIT_SUCCESS;
}

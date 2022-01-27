/**
 * @file util.c
 * @brief This file contains some useful, general-purpose functions.
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

#include "util.h"

#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
    #include <omp.h>
#else
    #define omp_get_thread_num() 0
#endif


void *safe_alloc(long long size) {
    if (size < 1) {
        fprintf(stderr, "Can not allocate memory of %lld bytes.\n", size);
        exit(EXIT_FAILURE);
    }

    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Could not allocate memory of %lld bytes.\n", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}


FILE *file_open(const char *path, const char *mode) {
    FILE *f = fopen(path, mode);
    if (f == NULL) {
        fprintf(stderr, "Could not open file '%s'.\n", path);
        exit(EXIT_FAILURE);
    }
    return f;
}


void array_init_random(int *array, long long size, int min, int max,
                       int nthreads)
{
    long long i = 0;

    #pragma omp parallel num_threads(nthreads) shared(array, size) private(i)
    {
        /* Each thread has its own seed to use when calling rand_r(). */
        unsigned seed = time(NULL) ^ omp_get_thread_num();

        #pragma omp for
        for (i = 0; i < size; i++)
            array[i] = rand_r(&seed) % (max + 1 - min) + min;
    }
}


void array_show(const int *array, long long size) {
    printf("----------------------- ARRAY OF %lld ELEMENTS:\n", size);
    for (long long i = 0; i < size; i++) {
        printf("%5d ", array[i]);
        if (i > 0 && i % 10 == 0)
            printf("\n");
    }
    printf("\n\n");
}

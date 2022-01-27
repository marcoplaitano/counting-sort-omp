/**
 * @file counting_sort.c
 * @brief This file contains different versions of the Counting Sort Algorithm.
 * @author Marco Plaitano
 * @date 29 Oct 2021
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

#include "counting_sort.h"

#ifdef _OPENMP
    #include <omp.h>
#endif

#include <stdlib.h>

#include "util.h"


/**
 * @brief Find minimum and maximum values in the array.
 * @param array:    The array.
 * @param size:     Number of elements stored in the array.
 * @param min:      Minimum value (output).
 * @param max:      Maximum value (output).
 * @param nthreads: Number of threads to use when OpenMP parallelization is
 *                  enabled.
 */
static void min_max(const int *array, long long size, int *min, int *max,
                    int nthreads)
{
    *min = array[0];
    *max = array[0];
    long long i = 0;

    /*
     * Critical regions have been introduced to avoid race conditions when
     * either the min or the max have to be updated.
     */
    #pragma omp parallel for num_threads(nthreads) default(shared) private(i)
    for (i = 0; i < size; i++) {
        if (array[i] < *min) {
            #pragma omp critical
            {
                if (array[i] < *min)
                    *min = array[i];
            }
        }
        else if (array[i] > *max) {
            #pragma omp critical
            {
                if (array[i] > *max)
                    *max = array[i];
            }
        }
    }
}


/**
 * @brief Return a positive integer representation of the item to use as index
 *        in an array.
 * @param item: The item to hash.
 * @param max:  Maximum value the key can be (minimum is 0); this value is
 *              usually the size of the array.
 * @return Positive integer key associated to the item.
 *
 * This is similar to an hash function; to be used when the elements stored in
 * the array are not all positive integers or not integers at all.
 */
static long key(int item, long long max) {
    return item;
}


void counting_sort(int *array, long long size, int nthreads) {
    long long i = 0, j = 0, k = 0;
    int max = 0, min = 0;
    min_max(array, size, &min, &max, nthreads);
    long long count_size = max - min + 1;
    int *count = (int *)safe_alloc(sizeof(int) * count_size);

    /* Initialize every item of count[] with the value 0. */
    #pragma omp parallel for num_threads(nthreads) shared(count, count_size) \
            private(i)
    for (i = 0; i < count_size; i++)
        count[i] = 0;

    /*
     * Increment count[j] for every repeated occurrence of array[i] found in the
     * array.
     *
     * Reduction on an array supported from OpenMP version 4.5+.
     */
    #pragma omp parallel for num_threads(nthreads) shared(array, min, size) \
            private(i, j) reduction(+: count[:count_size])
    for (i = 0; i < size; i++) {
        j = key(array[i], size) - min;
        count[j] += 1;
    }

    /*
     * Nested loops in which the iterations of the inner one depend on the value
     * of the outer one: they cannot collapse into one; moreover, there's a
     * data dependence on the k variable. There seems to be no easy way to
     * parallelize this section of the algorithm.
     */
    for (i = min; i <= max; i++)
        for (j = 0; j < count[i - min]; j++)
            array[k++] = i;

    free(count);
}

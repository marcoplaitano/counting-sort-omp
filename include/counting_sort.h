/**
 * @file counting_sort.h
 * @brief This file provides the user a function to sort an array of integers
 *        using Counting Sort Algorithm.
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

#ifndef COUNTING_SORT_H
#define COUNTING_SORT_H


/**
 * @brief Sort the given array using Counting Sort Algorithm.
 *
 * The array is sorted in-place.
 * @param array:    The input array.
 * @param size:     The size of the array.
 * @param nthreads: Number of threads to use when OpenMP parallelization is
 *                  enabled.
 */
void counting_sort(int *array, long long size, int nthreads);


#endif /* COUNTING_SORT_H */

# File:    makefile
# Brief:   Makefile used to automate the process of compiling the source files.
# Author:  Plaitano Marco
# Date:    13 Oct 2021
#
# COUNTING SORT OpenMP
# Parallelize and Evaluate Performances of "Counting Sort" Algorithm, by using
# OpenMP.
#
# Copyright (C) 2022 Plaitano Marco
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

BIN_DIR := bin
INCLUDE_DIR := include
BUILD_DIR := build
OUTPUT_DIR := output
SRC_DIR := src
TEST_DIR := test

CC := gcc
CFLAGS := -g -I $(INCLUDE_DIR)/ -Wno-unused-result
OPT_LEVEL = 0
CLIBS =
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
MAIN := main
EXEC := bin/$(MAIN).out


# Default target: create main executable (no parallelization by default).
$(EXEC): dirs $(OBJS)
	$(CC) $(CFLAGS) -O$(OPT_LEVEL) $(OBJS) $(CLIBS) -o $@


# Create object files.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -O$(OPT_LEVEL) -c $< $(CLIBS) -o $@


.PHONY: serial parallel all test test_serial test_parallel dirs clean


# Compile without parallelization.
serial: $(EXEC)


# Compile with parallelization.
parallel: CLIBS += -fopenmp
parallel: OPT_LEVEL = 1
parallel: $(EXEC)


# Compile all (with parallelization by default).
all: parallel


# Compile test file(s).
test: dirs $(OBJS)
	$(CC) $(CFLAGS) -O$(OPT_LEVEL) -c $(TEST_DIR)/test.c $(CLIBS) -o build/test.o
	rm $(BUILD_DIR)/$(MAIN).o
	$(CC) $(CFLAGS) -O$(OPT_LEVEL) build/*.o $(CLIBS) -o bin/test.out


# Compile serial version of the test file(s).
test_serial: test


# Compile test file(s) enabling OpenMP parallelization.
test_parallel: CLIBS += -fopenmp
test_parallel: OPT_LEVEL = 1
test_parallel: test


# Create needed directories if they do not already exist.
dirs:
	$(shell if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi)
	$(shell if [ ! -d $(BUILD_DIR) ]; then mkdir -p $(BUILD_DIR); fi)


# Delete object files and executables.
# The '-' at the beginning of the line is used to ignore the return code of
# the command.
clean:
	-rm $(BIN_DIR)/* $(BUILD_DIR)/*

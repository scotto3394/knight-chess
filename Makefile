cc = g++ #clang++
cflags = -std=c++17
debug_flags = -Wall -pedantic -Wextra -g
release_flags = -O2
lflags = -pthread -lpersonal -ltcmalloc

bin = ./bin
src = ./src
tmp = ./tmp
inc = ./src/include
test = ./test
bench = $(test)/bench
doc = ./doc

files = main knight 
components = $(patsubst %, $(tmp)/%.o, $(files))
components_test = $(patsubst %, $(test)/test_%.o, $(files))
components_bench = $(patsubst %, $(bench)/bench_%.o, $(files))
components_debug = $(patsubst %.o, %_debug.o, $(components))
debug_target = $(bin)/knight_debug.exe
release_target = $(bin)/knight.exe

all: $(debug_target)

final: $(release_target)

$(debug_target): $(components_debug)
	$(cc) -o $@ $^ $(lflags)

$(release_target): $(components)
	$(cc) -o $@ $^ $(lflags)

$(tmp)/%_debug.o: $(src)/%.cpp $(inc)/%.hpp
	$(cc) $(debug_flags) $(cflags) -c -o $@ $<

$(tmp)/%.o: $(src)/%.cpp $(inc)/%.hpp
	$(cc) $(release_flags) $(cflags) -c -o $@ $<

# Testing
test: $(test)/test
	$<

$(test)/test : $(components_test) $(filter-out $(tmp)/main_debug.o, $(components_debug))
	$(cc) -o $@ $^ $(lflags)

$(test)/%.o : $(test)/%.cpp
	$(cc) $(release_flags) $(cflags) -c -o $@ $^

# Benchmarking
bench: $(bench)/bench
	$<

$(bench)/bench : $(components_bench) $(filter-out $(tmp)/main.o, $(components))
	$(cc) -o $@ $^ $(lflags) -lbenchmark

$(bench)/%.o : $(bench)/%.cpp
	$(cc) $(release_flags) $(cflags) -c -o $@ $^

# Clean
clean:
	rm -f $(tmp)/*.o $(test)/*.o $(bench)/*.o $(bin)/*

.phony: clean test



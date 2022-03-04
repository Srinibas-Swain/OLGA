NAME=hydrogen

#######################################################
###################   DIRECTORIES   ###################
#######################################################
PROGRAM-DIR=~/neo-human/programs/vanity-fair/hydrogen
GTEST-DIR=./3rd-party/googletest
GTEST-DIR-INCLUDE=$(GTEST-DIR)/include
GTEST-DIR-LIB=$(GTEST-DIR)/$(GTEST-DIR-BUILD)
GTEST-DIR-LD-LIBRARY=$(GTEST-DIR-LIB)
TESTS-DIR=tests

#######################################################
###################       SRC       ###################
#######################################################
# todo: this has to be automatic
SRC=graph certificate partition block permutation utils schreiersims find_connected_graphs frequent_induced_subgraph combination_generator iterator combination_iterator bool_iterator vector_fixed graph_pool certificate_cache frequency_counter menu_option all_max_induced_subgraphs unique_certificates csv_iterator csv_row pool certificate_cache2 backward aliases bitmap v_pool generic_object block_pool blocks partition_pool permutation_pool find_connected_graphs_omp ncr chromatic_index find_connected_components vector_int_pool menu_option_helper graph_generator graph_seed all_max_induced_subgraphs_helper chromatic_index_helper degeneracy degeneracy_helper node node_list tree_width tree_width_helper ind_number ind_number_helper genus genus_helper graph_utils graph_basic_properties graph_basic_properties_helper graph_topological_properties graph_topological_properties_helper graph_connectivity graph_connectivity_helper vertex_set

 
SRC-CC=$(foreach src,$(SRC), $(src).cc)
SRC-HH =$(foreach src,$(SRC-CC),$(src:.cc=.hh))
SRC-OBJ=$(foreach src,$(SRC-CC),$(src:.cc=.o))
SRC-OBJ-DOT=$(foreach obj,$(SRC-OBJ),.$(obj))

TEST-CC=$(foreach src,$(SRC), $(src)_test.cc) test_main.cc addition_test.cc
# replace '.cc' with '.o' and prepend a '.'
TEST-OBJ=$(foreach testcc, $(TEST-CC), .$(testcc:.cc=.o))
#TEST-OBJ-PATH=$(foreach testobj, $(TEST-OBJ), $(testobj))

MAIN=compute_certificates playground welcome playground-openmp
MAIN-OBJ=$(foreach main1, $(MAIN), .$(main1).o)

#######################################################
###################      FLAGS      ###################
#######################################################
# CFLAGS=-g -std=c++11 -Wno-c++11-extensions -Wno-deprecated-register -Wunknown-pragmas
CFLAGS=-g -std=c++11 -Wall -Wunknown-pragmas
CFLAGS-TEST=-g -Wall -Wextra -pthread
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	IEIGEN=-I/usr/local/Cellar/eigen/HEAD/include/eigen3/
	GTEST-DIR-BUILD=build-mac
	IBINUTILS=-I/usr/local/opt/binutils/include
	LBINUTILS=-L/usr/local/opt/binutils/lib
	IOMP=-I/usr/local/Cellar/libiomp/20150701/include/libiomp
	#LOMP=-L/usr/local/Cellar/libiomp/20150701/lib
	CC=clang++
	LLIBSOS=-lomp
else ifeq ($(UNAME_S), Linux)
	IEIGEN=`pkg-config --cflags eigen3`
	IOMP=-fopenmp
	GTEST-DIR-BUILD=build-linux
	IBINUTILS=-I/usr/include
	LBINUTILS=-L/usr/lib/debug/usr/lib/x86_64-linux-gnu
	CC=clang++
	LLIBSOS=-fopenmp
endif

IPATH=-I./include -I/usr/local/include $(IEIGEN) -I/usr/include -I./3rd-party/backward-cpp $(IBINUTILS) $(IOMP)
LPATH=-L/usr/local/lib $(LBINUTILS)
LLIBS=-lm -lbfd -ldl $(LLIBSOS)

# # archiving shit
# COMP=tar --create --xz --file
# DATE = `date +%F.%H%M`

#######################################################
###################  COMPILE,BUILD  ###################
#######################################################
COMPILE=-g -c $(IPATH) $(CFLAGS)
COMPILE_FILE = $(CC) $(COMPILE) $(basename $(1)).cc -o .$(basename $(notdir $(1))).o
BUILD=$(CC) $(LPATH) $(LLIBS) -o $(NAME) -v

#######################################################
######################  TESTING  ######################
#######################################################
GTEST-LIBS=-lgtest -lgtest_main
IPATH-TEST=-isystem $(GTEST-DIR-INCLUDE) -I$(GTEST-DIR)
LPATH-TEST=$(LPATH) -L$(GTEST-DIR-LIB)
LLIBS-TEST=$(LLIBS) $(GTEST-LIBS)

all: welcome

init: gtest-build

init-force: gtest-build

gtest-build:
	./scripts/google-test-setup.sh "$(GTEST-DIR)" "$(GTEST-DIR-BUILD)"

mem: test
	valgrind --leak-check=full ./$(NAME)

$(NAME): $(SRC-OBJ-DOT)
	$(BUILD)

tags:
	find . -type f -regex ".*\.\(cc\|hh\|rb\)" | xargs etags -a

test_make:
	@echo testing make
	@echo $(IEIGEN)

test: .test_main
	LD_LIBRARY_PATH=$(GTEST-DIR-LIB) $(TESTS-DIR)/test_main

.test_main: $(TESTS-DIR)/test_main.cc $(TEST-OBJ) $(SRC-OBJ-DOT)
	@echo test_main
	$(CC) $(IPATH-TEST) $(CFLAGS-TEST) -c $(GTEST-DIR)/src/gtest-all.cc -o .gtest-all.o
	$(CC) $(IPATH-TEST) $(CFLAGS-TEST) -o $(TESTS-DIR)/test_main .gtest-all.o $(TEST-OBJ) $(SRC-OBJ-DOT)

# wrap target between '.' and '.o' so that 'target1' becomes '.target1.o'
$(MAIN): %: .%.o $(SRC-OBJ-DOT)
	$(CC) $(LPATH) $(SRC-OBJ-DOT) $< -o $@ $(LLIBS)

$(TEST-OBJ): .%.o: $(TESTS-DIR)/%.cc
	$(CC) $(COMPILE) $(IPATH-TEST) $< -o $@

# rm first '.' and replace '.o' with '.cc': '.target.o' becomes 'target.cc'
$(MAIN-OBJ): .%.o: %.cc
	$(call COMPILE_FILE,$<)

.%.o: src/%.cc include/%.hh
	@echo $<
	$(call COMPILE_FILE, $<)


clean:
#	@echo $(DOT_OBJS)
	rm -rf $(NAME) $(SRC-OBJ-DOT) *.o .test.o .test* *log*
	rm -rf .*.o
	rm -f welcome playground

clean-all: clean
	rm -rf $(GTEST-DIR)/$(GTEST-DIR-BUILD)

# comp: clean
# 	$(COMP) $(ARCH-DIR)/$(NAME).`date +%F.%H%M`.tar.xz ../$(NAME)/ $(UTILS-DIR)




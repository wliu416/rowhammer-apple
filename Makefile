#CC=g++
#build_env.mk sets CC.

# $(call log) comes from logging.mk
include logging.mk

# You can include build_env.mk into your own
# Makefile to get the cross compilation flags
include build_env.mk

all: bin bin/histogram bin/bank_mapper bin/row_bxor_mapper bin/bxor_manipulation_experiment bin/pagemap bin/hammering


bin/histogram: src/histogram/histogram.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/histogram/histogram.cc src/shared.cc

bin/histogram2: src/histogram/histogram2.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/histogram/histogram2.cc src/shared.cc


bin/bank_mapper: src/hammering/bank_mapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/hammering/bank_mapper.cc src/shared.cc

bin/row_bxor_mapper: src/hammering/row_bxor_mapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/hammering/row_bxor_mapper.cc src/shared.cc

bin/bxor_manipulation_experiment: src/hammering/bxor_manipulation_experiment.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/hammering/bxor_manipulation_experiment.cc src/shared.cc

bin/rowmapper: src/reverse_engineering/rowmapper.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/reverse_engineering/rowmapper.cc src/shared.cc

bin/hammering: src/hammering/hammering.cc src/shared.cc src/shared.hh src/params.hh src/util.hh
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/hammering/hammering.cc src/shared.cc

bin/pagemap: src/pagemap.cc
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ src/pagemap.cc

bin:
	mkdir bin

# Removed before the copy, @$(log_install) \n cp hello ${CRYPTEX_BIN_DIR} \n cp hello.plist ${CRYPTEX_LAUNCHD_DIR}
.PHONY: install
install:
	cp /bin/* ${CRYPTEX_BIN_DIR}

.PHONY: clean
clean:
	rm -rf bin/
.PHONY: bin test

bin: makebin.mk
	@make -f makebin.mk --no-print-directory

test:
	@make -f maketest.mk --no-print-directory

help:
	@echo "usage: {bin|test}"

clean:
	@make -f makebin.mk --no-print-directory clean
	@make -f maketest.mk --no-print-directory clean
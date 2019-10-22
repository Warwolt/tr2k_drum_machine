.PHONY: bin test

help:
	@echo "usage: {bin|test}"

bin: makebin.mk
	@make -f makebin.mk --no-print-directory

flash: makebin.mk
	@make -f makebin.mk flash --no-print-directory

test:
	@make -f maketest.mk --no-print-directory

clean:
	@make -f makebin.mk --no-print-directory clean
	@make -f maketest.mk --no-print-directory clean

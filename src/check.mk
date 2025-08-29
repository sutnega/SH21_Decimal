CHECK_INCLUDES=tests/includes.check
CHECK_FILES=$(filter-out $(CHECK_INCLUDES), $(wildcard tests/*.check))

_unittests.check: $(CHECK_INCLUDES) $(CHECK_FILES)
	$(MAKE) -s test-check_numbering
	cat $^ > tests/$@

unittests: _unittests.check
	rm -f tests/$@
	checkmk clean_mode=1 tests/$^ > tests/s21_test.c
	rm -f tests/$^
	clang-format $(CODE_STYLE) -i tests/s21_test.c

_unittests-forking_disable: unittests
ifeq ($(shell uname),Linux)
	sed -i 's/srunner_run_all/srunner_set_fork_status(sr, CK_NOFORK);\n\n  srunner_run_all/' tests/s21_test.c
endif
ifeq ($(shell uname),Darwin)
	sed -i '' 's/srunner_run_all/srunner_set_fork_status(sr, CK_NOFORK);\n\n  srunner_run_all/' tests/s21_test.c
endif

test-check_numbering:
	@for file in $$(find tests -name "*.check"); do \
		cp $$file $$file.bak; \
		awk -v count=1 -v prev="" ' \
		/^#test/ { \
			split($$0, a, " "); \
			gsub(/[0-9]+$$/, "", a[2]); \
			if(a[2] != prev){ \
				count = 1; \
				prev = a[2]; \
			} \
			printf "%s %s%d", a[1], a[2], count++; \
			for (i=3; i<=length(a); i++) printf " %s", a[i]; \
			print ""; \
		} \
		!/^#test/ {print $$0}' $$file.bak > $$file; \
		rm $$file.bak; \
	done
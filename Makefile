.PHONY:test docs

all:test docs lint

lint:bin/logs/lint.txt

OPENSSL_PATH=$(subst openssl=,-I,$(OS_OPTIONS))/include

PLATFORM = $(shell uname)

ifeq ($(PLATFORM),Darwin)
  CLANG_FORMAT_FLAGS = --verbose
  SANITIZERS = -fsanitize=address -fsanitize-address-use-after-scope -fsanitize=undefined
endif

ifeq ($(PLATFORM),Linux)
  USE_OPENSSL = -DOpenSSLAvailable=1 -lcrypto
endif

KNOWN_ERRORS:= --suppress=unusedFunction \
    			--inline-suppr

check:format lint docs

bin/logs/lint.txt: *.h
	@echo Linting ...
	@mkdir -p bin/logs
	@cppcheck --enable=all --force --std=c++11 $(KNOWN_ERRORS) --language=c++ $(OPENSSL_PATH) -I.. *.h &> $@
	@-cat $@ | grep performance: || true
	@-cat $@ | grep portability: || true
	@-cat $@ | grep style: || true
	@-cat $@ | grep warning: || true
	@-cat $@ | grep error: || true
	@grep -rniw todo *.h
	@echo `grep -rniw todo *.h | wc -l` TODO items
	@cat bin/coverage/*/*.gcov | grep -E '[0-9]+:' | grep -ve -: | grep -v "#####" > bin/logs/all_code_coverage.txt
	@grep // bin/logs/all_code_coverage.txt | grep -i test | sort | uniq  || true
	@echo `grep // bin/logs/all_code_coverage.txt | grep -i test | sort | uniq | wc -l` lines now tested

documentation/index.html:
	@mkdir -p documentation
	@doxygen protocol.dox 2> bin/logs/doxygen.txt
	@if [ `cat bin/logs/doxygen.txt | wc -l` -ne "0" ]; then echo `cat bin/logs/doxygen.txt | wc -l` documentation messages; fi

docs:documentation/index.html

test:bin/test
	@bin/test $(OS_OPTIONS) $(COMPILER) $(TEST)

bin/test:format
test:format
docs:format
lint:format

format:bin/logs/clang-format.txt

bin/logs/clang-format.txt:tests/*.cpp *.h
	@echo Cleaning code ...
	@mkdir -p bin/logs/
	@clang-format $(CLANG_FORMAT_FLAGS) -i *.h tests/*.cpp 2> bin/logs/clang-format.txt

../os/tests/test.cpp:
	@git clone http://github.com/marcpage/os ../os

../os/*.h:../os/tests/test.cpp

# -fsanitize=memory
# -fsanitize=thread
# -flto -fsanitize=cfi
# -fsanitize=leak
# -fsanitize=safe-stack
# -D_LIBCPP_DEBUG=1
bin/test:../os/tests/test.cpp ../os/*.h *.h
	@mkdir -p bin
	@clang++ ../os/tests/test.cpp -o $@ $(USE_OPENSSL) -I.. -std=c++11 -lsqlite3 -Wall -Weffc++ -Wextra -Wshadow -Wwrite-strings $(SANITIZERS) -fno-optimize-sibling-calls -O0 -g

clean:
	@rm -Rf documentation bin/coverage bin/test bin/tests bin/logs/*.log bin/logs/*.txt

# bin/coverage/DateTime_clang++_trace/DateTime.h.gcov| sed -E 's/^([^:]+:)([^:]+:)/\2\1/' | sort | uniq

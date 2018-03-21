.PHONY:test

test:bin/test
	@bin/test $(OS_OPTIONS)

os/tests/test.cpp os/*.h:
	@-git clone http://github.com/marcpage/os

bin/test:os/tests/test.cpp os/*.h
	@mkdir -p bin
	@clang++ os/tests/test.cpp -o $@ -I. -Wall -Weffc++ -Wextra -Wshadow -Wwrite-strings

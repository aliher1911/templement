test : main.cpp
	clang++ -std=c++11 -stdlib=libc++ -Weverything -Wno-c++98-compat -O0 -g -o test main.cpp

clean :
	rm test
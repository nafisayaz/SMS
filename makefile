

all: remove compile run
	
remove:
	rm -rf ./sms.exe

compile:
	i586-mingw32msvc-g++ sms.cpp -lwininet -o sms.exe

run:
	./sms.exe



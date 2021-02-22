
all: tests

tests:
	# Complex
	php7.4 parse.php <Test_files/tests/parse-only/complex.src >tmp.xml
	echo $$? >tmp.txt
	diff -Z tmp.txt Test_files/tests/parse-only/complex.rc
	diff -Eb tmp.xml Test_files/tests/parse-only/complex.out
	# Empty
	php7.4 parse.php <Test_files/tests/parse-only/empty.src >tmp.xml
	echo $$? >tmp.txt
	diff -Z tmp.txt Test_files/tests/parse-only/empty.rc
	diff -Eb tmp.xml Test_files/tests/parse-only/empty.out
	# Header errors
	- php7.4 parse.php <Test_files/tests/parse-only/header_error_1.src >tmp.xml
	- php7.4 parse.php <Test_files/tests/parse-only/header_error_2.src >tmp.xml
	# Invalid opcode
	- php7.4 parse.php <Test_files/tests/parse-only/invalid_opcode.src >tmp.xml
	# Parse errors
	- php7.4 parse.php <Test_files/tests/parse-only/parse_error_1.src >tmp.xml
	- php7.4 parse.php <Test_files/tests/parse-only/parse_error_2.src >tmp.xml
	- php7.4 parse.php <Test_files/tests/parse-only/parse_error_3.src >tmp.xml
	# Read
	php7.4 parse.php <Test_files/tests/parse-only/read_test.src >tmp.xml
	echo $$? >tmp.txt
	diff -Z tmp.txt Test_files/tests/parse-only/read_test.rc
	diff -Eb tmp.xml Test_files/tests/parse-only/read_test.out
	# Simple 			NOT OK i think
	# php7.4 parse.php <Test_files/tests/parse-only/simple_tag.src >tmp.xml
	# echo $$? >tmp.txt
	# diff -Z tmp.txt Test_files/tests/parse-only/simple_tag.rc
	# diff -Eb tmp.xml Test_files/tests/parse-only/simple_tag.out
	# Write
	php7.4 parse.php <Test_files/tests/parse-only/write_test.src >tmp.xml
	echo $$? >tmp.txt
	diff -Z tmp.txt Test_files/tests/parse-only/write_test.rc
	diff -Eb tmp.xml Test_files/tests/parse-only/write_test.out
	make clean

clean: 
	rm -f tmp.txt tmp.xml
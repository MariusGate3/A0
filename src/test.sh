#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
# EMPTY TEST
printf "" > test_files/empty.input

# ASCII TESTS #
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "  Hello, World!" > test_files/ascii3.input
printf "12345678910/!)" > test_files/ascii4.input

# ISO TESTS
printf "Jeg spiste 5 \xE6bler\n" > test_files/iso.input
printf "Hvussu gongur ta\0xF0?" > test_files/iso2.input
printf "\0xDEakka \0xFE \0xE9r" > test_files/iso3.input


# UTF-8 TESTS
printf "😅" > test_files/utf8.input
printf "Hvussu gongur tað?" > test_files/utf8-2.input

# DATA TESTS #
printf "Hello,\x00World!\n" > test_files/data.input

### TODO: Generate more test files ###


echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    ${f} | sed -e 's/ASCII text.*/ASCII text/' \
                         -e 's/UTF-8 Unicode text.*/UTF-8 Unicode text/' \
                         -e 's/Unicode text, UTF-8 text.*/UTF-8 text/' \
                         -e 's/ISO-8859 text.*/ISO-8859 text/' \
                         -e 's/writable, regular file, no read permission/cannot determine (Permission denied)/' \
                         > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done
exit $exitcode

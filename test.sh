test_compiler(){
    input_value="$1"
    expected_value="$2"

    ./smol "$input_value" > tmp.s
    nasm -f elf64 tmp.s -o tmp.o
    gcc tmp.o -o tmp
    ./tmp
    result="$?"

    if [ "$result" = "$expected_value" ]; then
        echo "Passed the test"
    else
        echo "Expected $expected_value, returned $result"
        exit 1
    fi
}
test_compiler 1 1
test_compiler 69 69
test_compiler 5+2+3 10
test_compiler 45-5-6 34
test_compiler '1 + 5 + 3' 9
test_compiler '1*2+4' 6
test_compiler '1 * 5 * 5' 25

echo OK
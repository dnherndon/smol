test() {
  expected="$1"
  executable="$2"

  ./test/"$executable"
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "passed: $executable"
  else
    echo "failed: $executable   expected $expected -> actual result $actual"
  fi
}
echo "\nRunning Generated Executables:\n"
test 55 add.exe
test 7 complex_expression.exe
test 5 divide.exe
test 20 multiply.exe
test 1 subtract.exe
test 15 unary_minus.exe
test 13 nullary_func.exe
test 21 empty.exe
test 1 equivalent.exe
test 1 not_equivalent.exe
test 3 modulo.exe
test 3 variable.exe
test 5 multivariable.exe
test 7 complexAssignment.exe

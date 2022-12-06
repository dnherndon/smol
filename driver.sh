test() {
  expected="$1"
  executable="$2"

  ./test/"$executable"
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "passed: $executable"
  else
    echo "failed: $executable   expected $expected -> actual result $actual"
    exit 1
  fi
}
test 3 add.exe
test 7 complex_expression.exe
test 5 divide.exe
test 20 multiply.exe
test 1 subtract.exe
test 15 unary_minus.exe
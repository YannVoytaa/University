prog=$1
dir=$2
for f in "$dir"/*.in
do
  if [ -n "$3" ] #jesli wpisze sie trzeci parametr to na testach uruchomi sie tez valgrind
    then valgrind --error-exitcode=123 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all ./"$prog" <"$f" >/dev/null
  fi
  ./"$prog" <"$f" > "temp.out" 2>"temp.err"
  if diff "temp.out" "${f%in}out" >/dev/null 2>/dev/null && diff "temp.err" "${f%in}err" >/dev/null 2>/dev/null
    then echo "$f OK"
    else echo "$f Prawie"
  fi
done
rm -f temp.out temp.err
let lista=[5;4;3;2;1];;
let f l=
  let rec pom l acc=
    if l=[] then acc
    else pom (tl l) (hd l :: acc)
  in
  pom l [];;

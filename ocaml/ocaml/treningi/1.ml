(*let rzadka x=
  let rec check liczba=
    if liczba=0 then 1
    else if liczba mod 4 = 3 then 0
    else check (liczba/2)
  in
  let rec loop n=
    if check n =1 then n
    else loop (n+1)
  in
  loop (x+1);;*)


(*let rec rzadkie x=
  let rec check liczba=
    if liczba=0 then 1
    else if liczba mod 4=3 then 0
    else check(liczba/2)
  in
  let rec loop n=
    if n=0 then 0
    else check n + loop (n-1)
  in
  loop x;;*)


(*let pierwsza x=
  let rec loop n=
    if n=1 then 1
    else if x mod n =0 then 0
    else loop (n-1)
  in
  loop (x-1);;*)


(*let koduj x y=
  let rec tworz n m wynik=
    if n=0 && m=0 then wynik
    else tworz (n/10) (m/10) ((wynik*10+n mod 10)*10+m mod 10)
  in
  tworz x y 0;;

let odkoduj x=
  let rec f liczba n m=
    if liczba = 0 then (m,n)
    else f (liczba/100) (n*10+liczba mod 10) (m*10+liczba/10 mod 10)
  in
  f x 0 0;;*)




let modulo x=
  let rec loop n=
    if n<=1 then 0
    else if (n*n) mod x = 1 then n
    else loop (n-1)
  in
  loop (x-2);;

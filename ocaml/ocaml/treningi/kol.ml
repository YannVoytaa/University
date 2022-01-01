(*let staly f=
  let rec bin l p f=
    if l=p then l
    else let avg=(l+p)/2
    in
    if f avg > avg then bin (avg+1) p f
    else bin l avg f
    in
  bin ~-1000000 1000000 f;;
*)

(*let rozne ls ls2=
  let min a b= if a< b then a else b
  in
  let rec pom ls ls2 odp last=match ls,ls2 with
    [],[]->odp|

    [],h::t -> if h=last then pom ls t  odp last
               else pom ls t (odp+1) h|

    h::t,[] -> if h=last then pom t ls2 odp last
               else pom t ls2 (odp+1) h|

    h::t, h2::t2 -> if h2<h then pom ls2 ls odp last
                    else if h=last then pom t ls2 odp last
                    else pom t ls2 (odp+1) h
    in
    match ls,ls2 with
    [],[]->0|
    [],h::t->pom ls ls2 1 h|
    h::t,[]->pom ls ls2 1 h|
    h::t,h2::t2->pom ls ls2 1 (min h h2);;*)


(*    let sumuj ls=
      let rec suma_listy ls odp=match ls with
        []->odp|
        h::t-> suma_listy t (odp+h)
      in
      let suma_tej_listy=suma_listy ls 0
      in
      let rec loop ls n (odp,dodawane,suma)=    (*niezmienniki: suma n pierwszych elementow z ls = suma, n pierwszych elementow + dodawane = ls*)
        let rec dodaj ile (odp,dodawane,suma)=
          if ile=0 then (((suma_tej_listy-suma)::odp),dodawane,suma)
          else match dodawane with
            []-> (((suma_tej_listy-suma)::odp),dodawane,suma)|
            h::t -> dodaj (ile-1) (odp,t,(suma+h))
        in
        match ls with
        []->odp|
        h::t->loop t (h-1) (dodaj (h-1-n) (odp,dodawane,suma))   (*chcemy sume h pierwszych elementow majac sume n pierwszych, wiec dodajemy h pierwszych elementow z listy dodawane*)
      in
      List.rev (loop ls 0 ([],ls,0));;
*)


(*let podziel ls=
  let maksimum x y=if x>y then x else y
  in
  let rec pom ls max n odp akt=match ls with
    []->odp|
    h::t-> if maksimum max h=n+1 then pom t (maksimum max h) (n+1) (List.rev (h::akt)::odp) []
           else pom t (maksimum max h) (n+1) odp (h::akt)
  in
  List.rev (pom ls 0 0 [] []);;*)


(*  let malo ls=
    let min x y=if x<y then x else y
    in
    let rec pom ls ls2 odp poz1 poz2=match ls,ls2 with
      [],_->odp|
      _,[]->odp|
      h::t,h2::t2 ->if poz1=poz2 then odp
                    else if h+h2>0 then pom ls t2 (min (h+h2) odp) poz1 (poz2-1)
                    else (*h+h2<=0*) pom t ls2 (min (-(h+h2)) odp) (poz1+1) poz2

  in
  pom ls (List.rev ls) 1000000 1 (List.length ls);;*)





let podziel modulo ls=
  let rec pom modulo ls odp ind=match ls with
  []->odp|
  h::t -> if ind mod modulo =0 then pom modulo t (h::odp) (ind+1)
          else pom modulo t odp (ind+1)
  in
  let rec loop ls odp_real modulo ind=match ls with
    []->odp_real|
    h::t->if ind>=modulo then odp_real
          else loop t (List.rev (pom modulo ls [] 0)::odp_real) modulo (ind+1)
  in
  List.rev (loop ls [] modulo 0);;

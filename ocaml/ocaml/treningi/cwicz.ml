(*let rec tails ls =match ls with
  []->[]::[] |
  glowa::ogon -> ls::tails ogon;;*)


(*let rozne ls ls2=
  let rec pom ls ls2 wynik=match (ls,ls2) with
    ([],[])->wynik|
    ([],glowa2::ogon2)->wynik+List.length ls2|
    (glowa::ogon,[])->wynik+List.length ls|
    (glowa::ogon,glowa2::ogon2)->if glowa<glowa2 then pom ogon ls2 (wynik+1)
                                 else if glowa=glowa2 then pom ogon ls2 wynik
                                 else pom ls ogon2 (wynik+1)
    in
    let rec redukuj ls=match ls with
      []->[]|
      glowa::[]->[glowa]|
      glowa::glowa2::ogon->if glowa=glowa2 then redukuj (glowa2::ogon)
                           else glowa::redukuj (glowa2::ogon)
    in
    pom (redukuj ls) (redukuj ls2) 0;;*)


(*let max_and_min ls=
  let maksimum x y=
    if x>y then x else y
  in
  let minimum x y=
    if x<y then x else y
  in
  let rec pom ls max min= match ls with
    []-> (max,min)|
    glowa::ogon -> pom ogon (maksimum max glowa) (minimum min glowa)
  in
  pom ls (-1000000) 1000000;;*)


(*  let lider ls=
    let rec pom ls war il=match ls with
      []->war|
      glowa::ogon -> if glowa=war then pom ogon war (il+1)
                     else if il=0 then pom ogon glowa 1
                     else pom ogon war (il-1)
      in pom ls 0 0;;*)



(*let mieszaj ls=
  let rec pom ls ls2 odp=match (ls,ls2) with
      ([],[])->odp|
      (glowa::ogon, glowa2::ogon2) -> pom ogon ogon2 (glowa::glowa2::odp)
  in
  let rec skroc ls n odp=match ls with
    []->odp|
    glowa::ogon -> if n=0 then odp
                   else skroc ogon (n-1) (glowa::odp)
  in
  List.rev (skroc (pom (List.rev ls) ls []) (List.length ls) []);;*)

  let maksimum x y=if x>y then x else y;;

(*let palindrom_np ls=
  let rec pom ls odp behind=
    let rec check ls1 ls2 akt_odp max=match (ls1,ls2) with
      ([],_)->maksimum max akt_odp|
      (_,[])->maksimum max akt_odp|
      (glowa::ogon,glowa2::ogon2)->if glowa=glowa2 then check ogon ogon2 (akt_odp+2) (maksimum max (akt_odp+2))
                                   else maksimum max akt_odp
    in
    match ls with
    []-> odp |
    glowa::ogon -> pom ogon (check ogon behind 1 odp) (glowa::behind)
  in
  pom ls 0 [];;

let palindrom_p ls=
  let rec pom ls odp behind=
    let rec check ls1 ls2 akt_odp max=match (ls1,ls2) with
      ([],_)->maksimum max akt_odp|
      (_,[])->maksimum max akt_odp|
      (glowa::ogon,glowa2::ogon2)->if glowa=glowa2 then check ogon ogon2 (akt_odp+2) (maksimum max (akt_odp+2))
                                   else maksimum max akt_odp
    in
    match ls with
    []-> odp |
    glowa::glowa2::ogon ->if glowa=glowa2 then pom (glowa2::ogon) (check ogon behind 2 odp) (glowa::behind)
                          else pom (glowa2::ogon) odp (glowa::behind)|
    glowa::[]->odp
  in
  pom ls 0 [];;

let palindrom ls=maksimum (palindrom_p ls) (palindrom_np ls);;*)

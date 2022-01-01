type wartosc = Pojedynczy of float*float |                                  (*Przedzial <a.b>, a<=b*)
                 Podwojny of float*float |                                    (*Przedzial (-infty, a) \cup (b,+infty), a<=b*)
                 Pusty;;                                                  (*{}*)

let first x=match x with
  Pusty->0.|
  Pojedynczy (a,_) ->a|
  Podwojny (a,_) -> a;;
let second x=match x with
  Pusty->0.|
  Pojedynczy (_,b) ->b|
  Podwojny (_,b) -> b;;
let min a b= if a<b then a else b;;

let wartosc_dokladnosc x p =Pojedynczy(x-.x*.p/.100.,x+.x*.p/.100.);;             (*= Przedzial <x-p%x,x+p%x>*)

let wartosc_od_do x y = Pojedynczy(x,y);;                                 (*Przedzial <x,y>*)

let wartosc_dokladna x = Pojedynczy(x,x);;                                (*{x}=<x,x>*)

let in_wartosc x y = match (x,y) with
    (Pusty,_) -> false |                                                  (*Zbiór pusty nie może być równy niczemu innemu [ew. jeśli zakładamy, że np 5/0 = 5/0, to dla (Pusty,Pusty)->true]*)
    (_,Pusty) -> false |
    (Pojedynczy (x,x2),Pojedynczy (y,y2)) -> if (min x2 y2) >= (max x y) then true else false | (*Jeśli <a,b> \cap <c,d> !={} i a<=c to b>=c*)
    (Podwojny (_,_),Podwojny (_,_)) -> true |                                                     (*[(-infty,a) \cup ...]  \cap [-infty, c] \cup ...] \subseteq (-infty, min(a,c)) != {}*)
    (Podwojny (x,x2),Pojedynczy (y,y2)) -> x >= y || x2 <= y2  |              (*[(-infty,a) \cup (b,+infty)] \cap <c,d> !={} => c<=a lub d>=b*)
    (Pojedynczy (y,y2),Podwojny (x,x2)) -> x >= y || x2 <= y2  ;;

let min_wartosc x = match x with
    Pusty -> infinity       |                                             (*inf({}) = +infty*)
    Pojedynczy (x,_) -> x |
    Podwojny (_,_) -> neg_infinity;;

let max_wartosc x = match x with
    Pusty -> neg_infinity       |                                             (*sup({}) = -infty*)
    Pojedynczy (_,x2) -> x2 |
    Podwojny (_,_) -> infinity;;


let sr_wartosc x = (max_wartosc x +. min_wartosc x)/.2.;;                        (*infty+neg_infty=nan, nie da się dostać przedziału (-infty,-infty) lub (infty,infty)*)

let plus a b = match (a,b) with
    (Pusty,_) -> Pusty |
    (_,Pusty) -> Pusty |
    (Pojedynczy (x,x2),Pojedynczy (y,y2)) -> wartosc_od_do (x +. y)  (x2 +. y2) | (*minimalna osiagalna wartosc to suma minimalnych osiagalnych wartosci, analogicznie z maks*)
    (Podwojny (x,x2),Podwojny (y,y2)) -> Podwojny(max x y, min x2 y2 ) |
    (Podwojny (x,x2),Pojedynczy (y,y2)) -> if x +. y2 >= x2 +. y then wartosc_od_do neg_infinity infinity
                                          else Podwojny(x+.y2,x2 +. y)  |
    (Pojedynczy (y,y2),Podwojny (x,x2)) -> if x +. y2 >= x2 +. y then wartosc_od_do neg_infinity infinity
                                          else Podwojny(x+.y2,x2 +. y) ;;

let przeciwny x = match x with
    Pusty -> Pusty |
    Pojedynczy (x,x2) -> wartosc_od_do (-.x2) (-.x)|
    Podwojny (x,x2) -> Podwojny ((-.x2), (-.x));;

let minus a b = plus a (przeciwny b)

let razy a b = match (a,b) with
    (Pusty,_) -> Pusty |
    (_,Pusty) -> Pusty |
    (Pojedynczy (x,x2),Pojedynczy (y,y2)) ->if min x y =neg_infinity && max x2 y2 =infinity then wartosc_od_do  neg_infinity infinity
                                  else wartosc_od_do (min (min  (x *. y) (x2 *. y2)) (min (x *. y2) (x2 *. y))) (max (max (x *. y) (x2 *. y2)) (max (x *. y2) (x2 *. y)))   |
    (Podwojny (x,x2),_) -> wartosc_od_do neg_infinity infinity |
    (_,Podwojny (x,x2)) -> wartosc_od_do neg_infinity infinity;;


let odwrotny x=match x with
    Pusty-> Pusty |

    Pojedynczy (x,x2) -> if x=0. && x2=0. then Pusty
                         else if x=neg_infinity && x2=infinity then wartosc_od_do neg_infinity infinity
                         else if x<0. && x2>0. then Podwojny(1./.x,1./.x2)
                         else if x<0. && x2=0. then wartosc_od_do neg_infinity (1./.x)
                         else if x=0. && x2>0. then wartosc_od_do (1./.x2) infinity
                         else if x<0. (*x2<0*) then wartosc_od_do (1./.x2) (1./.x)
                         else (*x>0 and x2>0*) wartosc_od_do (1./.x2) (1./.x)|

    Podwojny(x,x2) -> if x<0. && x2<0. then Podwojny(1./.x2,1./.x)
                      else if x>0. && x2>0. then Podwojny(1./.x,1./.x2)
                      else if x<0. && x2=0. then wartosc_od_do (1./.x) infinity
                      else if x=0. && x2>0. then wartosc_od_do neg_infinity (1./.x2)
                      else (*x<0 and x2>0*) wartosc_od_do (1./.x) (1./.x2);;

let podzielic a b = razy a (odwrotny b);;

let a=wartosc_od_do neg_infinity infinity;;
let b=wartosc_od_do neg_infinity (-5.);;
let c=wartosc_od_do neg_infinity 0.;;
let d=wartosc_od_do neg_infinity 7.;;
let e=wartosc_od_do (-5.) infinity;;
let f=wartosc_od_do 0. infinity;;
let g=wartosc_od_do 7. infinity;;
let h=wartosc_od_do (-10.) (-6.);;
let i=wartosc_od_do (-10.) 0.;;
let j=wartosc_od_do (-10.) 9.;;
let k=wartosc_od_do 0. 3.;;
let l=wartosc_od_do 0. 5.;;
let m=wartosc_od_do 3. 5.;;
let n=wartosc_od_do 0. 0.;;

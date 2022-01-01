(*Autor: Jan Wojtach, grupa 3*)

type wartosc = Pojedynczy of float*float |                                  (*Przedzial <a.b>, a<=b*)
               Podwojny of float*float |                                    (*Przedzial (-infty, a) \cup (b,+infty), a<=b*)
               Pusty;;                                                      (*{}*)
exception Blad;;
let first x=match x with                                                    (*Zwraca poczatek pojedynczego przedzialu lub koniec pierwszego z przedziałow w podwojnym*)
    Pusty -> raise Blad|
    Pojedynczy (a,_) -> a|
    Podwojny (a,_)   -> a;;
let second x=match x with                                                   (*Zwraca koniec pojedynczego przedzialu lub poczatek drugiego z przedzialow w podwojnym*)
    Pusty -> raise Blad|
    Pojedynczy (_,b) -> b|
    Podwojny (_,b)   -> b;;


let min a b= if a<b then a else b;;

let wartosc_dokladnosc x p =Pojedynczy(min (x-.x*.(p/.100.)) (x+.x*.(p/.100.)),max (x-.x*.(p/.100.))(x+.x*.(p/.100.)));;             (*= Przedzial <x-p%x,x+p%x>, (min i max po to, by zachowac niezmiennik a<=b)*)

let wartosc_od_do x y = Pojedynczy(x,y);;                                 (*Przedzial <x,y>*)

let wartosc_dokladna x = Pojedynczy(x,x);;                                (*{x}=<x,x>*)

let in_wartosc x y = match x with
    Pusty             -> false |                                          (*Zbior pusty nie zawiera zadnej wartosci, wiec w szczegolnosci nie zawiera y*)
    Pojedynczy (x,x2) -> x<=y && y<=x2 |                                  (*Musi zajsc a<=y<=b*)
    Podwojny (x,x2)   -> y<=x || y>=x2 ;;                                 (*Musi zajsc -inf<=y<=a || b<=y<=inf, nierownosci z nieskonczonosciami sa zawsze prawdziwe*)

let min_wartosc x = match x with
    Pusty            -> infinity       |                                  (*inf({}) = +infty*)
    Pojedynczy (x,_) -> x |
    Podwojny (_,_)   -> neg_infinity;;

let max_wartosc x = match x with
    Pusty             -> neg_infinity  |                                  (*sup({}) = -infty*)
    Pojedynczy (_,x2) -> x2 |
    Podwojny (_,_)    -> infinity;;


let sr_wartosc x = (max_wartosc x +. min_wartosc x)/.2.;;                 (*infty+neg_infty=nan (wiec dla przedzialu -inf,inf zwraca nan, dla (a,inf) -> inf, dla (-inf,a) -> -inf, dla reszty dziala "standardowo"), nie da się dostać przedziału (-infty,-infty) lub (infty,infty), nie ma to sensu, oznaczaloby to mniej wiecej "przedzial ktory ma liczby wieksze od dowolnie duzej liczby/ mniejsze od dowolnie malej liczby"*)

let rec plus a b = match (a,b) with
    (Pusty,_) -> Pusty |
    (_,Pusty) -> Pusty |
    (Pojedynczy (x,x2),Pojedynczy (y,y2)) -> wartosc_od_do (x +. y)  (x2 +. y2) |                                                (*minimalna osiagalna wartosc to suma minimalnych osiagalnych wartosci, analogicznie z maks*)
    (Podwojny (x,x2),Podwojny (y,y2))     -> wartosc_od_do neg_infinity infinity |                                               (*a=(x-y2)+(y2+a) || a=(y-x2)+(x2+a), x-y2, x2+a \in pierwszy, y-x2, y2+a \in drugi*)
    (Podwojny (x,x2),Pojedynczy (y,y2))   -> if x +. y2 >= x2 +. y then wartosc_od_do neg_infinity infinity                      (*Podwojny (a,b) zawiera wszystkie wartosci poza przedzialem (a,b). Jesli do liczb z przedzialu (a-y2,a) dodamy y2 to osiagniemy wartosci z przedzialu (a,a+y2), analogicznie z drugiej strony. Jesli przedzialy beda na siebie nachodzic, to mamy przedzial (-inf,inf)*)
                                             else  Podwojny(x+.y2,x2 +. y)  |
    (Pojedynczy (y,y2),Podwojny (x,x2))   -> plus (Podwojny(x,x2)) (Pojedynczy(y,y2)) ;;

let przeciwny x = match x with
    Pusty -> Pusty |
    Pojedynczy (x,x2) -> wartosc_od_do (-.x2) (-.x)|
    Podwojny (x,x2) -> Podwojny ((-.x2), (-.x));;

let minus a b = plus a (przeciwny b);;

let zero_zamiast_nan x=if (x=x)=false then 0. else x;;

let rec razy a b = match (a,b) with
    (Pusty,_) -> Pusty |
    (_,Pusty) -> Pusty |

    (Pojedynczy (x,x2),Pojedynczy (y,y2)) ->  (*Tu byly ify*)
                                            wartosc_od_do  (min (min (zero_zamiast_nan (x *. y)) (zero_zamiast_nan (x2 *. y2))) (min (zero_zamiast_nan(x *. y2)) (zero_zamiast_nan(x2 *. y))))
                                                           (max (max (zero_zamiast_nan (x *. y)) (zero_zamiast_nan (x2 *. y2))) (max (zero_zamiast_nan(x *. y2)) (zero_zamiast_nan(x2 *. y)))) |
                                            (*wynika z liniowosci mnozenia - gdyby jakies wartosci ze srodka osiagalyby minimalna wartosc, to jedna z nich mozna przesuwac w jedna lub druga strone otrzymujac wiekszy lub mniejszy wynik, czyli w jedna ze stron uzyskamy mniejszy wynik, sprzecznosc; wiec liczby tworzace najmniejszy iloczyn sa na brzegach przedzialu*)

    (Pojedynczy(x,x2),Podwojny (y,y2)) -> if x=0. && x2=0. then wartosc_dokladna 0.
                                          else if x=0. || x2=0. then wartosc_od_do neg_infinity infinity                                   (*Przedzial zawierajacy dowolnie duze i male wartosci * przedzial zawierajacy wartosci dowolnie bliskie zeru*)
                                          else if x<0. && x2<0. then Podwojny(y2*.x2,y*.x2)
                                          else if x>0. && x2>0. then Podwojny(y*.x,y2*.x)
                                          else (*x<0. and x2>0. => zawiera okolice 0*) wartosc_od_do neg_infinity infinity|
    (Podwojny (x,x2),Pojedynczy(y,y2)) -> razy (Pojedynczy(y,y2)) (Podwojny(x,x2))|
    (Podwojny (x,x2),Podwojny(y,y2))   -> if x>=0. || x2<=0. then wartosc_od_do neg_infinity infinity                                       (*Jesli przedzial zawiera 0, a wiec rowniez jego okolice, to kazda wartosc jest mozliwa do osiagniecia w przypadku, gdy drugi przedzial osiaga wartosci dowolnie duze i male*)
                                          else if y>=0. || y2<=0. then wartosc_od_do neg_infinity infinity
                                          else (* x,y<0. and x2,y2>0.*) Podwojny(max (x*.y2) (y*.x2),min (x*.y) (x2*.y2));;
let odwrotny x=match x with                                                                                                                 (*odwrotny x= podzielic <1.,1.> x*)
    Pusty-> Pusty |

    Pojedynczy (x,x2) -> if x=0. && x2=0. then Pusty
                         else if x=neg_infinity && x2=infinity then wartosc_od_do neg_infinity infinity                                     (*Nie da sie uzyskac zera, ale da sie uzyskac cokolwiek bliskie zeru, a przedzialy domykamy*)
                         else if x<0. && x2>0. then Podwojny(1./.x,1./.x2)
                         else if x<0. && x2=0. then wartosc_od_do neg_infinity (1./.x)
                         else if x=0. && x2>0. then wartosc_od_do (1./.x2) infinity
                         else if x<0. (*x2<0*) then wartosc_od_do (1./.x2) (1./.x)
                         else (*x>0 and x2>0*) wartosc_od_do (1./.x2) (1./.x)|

    Podwojny(x,x2) -> if x<0. && x2<0. then Podwojny(1./.x2,1./.x)                                                                          (*Korzystamy z tego, ze Podwojny(a,b)=(-inf,inf)\Pojedynczy(a,b) [poza punktami brzegowymi], czyli ich odwrotnosci spelniaja podobna zaleznosc*)
                      else if x>0. && x2>0. then Podwojny(1./.x2,1./.x)
                      else if x<0. && x2=0. then wartosc_od_do (1./.x) infinity
                      else if x=0. && x2>0. then wartosc_od_do neg_infinity (1./.x2)
                      else (*x<0 and x2>0*) wartosc_od_do (1./.x) (1./.x2);;
                                                                                                                                            (*moznaby bylo napisac Podwojny(x,x2) -> wartosc_od_do (first (odwrotny Pojedynczy(x,x2))) (second (odwrotny Pojedynczy(x,x2)))*)

let podzielic a b = razy a (odwrotny b);;
(*Aktualizacja po pierwszym sprawdzeniu: jedynym problemem mnozenia przez przedzial, ktory zaczynal lub konczyl sie na zerze bylo 0*(+-infty)=nan, a w takiej sytuacji zawsze chcemy dostac 0. w takim razie jesli gdzies w tej sytuacji otrzymujemy nan, chcemy uzyskac 0. Funkcja zero_zamiast_nan zmienia nan na 0, reszte zostawia jak byla. w ten sposob funkcja razy dla pojedynczych nie ma ifow*)

type expr = Add of expr * expr | Var of string

let rec fold_expr f_add f_var = function
| Add (l, r) -> f_add (fold_expr f_add f_var l) (fold_expr f_add f_var r)
| Var z -> f_var z;;
let rec st = function Add(l, r) -> max (st l) (1 + st r) | Var _ -> 1


let max x y=if x>y then x else y;;

let optymalizuj e=
    let f_add (r1,w1) (r2,w2)= (*rx,wx- wysokosc optymalnego wyrazenia, optymalne wyrazenie*)
        if r1>=r2 then (max r1 (r2+1),Add(w1,w2)) (*Jesli oplaca sie zmienic kolejnosc to zmieniamy, jesli nie to nie (jak wysokosci sa rowne to bez roznicy, i tak musimy "zaplacic"/zwiekszyc wysokosc)*)
        else (max r2 (r1+1),Add(w2,w1))
    in
    let f_var w=
        (1,Var(w))              (*rozmiar to 1,optymalne wyrazenie to Var(w) bo jest jedyne*)
    in
    let (_,res)=fold_expr f_add f_var e
    in res;;
(*Zlozonosc pamieciowa- liniowa od wejscia, bo dla kazdego Varu tworzymy kolejny Var, natomiast Add laczy Vary i nie pogarsza zlozonosci (nie tworzy nowych Varow, tylko skleja istniejace)*)
(*Zlozonosc czasowa- liniowa od wejscia, kazde wyrazenie rozpatrujemy w czasie stalym- mamy ifa i za kazdym razem laczymy dwa elementy, czas jest staly*)

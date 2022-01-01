(*
Autor: Jan Wojtach, grupa 3;
*)

type t =
    | Empty
    | Node of t * (int * int) * t * int * int
    (*Lewe poddrzewo; wartosc wierzcholka- przedzial <l,r); Prawe poddrzewo; wysokosc; rozmiar (ilosc elementow w danym drzewie)*)


let height = function
    | Node (_, _, _, h, _) -> h
    | Empty -> 0

let size = function
    | Node (_,_,_,_,sz) -> sz
    | Empty ->0

(*Sumuje licznosci drzew/przedzialow uwazajac przy okazji na to, zeby nie wyjsc za zakres intow*)
let fix_size a b c=
    if a+b<0||a+b+c+1<0 then max_int else a+b+c+1

let make l (lk,rk) r = Node (l, (lk,rk), r, max (height l) (height r) + 1,fix_size (size r) (size l) (rk-lk))

let bal l k r =
    let hl = height l in
    let hr = height r in
    if hl > hr + 2 then
        match l with
        | Node (ll, lk, lr, _, _) ->
            if height ll >= height lr then make ll lk (make lr k r)
            else
                (match lr with
                | Node (lrl, lrk, lrr, _, _) ->
                    make (make ll lk lrl) lrk (make lrr k r)
                | Empty -> assert false)
        | Empty -> assert false
    else if hr > hl + 2 then
        match r with
        | Node (rl, rk, rr, _, _) ->
            if height rr >= height rl then make (make l k rl) rk rr
            else
              (match rl with
              | Node (rll, rlk, rlr, _, _) ->
                  make (make l k rll) rlk (make rlr rk rr)
              | Empty -> assert false)
        | Empty -> assert false
    else make l k r


let empty = Empty

let is_empty x = x = Empty


(*find_smaller przedzialu zawierajacego x. Jesli takiego nie ma, zwraca najwiekszy wierzcholek (pod wzgledem priorytetu), ktory ma wszystkie wartosci < niz x*)
let rec find_smaller x= function
    | Node (l, (lk,rk), r, _, _) ->
        if lk > x then find_smaller x l         (*Jesli wierzcholek nie jest mniejszy, to prawe poddrzewo tez*)
        else make l (lk,rk) (find_smaller x r)  (*Jesli wierzcholek jest mniejszy, to lewe tez, a prawe nalezy zbadac*)
    | Empty -> Empty


(*Analogicznie dla find_smaller, tylko zwraca najmniejszy wierzcholek z wszystkimi wartosciami > niz x*)
let rec find_bigger x= function
    | Node (l, (lk,rk), r, _, _) ->
        if rk < x then find_bigger x r
        else make (find_bigger x l) (lk,rk) r
    | Empty -> Empty

(*Laczy dwa drzewa, wykonuje O(rozmiar) balansów za każdym razem, gdy dochodzi max 1 wierzchołek (i potencjalnie jakieś zostają wyrzucone), czyli wysokosc drzewa mozemy szacowac jako O(log(n)) (Jesli drzewa sa takiej samej wysokosci to zwieksza rozmiar drzewa o 1, w przeciwnym wypadku utrzymuje wczesniejszy poziom), usuwanie duzej ilosci wierzcholkow nie powoduje, ze rozmiar > log(n), (gdzie n to ilosc wszystkich przedzialow)*)
let rec merge t1 t2 =
    match t1, t2 with
    | Empty, _ -> t2
    | _, Empty -> t1
    | Node(ll, lk, lr, lh, _),Node(rl, rk, rr, rh, _) ->
        if lh<=rh then bal (merge t1 rl) rk rr
        else bal ll lk (merge lr t2)

(*Sprawdza, czy najwiekszy przedzial sposrod mniejszych rownych od (lx,rx) przecina sie z (lx,rx) i "laczy te dwa wierzcholki" (w tym kroku odcina ten wierzcholek i przekazuje, ze istnieje i sie przecina)*)
let rec mniejsze_graniczne mniejsze (lx,rx)=match mniejsze with
    | Empty -> (Empty,Empty)
    | Node(l,(lk,rk), r, _, _)->
        if is_empty r then
            if lk>=lx then match l with
            (*Chcemy usunac nasz wierzcholek, ale blokuje nas jego lewe poddrzewo, musimy zmerge'owac*)
                | Empty -> (Empty, Empty)
                | Node(_,(_,_), _, _, _) -> (l, Empty)
            else if rk+1<lx&& rk!=max_int then (make l (lk,rk) r,Empty) (*Nie przecina sie*)
            else (*lx<=rk+1*) match l with
            (*Chcemy usunac nasz wierzcholek, ale blokuje nas jego lewe poddrzewo, musimy zmerge'owac*)
                | Empty -> (Empty, make Empty (lk,rk) Empty)
                | Node(_,(_,_), _, _, _) -> (l, make Empty (lk,rk) Empty)
        else let (nowe_prawe,wynik)= mniejsze_graniczne r (lx,rx)
        in (make l (lk,rk) nowe_prawe, wynik)

(*Analogiczne dla funkccji mniejse_graniczne*)
let rec wieksze_graniczne wieksze (lx,rx)=match wieksze with
    | Empty -> (Empty,Empty)
    | Node(l,(lk,rk), r, _, _)->
        if is_empty l then
            if rk<= rx then match r with
                | Empty -> (Empty, Empty)
                | Node(_,(_,_), _, _, _) -> (r, Empty)
            else if lk>rx+1 then (wieksze, Empty)
            else (*rx+1<=lk*) match r with
                | Empty -> (Empty, make Empty (lk,rk) Empty)
                | Node(_,(_,_), _, _, _) -> (r, make Empty (lk,rk) Empty)
        else let (nowe_lewe,wynik)= wieksze_graniczne l (lx,rx)
        in (make nowe_lewe (lk,rk) r, wynik)


(*Dzieli drzewo na wierzcholki mniejsze i wieksze pod wzgledem priorytetu od (lx,rx) + ewentualnie znajduje dwa przedzialy przecinajace sie z (lx,rx)*)
let znajdz_mniejsze_wieksze_graniczne (lx,rx) s=match s with
    | Empty -> assert false
    | Node(_,(_, _), _, _, _)->
        let mniejsze= find_smaller lx s
        and wieksze= find_bigger rx s
        in
        let (mniejsze,mniejsze_gra)=mniejsze_graniczne mniejsze (lx,rx)
        and (wieksze,wieksze_gra)=wieksze_graniczne wieksze (lx,rx)
        in
        (mniejsze,wieksze,mniejsze_gra,wieksze_gra)

(*Wstawia przedzial do drzewa zakladajac, ze bedzie najwiekszy w drzewie*)
let rec wstaw_najwiekszy s (lx,rx)=match s with
    | Empty -> make Empty (lx,rx) Empty
    | Node(l,k,r,_,_)-> bal l k (wstaw_najwiekszy r (lx,rx))

(*Wstawia przedzial do drzewa zakladajac, ze bedzie najmniejszy w drzewie*)
let rec wstaw_najmniejszy s (lx,rx)=match s with
    | Empty -> make Empty (lx,rx) Empty
    | Node(l,k,r,_,_)-> bal (wstaw_najmniejszy l (lx,rx)) k r


(*Robi to co funkcja znajdz_mniejsze_wieksze_graniczne z tym, ze od granicznych odjety jest juz przedzial (lx,rx), a przedzialy sa dodane w odpowiednie miejsca (czyli dodatkowo O(log(n)) operacji)*)
let podziel_na_mniejsze_i_wieksze_roz (lx,rx) s=match s with
    | Empty -> (Empty, Empty)
    | Node(_,(_, _), _, _, _)->
        let (mniejsze,wieksze,mniejsze_gra,wieksze_gra)=znajdz_mniejsze_wieksze_graniczne (lx,rx) s
        in
        let mniejsze=match mniejsze_gra with
            | Empty -> mniejsze
            | Node(_,(lk,rk),_,_,_) ->wstaw_najwiekszy mniejsze (lk,lx-1)
        and wieksze=match wieksze_gra with
            | Empty -> wieksze
            | Node(_,(lk,rk),_,_,_) ->wstaw_najmniejszy wieksze (rx+1,rk)
        in
        (mniejsze, wieksze)

(*Tworzy drzewo z wartosciami mniejszymi niz lx i drugie drzewo z wartosciami wiekszymi niz rx (+ewentualnie znajduje przedzialy przecinajace sie z (lx,rx) i je skleja w jeden) w O(log(n))*)
let add (lx,rx) s=match s with
    | Empty -> make Empty (lx,rx) Empty
    | Node(_,_, _, _, _)->
        let (mniejsze,wieksze,mniejsze_gra,wieksze_gra)=znajdz_mniejsze_wieksze_graniczne (lx,rx) s
        in
        let lx=match mniejsze_gra with
            | Empty -> lx
            | Node(_,(lk, _),_,_,_) -> lk
        and rx=match wieksze_gra with
            | Empty -> rx
            | Node(_,(_, rk), _, _, _) -> rk
        in
        if not (is_empty mniejsze) then
            let mniejsze=wstaw_najwiekszy mniejsze (lx,rx)
            in
            merge mniejsze wieksze
        else
            let wieksze = wstaw_najmniejszy wieksze (lx,rx)
            in
            wieksze



(*Dzieli drzewo na dwa drzewa (jedno z wartosciami mniejszymi od przedzialu (lx,rx) i drugie wieksze (drzewa sa tez rozlaczne z przedzialem)) i je laczy w O(log(n))*)
let remove (lx,rx) s=
    let (mniejsze, wieksze)=podziel_na_mniejsze_i_wieksze_roz (lx,rx) s
    in
    merge mniejsze wieksze

(*Przechodzi wglab drzewa szukajac wartosci, zlozonosc O(wysokosc) czyli O(log(n))*)
let rec mem x s=match s with
    | Empty -> false
    | Node(l, (lk,rk), r, _, _) ->
        if x<lk then mem x l
        else if x>rk then mem x r
        else true

let iter f set =
    let rec loop = function
        | Empty -> ()
        | Node (l, (lk,rk), r, _, _) -> loop l;f (lk,rk); loop r in
    loop set

let fold f set acc =
    let rec loop acc = function
        | Empty -> acc
        | Node (l, (lk,rk), r, _, _) ->
            loop (f (lk,rk) (loop acc l)) r in
    loop acc set

let elements set =
    let rec loop acc = function
        | Empty -> acc
        | Node(l, (lk,rk), r, _, _) -> loop ((lk,rk) :: loop acc r) l in
    loop [] set


let split x s=
    let (mniejsze, wieksze)=podziel_na_mniejsze_i_wieksze_roz (x,x) s
    in
    (mniejsze, mem x s , wieksze)

(*Dzieli na mniejsze i wieksze w O(log n) i do licznosci drzewa z mniejszymi wartosciami ewentualnie dodaje 1 jesli w drzewie jest wartosc x*)
let rec below x s=
    let (mniejsze, _)=podziel_na_mniejsze_i_wieksze_roz (x,x) s
    in
    if mem x s then fix_size (size mniejsze) 0 0 (*rownowazne z (size mniejsze+1), ale obsluguje wyjatek pod tytulem (size mniejsze=max_int)*)
    else size mniejsze

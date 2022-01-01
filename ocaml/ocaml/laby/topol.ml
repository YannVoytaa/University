(*Autor: Jan Wojtach, grupa 3*)

exception Cykliczne;;

(*Funkcja dodajaca do mapy krawedzi sasiadow danego wierzcholka*)
let rec dodaj mapa w sas=
    match sas with
    | []->mapa
    | h::t->
        if PMap.mem w mapa then dodaj (PMap.add w (h::(PMap.find w mapa)) mapa) w t
        else dodaj (PMap.add w [h] mapa) w t

(*Tworzy mape krawedzi na podstawie listy wejsciowej*)
let rec stworz_krawedzie ls acc=
    match ls with
    | []->acc
    | (h, ls_sas)::t->stworz_krawedzie t (dodaj acc h ls_sas)

(*Funkcja majaca za zadanie stworzyc poprawny porzadek i zaktualizowac
  rozwiazanie o kolejne wierzcholki, odwiedzi kazdy wierzcholek maksymalnie raz*)
let rec dfs x kraw_map odw result=
    (*Zaznaczamy, ze wierzcholek x jest rozpatrywany w aktualnym przejsciu po grafie-
      w takiej sytuacji zazanaczamy odw[x]=false; gdy rozpatrzymy wszystkich sasiadow wierzcholka to zaznaczamy odw[x]=true
      Pomoze nam to w znalezieniu ewentualnego cyklu, bo jesli sasiad rozpatrywanego wierzcholka
      jeszcze jest w trakcie przetwarzania (zachodzi odw[sasiad]=false) to w pewien sposob doszlismy
      z sasiada do wierzcholka + mamy krawedz z wierzcholka do sasiada
      A wiec wtedy bedziemy mieli cykl. W taki sposob znajdziemy kazdy mozliwy cykl, bo zanim skonczymy przetwarzac wierzcholek
      odwiedzimy wszystkie wierzcholki, do ktorych mozemy dojsc (czyli w szczegolnosci dojdziemy drugi raz do tego wierzcholka jesli nalezy do cyklu)*)
    let odw=PMap.add x false odw
    and
    kraw=
    if PMap.mem x kraw_map then PMap.find x kraw_map
    else []
    in
    let fold_fun (odw,result) v=
        if PMap.mem v odw then
            if PMap.find v odw=false then raise Cykliczne (*Znalezlismy cykl*)
            else (odw,result)
        else dfs v kraw_map odw result
    in
    let (odw,result)=List.fold_left fold_fun (odw,result) kraw
    in
    (PMap.add x true odw,x::result)
    (*Konczymy przetwarzac x, wiec zaznaczamy odw[x]=true i dodajemy x na poczatek stosu, bo x nie musi byc przed zadnym nieprzerobionym
      wierzcholkiem (przerobilismy wczesniej wszystkich, od ktorych byl zalezny)*)

(*Przetwarza po kolei wierzcholki, ktore do tej pory nie byly przetworzone (sumarycznie przetworzy kazdy wierzcholek raz)*)
let rec find_result kraw odw result wierzcholki=
    match wierzcholki with
    | []->result
    | (h, _)::t->
        if PMap.mem h odw then find_result kraw odw result t
        else
            let (odw,result)=dfs h kraw odw result
            in
            find_result kraw odw result t

let topol lista=
    let krawedzie=stworz_krawedzie lista (PMap.empty)
    in
    find_result krawedzie (PMap.empty) [] lista

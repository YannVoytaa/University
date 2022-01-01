(*Autor: Jan Wojtach, grupa 3*)

type 'a queue = Null|                                               (*Puste drzewo*)
                Node of  'a queue * ('a * int) * 'a queue;;         (*W wierzcholku przechowywane sa dwie wartosci (wartosc/priorytet, wysokosc), gdzie wysokosc:=dlugosc sciezki do najblizszego Nulla (u nas rowne wysokosci prawego poddrzewa)*)


let read_h q=match q with                                           (*Zwraca wysokosc drzewa*)
    Null->0|                                                        (*dlugosc sciezki od Nulla do najblizszego Nulla to 0*)
    Node(l,(x,h),r)->h;;

let empty=Null;;

let rec join q q2=
    match q,q2 with
    Null, Null-> Null|
    Null, Node(l2,(x2,h2),r2) ->q2|
    Node(l,(x,h),r), Null ->q |
    Node(l,(x,h),r), Node(l2,(x2,h2),r2) ->
        if x<x2 then tworz l x r q2           (*lewe poddrzewo:= lewe poddrzewo z drzewa z wierzcholkiem o wiekszym priorytecie, prawe poddrzewo:= merge prawego poddrzewa i drugiego drzewa (potem ewentualnie obrocic)*)
        else tworz l2 x2 r2 q

(*funkcja, ktora dostaje korzen, poddrzewo i dwie skladowe drugiego poddrzewa nowego drzewa i tworzy drzewo spelniajace warunek lewicowosci*)
and tworz lewe war_korzen prawe1 prawe2=
    let pelne_prawe=join prawe1 prawe2
    in
    if read_h pelne_prawe >= read_h lewe then Node(pelne_prawe,(war_korzen, read_h lewe +1),lewe)          (*utrzymanie warunku lewicowosci*)
    else Node(lewe,(war_korzen, read_h pelne_prawe +1),pelne_prawe);;

let add x q=join (Node(Null,(x,1),Null)) q;; (*Dodawanie elementu jest jak laczenie drzew, z czego jedno ma tylko jeden wierzcholek*)

exception Empty;; (*Wyjatek zwracany, gdy chcemy wziac element z drzewa, a drzewo jest puste*)

let delete_min q=match q with
    Null->raise Empty|
    Node(l,(x,h),r)->(x,join l r);;

let is_empty q= q=Null;;

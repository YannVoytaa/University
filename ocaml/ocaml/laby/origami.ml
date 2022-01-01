(*
Autor: Jan Wojtach, grupa 3;
*)
type point=float * float;;

type kartka= point -> int;;
let epsilon=Float.epsilon*.1000.;;


let square x=min (x*.x) max_float;;

let prostokat ((xp,yp):point) ((xk,yk):point) : kartka=
    fun ((x,y):point) ->
        if xp<=x && x<=xk && yp<=y && y<=yk then 1
        else 0;;


let kolko ((xp,yp):point) r :kartka=
    fun ((x,y):point) ->
        if square (x-.xp) +. square (y-.yp)<=square ((1.+.epsilon)*.r) then 1
        else 0;;

(*Wiemy, ze iloczyn wektorowy=iloczyn dlugosci pomnozony przez sinus kata miedzy danymi wektorami*)
let sinus (xp,yp) (xk,yk) (x,y)=
    let iloczyn_dlugosci=square (xk-.xp) +. square (yk-.yp) (*Mozna szacowac co do rzedu z prawdziwym iloczynem dlugosci, a na pewno nie dostaniemy zerowego iloczynu*)
    and
    iloczyn_wektorowy (vx,vy) (wx,wy)=vx*.wy-.vy*.wx
    and
    wektor_odcinek=(xk-.xp,yk-.yp)
    and
    wektor_do_punktu=(x-.xp,y-.yp)
    in
    iloczyn_wektorowy wektor_odcinek wektor_do_punktu /. iloczyn_dlugosci;;

(*sprawdzamy, czy punkt jest po prawej stronie od prostej/odcinka, korzystamy z wlasnosci iloczynu wektorowego (a dokladniej liczymy sinus kata: jesli jest on "wystarczajaco ujemny" to jest po prawej)*)
let po_prawej pocz kon pkt=if sinus pocz kon pkt <=(-.epsilon) then true else false;;

(*sprawdza czy punkt lezy na prostej (zakladamy, ze floaty sa niedokladne, wiec mamy margines bledu zalezny od wielkosci naszych wektorow)*)
let na_prostej pocz kon pkt=
    if sinus pocz kon pkt <epsilon (*&& > -epsilon, ale funkcja na_prostej jest wywolywana, gdy funkcja po_prawej zwraca falsz, czyli to na pewno zachodzi*)
    then true else false;;

(*Operacje na wektorach*)
let dodaj (wx,wy) (vx,vy)=(wx+.vx,wy+.vy);;

let odejmij (wx,wy) (vx,vy)=(wx-.vx,wy-.vy);;

let pomnoz (wx,wy) skalar=(wx*.skalar,wy*.skalar);;

let podziel (wx,wy) skalar=(wx/.skalar, wy/.skalar);;

let iloczyn_skalarny (wx,wy) (vx,vy)=wx*.vx+.wy*.vy;;

let przerzuc pocz kon pkt=
    let v1=odejmij pkt pocz and v2=odejmij kon pocz
    in
    let x=dodaj pocz (podziel (pomnoz v2 (iloczyn_skalarny v1 v2)) (iloczyn_skalarny v2 v2)) (*Rzut punktu pkt na prosta pocz->kon*)
    in
    odejmij (pomnoz x 2.) pkt;; (*do punktu musimy dodac dwa razy wektor p->x, zeby znalazl sie po drugiej stronie prostej; w takim razie wynikiem jest p+2(x-p)=2x-p*)

let zloz ((xp,yp):point) ((xk,yk):point) (k:kartka) :kartka=
    fun ((x,y):point) ->
        if po_prawej (xp,yp) (xk,yk) (x,y) then 0
        else if na_prostej (xp,yp) (xk,yk) (x,y) then k (x,y)
        else (*po_lewej*) k (x,y) + k (przerzuc (xp,yp) (xk,yk) (x,y));;


let skladaj (ls:(point*point) list) (k:kartka) :kartka=
  List.fold_left (fun acc (p1,p2) -> zloz p1 p2 acc) k ls;;

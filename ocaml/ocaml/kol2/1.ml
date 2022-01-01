let exists pred ls=List.fold_left (fun acc h -> if acc=true then true else pred h) false ls;;

exception Wyjatek;;
let rec exist2 pred ls=
  try
    match ls with
      | []-> false
      | h::t ->
        if pred h then raise Wyjatek
        else exist2 pred t
  with Wyjatek -> true


let non pred: ('a->bool)=fun x->not (pred x)

let forall pred ls=not(exist2 (non pred) ls);;

let heads lst=List.flatten (List.map (fun ls->match ls with | []-> [] | h::_ -> [h]) lst)

let transpose lst=
  let pom (acc,accpom) ls=
    let pom2 (acc2,accpom2) x=
      match acc2 with
        | []->([],[x]::accpom2)
        | h::t->(t,(x::h)::accpom2)
    in
    let (new_acc_pom,new_acc)=List.fold_left pom2 (acc,accpom) ls
    in (List.rev new_acc, new_acc_pom)
  in
  let (res,_)=List.fold_left pom ([],[]) lst
  in List.map (fun ls->List.rev ls) res;;


type 'a tree=
  | Null
  | Node of 'a * 'a tree list;;

let rec fold_tree f (Node(x,ls))=
  f x (List.map (fold_tree f) ls)

let pre t=
  let pom x ls acc=
    List.fold_left (fun acc f-> f acc) (x::acc) ls
  in
  fold_tree pom t [];;

let post t=
  let pom x ls acc=
    x::(List.fold_left (fun acc f-> f acc) acc ls)
  in
  fold_tree pom t [];;

let levels t=
  let pom x ls acc=
    match acc with
      | []->[x]::(List.fold_right (fun f acc-> f acc) ls [])
      | h::t->(x::h)::(List.fold_right (fun f acc-> f acc) ls t)
  in
  fold_tree pom t [];;

(*type 'a b_tree=
  | Null
  | Node of 'a b_tree * 'a * 'a b_tree;;

let rec fold_tree f a t=
  match t with
    | Null -> a
    | Node (l,x,r) -> f x (fold_tree f a l) (fold_tree f a r);;

let min x y=if x<y then x else y;;

let czapeczki t1 t2=
  let rec pom t1 t2 poziom=
    match t1,t2 with
      | Null,Null-> (max_int,poziom-1)
      | Null,_ -> (poziom,poziom)
      | _,Null -> (poziom,poziom)
      | Node(l,x,r),Node(l2,x2,r2)->
          if x!=x2 then (poziom,poziom)
          else
          let (l_blad,l_gleb)=pom l l2 (poziom+1)
          and (r_blad,r_gleb)=pom r r2 (poziom+1)
          in
          (min l_blad r_blad,max l_gleb r_gleb)
  in
  let(blad,glebokosc)=pom t1 t2 1
  in
  if blad=max_int then glebokosc else blad-1;;

let abs x=if x<0 then -x else x;;
let max x y=if x>y then x else y;;

let avl_shape t=
  let pom _ (lh, czy_lewy) (rh, czy_prawy)=
    if czy_lewy && czy_prawy && abs (lh-rh)<=1 then (max lh rh+1,true)
    else (max lh rh+1, false)
  in
  let (_,res)=fold_tree pom (0,true) t
  in
  res;;
*)

let liscie t=
  let pom x ls acc=
    let do_dodania=if ls=[] then 1 else 0
    in
    match acc with
      | [] -> do_dodania::(List.fold_left (fun acc2 f-> f acc2) [] ls)
      | h::t ->(h+do_dodania)::(List.fold_left (fun acc2 f-> f acc2) t ls)
  in
  fold_tree pom t [];;


(*let liscie t=
  let rec pom_tree t acc=
    match t with
      | Null->acc
      | Node(_,ls)->
        match acc with
          | []-> 1::(pom_lst ls [])
          | h::t->(h+1)::(pom lst ls t)
  and pom_lst ls=
    match acc with
      | []-> (List.fold_left (fun acc _ ->acc+1) 0 ls)::
*)

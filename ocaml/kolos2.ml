type 'a tree =  Node of 'a * 'a tree list;;
 
let rec fold_tree f (Node (x, l)) = 
  f x (List.map (fold_tree f) l);;
  
let mytree = Node(4, [Node(2, [Node(1, [Node(9, [])]); Node(3, [])]); Node(6, [Node(5, [Node(10, []); Node(11, [])]); Node(7, []); Node(8, [Node(12, [])])])]);;
let mytree2 = Node(4, [Node(2, [Node(1, [Node(9, [])]); Node(3, [Node(13, []);Node(14, []);Node(15, []);Node(16, []);])]); Node(6, [Node(5, [Node(10, []); Node(11, [])]); Node(7, []); Node(8, [Node(12, [])]);Node(17, []);])]);;

(*
			4
	2				   6
1		3		5	   7	8
9			  10  11           12
*)

(*
6 ma 1 liścia, 3 ma 4 dodatkowe liście.

*)

(* jak działa fold: *)
let pierwszy_lisc tre = 
  fold_tree (fun x lis -> if lis = [] then x else List.hd lis) tre;;

(* 6.13 *)
let polowa tre =
  let calc_children lis = 
	List.fold_left (fun acc count -> acc + count) 0 lis
  in
  let leaf_ct = fold_tree (fun x lis -> if lis = [] then 1 else calc_children lis) tre in
  let solve x lis =
    if lis = [] then if leaf_ct < 3 then ((x, 1), 1) else ((x, 1000000000), 1)
    else (* List.fold_left (fun ((a, b), c) elem -> ((a, b), c + elem)) ((x, 1), 1) lis *)
      let here_count = List.fold_left (fun acc (_, sub_count) -> acc + sub_count) 0 lis in
      let here_count = if here_count = 0 then 1 else here_count in
      let best = List.fold_left (fun (acc_nr, acc_count) ((nr, count), _) -> if count * 2 >= leaf_ct then (if count < acc_count then (nr, count) else (acc_nr, acc_count)) else (acc_nr, acc_count))
                 (if here_count * 2 >= leaf_ct then (x, here_count) else (-1, 1000000000)) lis
      in
      (best, here_count) 	  
  in
  let Node(x, d) = tre in
  fst (fst (fold_tree (fun x lis -> solve x lis) tre))
  (* fold_tree solve tre *)
;;  
polowa mytree;;  
polowa mytree2;;
(*      
zwracam liste (wierzchołek, ilu_jest_przodkiem)
*)

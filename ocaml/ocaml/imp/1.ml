(*let cykl arr=
    let odw=Array.make (Array.length arr) false
    and
    maks=ref 0
    in
    let rec dfs x acc=
        begin
        odw.(x)<-true;
        if odw.(arr.(x))=false then dfs (arr.(x)) (acc+1)
        else acc
        end
    in
    for i=0 to Array.length arr-1 do
      if odw.(i)=false then maks:=max (!maks) (dfs i 1);
    done;
    !maks;;
*)
(*type 'a tree =
        Node of 'a * 'a tree * 'a tree * 'a tree ref |
        Null;;
*)
(*let fastryguj t=
    let prev=ref Null
    in
    let rec dfs tr=
        match tr with
        | Null->()
        | Node(w,l,r,ptr)->
            begin
            print_int w;
            dfs l;
            begin
            match !prev with
            | Null->()
            | Node(w,_,_,ptr_prev)->
                begin
                print_int w;
                ptr_prev:=tr;
                end;
            end;
            print_int 7;
            prev:=tr;
            dfs r;
            end
    in
    dfs t;
    prev;;*)
(*let fastryguj d =
  let rec pom d nast = match d with
   | Null -> nast
   | Node(x, l, p, r) ->
       r := pom p nast;
       pom l d
  in
    ignore (pom d Null); ()
let drzewo=Node(1,Node(2,Node(4,Null,Null,ref Null),Node(5,Null,Null,ref Null),ref Null),Node(3,Null,Null,ref Null),ref Null)


let drzewo2=Node(1,Node(2,Null,Null,ref Null),Null,ref Null)
*)
(*type elem = { v: int; mutable prev: elem list };;
type lista = elem list;;

let rec ustaw ls=
    let rec pom ls ls2=
    match ls with
    | None->ls2
    | Some({v=x;prev=prv})->
        begin
        let ls2=pom prv ls2
        in
        match ls2 with
        | None->()
        | Some({v=ls2_x;prev=ls2_prv})->
            let n=ls2_prv
            in
            begin
            ls2.prev<-ls;
            n
            end
        end
    in
    pom ls ls;;
*)

let bfs_make lista_relacji=
    let map=Hashtbl.create 100000
    and
    odw=Hashtbl.create 100000
    in
    let dodaj_sasiada w sas=
        if Hashtbl.mem map w then Hashtbl.replace map w (sas::(Hashtbl.find map w))
        else Hashtbl.add map w [sas]
    and
    bfs (x:int)=
        let stack=Queue.create ()
        in
        Queue.push x stack;
        while Queue.is_empty stack=false do
        let x=Queue.take stack
        in
        begin
        let sasiedzi=Hashtbl.find map x
        in
        begin
        print_int x;
        Hashtbl.add odw x true;
        List.fold_left (fun () sasiad->if Hashtbl.mem odw sasiad then ()
                                    else
                                        begin
                                            Hashtbl.add odw sasiad true;
                                            Queue.push sasiad stack;
                                        end ) () sasiedzi;
        end;
        end;
        done;
    in
    begin
    List.fold_left (fun () (x,y)->(dodaj_sasiada x y;dodaj_sasiada y x)) () lista_relacji;
    bfs 1;
    end;;



let dijkstra lista_relacji=
    let map=Hashtbl.create 100000
    and
    odw=Hashtbl.create 100000
    and
    dist=Hashtbl.create 100000
    in
    let dodaj_sasiada w sas war=
        if Hashtbl.mem map w then Hashtbl.replace map w ((sas,war)::(Hashtbl.find map w))
        else Hashtbl.add map w [(sas,war)]
    and
    bfs (x:int)=
        let stack=PriorityQueue.create ()
        in
        PriorityQueue.push (0,x) stack;
        Hashtbl.add dist x 0;
        while PriorityQueue.is_empty stack=false do
        let (_,x)=PriorityQueue.take stack
        in
        begin
        if Hashtbl.mem odw x then ()
        else
          begin
            let sasiedzi=Hashtbl.find map x
            in
            begin
            print_int x;
            Hashtbl.add odw x true;
            List.fold_left (fun () (sasiad,dlugosc)->if Hashtbl.mem odw sasiad then ()
                                        else if Hashtbl.find dist x+dlugosc<Hashtbl.find dist sasiad then
                                            begin
                                                Hashtbl.replace dist sasiad (Hashtbl.find dist x+dlugosc);
                                                Queue.push (Hashtbl.find dist sasiad,sasiad) stack;
                                            end ) () sasiedzi;
            end;
            end;
          end;
        done;
    in
    begin
    List.fold_left (fun () (x,y,dl)->(dodaj_sasiada x y dl)) () lista_relacji;
    bfs 1;
    end;;

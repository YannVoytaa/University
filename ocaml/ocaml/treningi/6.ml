type 'a t= Null|
        Node of 'a t * 'a * 'a t;;


        let preOrder v =
            let rec pre_aux list v =
                match v with
                    | Null -> list
                    | Node(l, x, r) -> pre_aux (pre_aux (x::list) l) r
            in
            pre_aux [] v
        ;;

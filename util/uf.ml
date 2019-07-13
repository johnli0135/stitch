open Misc

type t = Meta.t MetaM.t

let empty = MetaM.empty

let rec find x m =
  match MetaM.find_opt x m with
  | None -> x
  | Some x -> find x m

let rec find' x m =
  match MetaM.find_opt x m with
  | None -> (m, x)
  | Some x ->
      let m, y = find' x m in
      (MetaM.add x y m, y)

let union x y m =
  let m, x = find' x m in
  let m, y = find' y m in
  if Meta.equal x y then m else MetaM.add x y m

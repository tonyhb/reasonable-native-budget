/*
  Group represents a group of categories as one expense unit
 */
type t = {
  id: string,
  data: array(Category.t),
  name: string
};
let group = (~name, ~data) => {id: Uuid.gen(), name, data};
let updateCategoryInGroup = (group, cat: Category.t) => {
  ...group,
  data: group.data |> Array.map((item: Category.t) => item.id == cat.id ? cat : item)
};
let removeCategoryFromGroup = (group, cat: Category.t) => {
  ...group,
  data:
    group.data
    |> Array.to_list
    |> List.filter((item: Category.t) => item.id != cat.id)
    |> Array.of_list
};
let groupTotal = (group: t) =>
  group.data |> Array.fold_left((total, cat) => total +. cat.Category.amount, 0.);
let total = (groups: array(t)) =>
  groups |> Array.fold_left((total, group) => total +. groupTotal(group), 0.);
let categories = (groups: array(t)) =>
  groups |> Array.fold_left((cats, group) => cats |> Js.Array.concat(group.data), [||]);
module JSON = {
  let marshal = (g) =>
    Json.Encode.(
      object_([
        ("id", string(g.id)),
        ("name", string(g.name)),
        ("data", arrayOf(Category.JSON.marshal, g.data))
      ])
    );
  let unmarshal = (json) =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      data: json |> field("data", array(Category.JSON.unmarshal))
    };
};

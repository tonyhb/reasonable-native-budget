/* TODO: latlng coordinates */
type t = {
  id: string,
  name: string,
  defaultCategory: option(Category.t) /* A default category for this recipient */
};

let recipient = (name) => {id: Uuid.gen(), name, defaultCategory: None};

/**
  Attempts to find an existing recipient by name and returns it, or creates
  a new recipient and returns it
 */
let newRecipientByName = (l: list(t), name: string): t => {
  switch (l |> List.find((item) => item.name == name)) {
    | item => item
    | exception Not_found => recipient(name)
  };
};

let append = (l: list(t), t: t): (list(t), t) =>
  l |> List.exists((item) => item.name == t.name) ?
    (l, t) :
    (
      if (t.id == "") {
        let copy = {...t, id: Uuid.gen()};
        ([copy, ...l], copy);
      } else {
        ([t, ...l], t);
      }
    );

module JSON = {
  let marshal = (t) => Json.Encode.(object_([("id", string(t.id)), ("name", string(t.name))]));
  let unmarshal = (json) =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      defaultCategory: json |> optional(field("defaultCategory", Category.JSON.unmarshal))
    };
};

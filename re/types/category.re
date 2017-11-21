type t = {
  id: string,
  name: string,
  amount: float,
  hint: option(string)
};
let category = (~name, ~hint=?, ~amount=?, ()) => {
  id: Uuid.gen(),
  name,
  amount:
    switch amount {
    | Some(a) => a
    | None => 0.
    },
  hint
};
module JSON = {
  let marshal = (cat: t) =>
    Json.Encode.(
      object_([
        ("id", string(cat.id)),
        ("name", string(cat.name)),
        ("amount", Json.Encode.float(cat.amount)),
        ("hint", nullable(string, cat.hint))
      ])
    );
  let unmarshal = (json) =>
    Json.Decode.{
      id: json |> field("id", string),
      name: json |> field("name", string),
      amount: json |> field("amount", Json.Decode.float),
      hint: json |> field("hint", optional(string))
    };
};

let name = "Checking account";

type t = {bank: option(string)};

let default = {bank: None};

module JSON = {
  let marshal = (item) =>
    Json.Encode.(object_([("bank", string(Js.Option.getWithDefault("", item.bank)))]));
  let unmarshal = (json) => Json.Decode.{bank: json |> optional(field("bank", string))};
};

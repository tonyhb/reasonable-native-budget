type rewardsType =
  | Cashback(float)
  | Points
  | Miles;

let name = "Credit Card";

type t = {
  type_: option(string),
  apr: float,
  openedOn: int,
  rewardsType
};

let default = {type_: None, apr: 0., openedOn: 0, rewardsType: Points};

module JSON = {
  let marshal = (item) =>
    Json.Encode.(
      object_([
        ("type", string(Js.Option.getWithDefault("", item.type_))),
        ("apr", Json.Encode.float(item.apr)),
        ("openedOn", Json.Encode.int(item.openedOn)),
        (
          "rewardsType",
          switch item.rewardsType {
          | Cashback(f) => object_([("type", string("cashback")), ("value", Json.Encode.float(f))])
          | Points => object_([("type", string("points"))])
          | Miles => object_([("type", string("miles"))])
          }
        )
      ])
    );
  let unmarshalRewards = (json) => {
    open! Json.Decode;
    switch (json |> field("type", string)) {
    | "cashback" => Cashback(json |> field("value", float))
    | "points" => Points
    | "miles" => Miles
    | _ => failwith("Expected valid rewards type")
    }
  };
  let unmarshal = (json) =>
    Json.Decode.{
      type_: json |> optional(field("type", string)),
      apr: json |> field("apr", Json.Decode.float),
      openedOn: json |> field("openedOn", Json.Decode.int),
      rewardsType: json |> field("rewardsType", unmarshalRewards)
    };
};

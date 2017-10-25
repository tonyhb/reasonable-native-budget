let name = "Savings account";

type t = {
  bank: option string,
  interest: float
};

let default = {bank: None, interest: 0.01};

module JSON = {
  let marshal item =>
    Json.Encode.(
      object_ [
        ("bank", string (Js.Option.getWithDefault "" item.bank)),
        ("interest", Json.Encode.float item.interest)
      ]
    );
  let unmarshal json =>
    Json.Decode.{
      bank: json |> optional (field "bank" string),
      interest: json |> field "interest" Json.Decode.float
    };
};

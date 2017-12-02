type t = {
  payee: option(Recipient.t),
  amountIn: float /*** TODO: Should this just be amount within Entry.t? */
};

module JSON = {
  let marshal = (t: t) : Js.Json.t =>
    Json.Encode.(
      object_([
        (
          "payee",
          switch t.payee {
          | Some(c) => string(c.id)
          | None => Json.Encode.null
          }
        ),
        ("amountIn", Json.Encode.float(t.amountIn))
      ])
    );
  let unmarshal = (payees: list(Recipient.t), json: Js.Json.t) : t => {
    open Json.Decode;
    let payeeId = json |> field("payee", optional(string));
    {
      payee:
        switch payeeId {
        | Some(id) => Some(payees |> List.find((r: Recipient.t) => r.id == id)) /* TODO: Not found? */
        | None => None
        },
      amountIn: json |> field("amountIn", Json.Decode.float)
    }
  };
};

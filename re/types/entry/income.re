type t = {
  payee: option(Recipient.t),
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
        )
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
        }
    }
  };
};

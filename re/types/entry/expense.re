type t = {
  recipient: option(Recipient.t), /* Possible to have no recipients, but not very cool */
  expensable: bool,
  tags: list(string)
};

module JSON = {
  let marshal = (t: t) : Js.Json.t =>
    Json.Encode.(
      object_([
        (
          "recipient",
          switch t.recipient {
          | Some(c) => string(c.id)
          | None => Json.Encode.null
          }
        ),
        ("expensable", bool(t.expensable)),
        ("tags", list(string, t.tags))
      ])
    );
  let unmarshal = (recipients: list(Recipient.t), json: Js.Json.t) : t => {
    open Json.Decode;
    let recipientId = json |> field("recipient", optional(string));
    {
      recipient:
        switch recipientId {
        | Some(id) => Some(recipients |> List.find((r: Recipient.t) => r.id == id)) /* TODO: Not found? */
        | None => None
        },
      expensable: json |> field("expensable", bool),
      tags: json |> field("tags", list(string))
    }
  };
};

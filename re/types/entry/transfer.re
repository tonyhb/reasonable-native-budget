type t = {
  destination: Account.t,
  toCurrency: Currency.currencyType
};

module JSON = {
  let marshal = (t: t) : Js.Json.t =>
    Json.Encode.(
      object_([
        ("destination", string(t.destination.id)),
        ("toCurrency", Currency.JSON.marshal(t.toCurrency))
      ])
    );
  let unmarshal = (accounts: list(Account.t), json: Js.Json.t) : t => {
    open Json.Decode;
    let destId = json |> field("desitnation", string);
    {
      destination: accounts |> List.find((acc: Account.t) => acc.id == destId), /* TODO: Not found? */
      toCurrency: json |> field("toCurrency", Currency.JSON.unmarshal)
    }
  };
};

type t = {
  type_: string,
  code: string
};

type currencyType =
  | FiatCurrency(t)
  | CryptoCurrency(t);

let defaultCurrencyType = FiatCurrency({type_: "fiat", code: "USD"});

module JSON = {
  let marshal = (item) =>
    Json.Encode.(
      object_(
        switch item {
        | FiatCurrency(i) => [("type", string(i.type_)), ("code", string(i.code))]
        | CryptoCurrency(i) => [("type", string(i.type_)), ("code", string(i.code))]
        }
      )
    );
  let unmarshal = (json) => {
    let item = Json.Decode.{type_: field("type", string, json), code: field("code", string, json)};
    switch item.type_ {
    | "fiat" => FiatCurrency(item)
    | "crypto" => CryptoCurrency(item)
    | _ => defaultCurrencyType
    }
  };
};

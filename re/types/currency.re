type currency = {
  type_: string,
  code: string
};

type currencyType =
  | FiatCurrency currency
  | CryptoCurrency currency;

let defaultCurrencyType = FiatCurrency {type_: "fiat", code: "USD"};

module MarshalJSON = {
  let marshal item =>
    Json.Encode.(
      object_ (
        switch item {
        | FiatCurrency i => [("type", string i.type_), ("code", string i.code)]
        | CryptoCurrency i => [("type", string i.type_), ("code", string i.code)]
        }
      )
    );
  let unmarshal json => {
    let item = Json.Decode.{type_: field "type" string json, code: field "code" string json};
    switch item.type_ {
    | "fiat" => Some (FiatCurrency item)
    | "crypto" => Some (CryptoCurrency item)
    | _ => None
    }
  };
};

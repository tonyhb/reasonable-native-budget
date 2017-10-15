type settings = {defaultCurrency: Currency.currencyType};

module JSON = {
  let marshal settings =>
    Json.Encode.(object_ [("defaultCurrency", Currency.JSON.marshal settings.defaultCurrency)])
    |> Js.Json.stringify;
  let unmarshal (json: Js.Json.t) :settings =>
    Json.Decode.{defaultCurrency: field "defaultCurrency" Currency.JSON.unmarshal json};
};

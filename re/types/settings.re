type t = {defaultCurrency: Currency.currencyType};

module JSON = {
  let marshal = (settings) =>
    Json.Encode.(object_([("defaultCurrency", Currency.JSON.marshal(settings.defaultCurrency))]));
  let unmarshal = (json: Js.Json.t) : t =>
    Json.Decode.{defaultCurrency: field("defaultCurrency", Currency.JSON.unmarshal, json)};
};

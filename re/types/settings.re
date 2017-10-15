type settings = {defaultCurrency: Currency.currencyType};

module MarshalJSON = {
	let marshal settings =>
		Json.Encode.(
			object_ [("defaultCurrency", Currency.MarshalJSON.marshal settings.defaultCurrency)]
		);
	let unmarshal json :settings =>
		Json.Decode.{
			defaultCurrency:
				switch (Currency.MarshalJSON.unmarshal (json |> field "defaultCurrency" string |> Js.Json.parseExn)) {
				| Some c => c
				| None => Currency.defaultCurrencyType
				}
		};
};

/**
 * {
 *   "defaultCurrency": {
 *     "type_": "fiat",
 *     "code": "USD",
 *   }
 * }
 */
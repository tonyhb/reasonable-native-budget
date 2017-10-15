type settings = {
	defaultCurrency: Currency.currency,
	lastBackup: int
};

let parseSettingsJson json : settings =>
	Json.Decode.{
		defaultCurrency: Currency.FiatCurrency (json |> field "defaultCurrency" string),
		lastBackup: json |> field "lastBackup" int
	}
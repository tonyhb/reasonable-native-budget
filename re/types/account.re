type accountType =
  | Cash
  | Checking Checking.t
  | Savings Savings.t
  | CreditCard CreditCard.t
  | IRA Ira.t
  | Retirement Retirement.t
  | Crypto
  | Other string;


/** TODO: This should depend on currency */
type t = {
  id: string,
  balance: float,
  name: string,
  currency: Currency.currencyType,
  accountType
};

let string_of_accountType =
  fun
  | Cash => Cash.name
  | Checking _s => Checking.name
  | Savings _s => Savings.name
  | CreditCard _s => CreditCard.name
  | IRA _s => Ira.name
  | Retirement _s => Retirement.name
  | Crypto => Crypto.name
  | Other _s => "Other";

module JSON = {
  /* BS Json */
  let marshal item =>
    Json.Encode.(
      object_ [
        ("id", string item.id),
        ("name", string item.name),
        ("balance", Json.Encode.float item.balance),
        ("currency", Currency.JSON.marshal item.currency),
        ("type", string (string_of_accountType item.accountType)),
        (
          "data",
          switch item.accountType {
          | Cash => Js.Json.null
          | Checking t => Checking.JSON.marshal t
          | Savings t => Savings.JSON.marshal t
          | CreditCard t => CreditCard.JSON.marshal t
          | IRA t => Ira.JSON.marshal t
          | Retirement t => Retirement.JSON.marshal t
          | Crypto => Js.Json.null
          | Other s => string s
          }
        )
      ]
    );
  let unmarshal json =>
    Json.Decode.{
      id: json |> field "id" string,
      balance: json |> field "balance" Json.Decode.float,
      name: json |> field "name" string,
      currency: json |> field "currency" Currency.JSON.unmarshal,
      accountType:
        switch (json |> field "type" string) {
        | s when s == Cash.name => Cash
        | s when s == Checking.name => Checking (json |> field "data" Checking.JSON.unmarshal)
        | s when s == CreditCard.name =>
          CreditCard (json |> field "data" CreditCard.JSON.unmarshal)
        | s when s == Savings.name => Savings (json |> field "data" Savings.JSON.unmarshal)
        | s when s == Ira.name => IRA (json |> field "data" Ira.JSON.unmarshal)
        | s when s == Retirement.name =>
          Retirement (json |> field "data" Retirement.JSON.unmarshal)
        | s when s == Crypto.name => Crypto
        | s => Other s
        }
    };
};

let accountType_of_string =
  fun
  | s when s == Cash.name => Cash
  | s when s == Checking.name => Checking Checking.default
  | s when s == CreditCard.name => CreditCard CreditCard.default
  | s when s == Savings.name => Savings Savings.default
  | s when s == Ira.name => IRA Ira.default
  | s when s == Retirement.name => Retirement Retirement.default
  | s when s == Crypto.name => Crypto
  | other => Other other;

type accountTypeLabel = {
  text: string,
  type_: accountType
};

let accountDefaults = [|
  {text: Cash.name, type_: Cash},
  {text: Checking.name, type_: Checking Checking.default},
  {text: CreditCard.name, type_: CreditCard CreditCard.default},
  {text: Savings.name, type_: Savings Savings.default},
  {text: Ira.name, type_: IRA Ira.default},
  {text: Retirement.name, type_: Retirement Retirement.default},
  {text: Crypto.name, type_: Crypto},
  {text: "Other", type_: Other ""}
|];

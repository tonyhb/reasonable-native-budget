type accountType =
  | Cash
  | Checking
  | Savings
  | CreditCard (option string) /* Credit card type, ie "Sapphire Reserve" */
  | IRA string
  | Retirement
  | Crypto
  | Other string;

/* Used for rendering views when configuring accounts */
type printableAccount = {
  text: string,
  type_: accountType
};

let typeArray = [|
  {text: "Cash", type_: Cash},
  {text: "Checking account", type_: Checking},
  {text: "Credit card", type_: CreditCard None},
  {text: "Savings account", type_: Savings},
  {text: "IRA", type_: IRA ""},
  {text: "401k", type_: Retirement},
  {text: "Cryptocurrency", type_: Crypto},
  {text: "Other", type_: Other ""}
|];

let printableAccountName _type =>
  switch (typeArray |> Js.Array.find (fun item => item.type_ == _type)) {
  | Some f => f.text
  | None =>
    switch _type {
    /* This can hit if typeArray is not exhaustive or we have a (CreditCard Some "foo")*/
    | CreditCard (Some cardType) => cardType
    | _ => "Other"
    }
  };


/** TODO: This should depend on currency */
type account = {
  id: string,
  accountType,
  balance: float,
  name: string
};

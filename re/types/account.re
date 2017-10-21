type rewardsType =
  | Cashback float
  | Points
  | Miles;

type taxType =
  | Roth
  | Traditional;

module Cash = {};

module Checking = {
  type t = {bank: option string};
  let default = {bank: None};
};

module Savings = {
  type t = {
    bank: option string,
    interest: float
  };
  let default = {bank: None, interest: 0.01};
};

module CreditCard = {
  type t = {
    type_: option string,
    apr: float,
    openedOn: int,
    rewardsType
  };
  let default = {type_: None, apr: 0., openedOn: 0, rewardsType: Cashback 0.01};
};

module IRA = {
  type t = {taxType};
  let default = {taxType: Traditional};
};

module Retirement = {
  type t = {taxType};
  let default = {taxType: Traditional};
};

type accountType =
  | Cash
  | Checking Checking.t
  | Savings Savings.t
  | CreditCard CreditCard.t
  | IRA IRA.t
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
  | Cash => "Cash"
  | Checking _s => "Checking account"
  | Savings _s => "Savings account"
  | CreditCard _s => "Credit card"
  | IRA _s => "IRA"
  | Retirement _s => "401k"
  | Crypto => "Cryptocurrency"
  | Other _s => "Other";

let accountType_of_string =
  fun
  | "Cash" => Cash
  | "Checking account" => Checking Checking.default
  | "Credit card" => CreditCard CreditCard.default
  | "Savings account" => Savings Savings.default
  | "IRA" => IRA IRA.default
  | "401k" => Retirement Retirement.default
  | "Cryptocurrency" => Crypto
  | other => Other other;

type accountTypeLabel = {
  text: string,
  type_: accountType
};

let accountDefaults = [|
  {text: "Cash", type_: Cash},
  {text: "Checking account", type_: Checking Checking.default},
  {text: "Credit card", type_: CreditCard CreditCard.default},
  {text: "Savings account", type_: Savings Savings.default},
  {text: "IRA", type_: IRA IRA.default},
  {text: "401k", type_: Retirement Retirement.default},
  {text: "Cryptocurrency", type_: Crypto},
  {text: "Other", type_: Other ""}
|];

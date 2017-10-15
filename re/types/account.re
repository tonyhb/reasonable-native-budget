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
  {text: "Checking", type_: Checking},
  {text: "Credit card", type_: CreditCard None},
  {text: "Savings", type_: Savings},
  {text: "IRA", type_: IRA ""},
  {text: "401k", type_: Retirement},
  {text: "Cryptocurrency", type_: Crypto},
  {text: "Other", type_: Other ""}
|];

type account = {
  id: Types.uuid,
  accountType,
  balance: float,
  name: string,
  business: option string, /* ie. Chase */
  openedOn: option int
};

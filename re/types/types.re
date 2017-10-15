type uuid = string;

type accountType =
  | Cash
  | Checking
  | Savings
  | Credit
  | Roth
  | Retirement
  | Crypto
  | Forex
  | Other;

type account = {
  id: uuid,
  name: string,
  accountType,
  balance: float
};

type category =
  | Income string
  /* categories for expenses */
  | Dining string
  | Travel string /* should this be broken down into hotels, etc? */
  | Gas string
  | Groceries string
  | House string /* mortgage, rent */
  | Utilities string
  | Medical string
  | Pet string
  | Household string
  | Auto string
  | Clothing string
  | Charity string
  | Business string
  | Disposable string
  /* categories for transfers */
  | Saving string
  | Investment string;

type occurence =
  | Daily
  | Weekly int
  | Biweekly int
  | SemiMonthly int int
  | Monthly int
  | Quarterly int
  | Yearly int;

type subscription = {
  id: uuid,
  name: string,
  description: string,
  amount: float,
  account, /* The account that this will be debited from */
  currency: Currency.currency, /* The currency that the subscription is in */
  occurence, /* How often this subscription occurs */
  category /* A category for the subscription expense */
};

type latlng = (float, float);

type recipient = {
  id: uuid,
  name: string,
  latlng
};

/* Let you tag entries, eg. under "Ibiza Trip" to see totals for the ibiza trip */
type tag = string;


/** Entry types **/
type income = {
  id: uuid,
  payee: recipient,
  amountIn: float,
  description: string,
  category,
  createdAt: int,
  updatedAt: int
};

type subscriptionExpense = {
  id: uuid,
  subscription,
  account,
  category,
  recipient,
  currency: Currency.currency,
  amountOut: float,
  desciption: string,
  date: int, /* The actual date of the transaction */
  createdAt: int,
  updatedAt: int
};

type expense = {
  id: uuid,
  account,
  category,
  recipient,
  currency: Currency.currency,
  amountOut: float,
  description: string,
  expensable: bool,
  date: int,
  createdAt: int,
  updatedAt: int,
  /** TODO: Based on category and createdAt, is this worth double/triple points? */
  tags: array tag
};

type transfer = {
  id: uuid,
  fromAccount: account,
  toAccount: account,
  fromCurrency: Currency.currency,
  toCurrency: Currency.currency,
  amount: float,
  description: string,
  category,
  createdAt: int,
  updatedAt: int
};

/* an entry in a budget is either an income, expense or transfer */
type entry =
  | Income income
  | Expense expense
  | SubscriptionExpense subscriptionExpense
  | Transfer transfer;

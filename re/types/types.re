type uuid = string;

type occurence =
 | Yearly
 | Monthly
 | Semimonthly
 | Biweekly
 | Weekly
 | Daily;

type subscription = {
  id: uuid,
  name: string,
  description: string,
  amount: float,
  account: Account.t, /* The account that this will be debited from */
  currency: Currency.currencyType, /* The currency that the subscription is in */
  occurence, /* How often this subscription occurs */
  category: Budget.category /* A category for the subscription expense */
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
  category: Budget.category,
  createdAt: int,
  updatedAt: int
};

type subscriptionExpense = {
  id: uuid,
  subscription,
  account: Account.t,
  category: Budget.category,
  recipient,
  currency: Currency.currencyType,
  amountOut: float,
  desciption: string,
  date: int, /* The actual date of the transaction */
  createdAt: int,
  updatedAt: int
};

type expense = {
  id: uuid,
  account: Account.t,
  category: Budget.category,
  recipient,
  currency: Currency.currencyType,
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
  fromAccount: Account.t,
  toAccount: Account.t,
  fromCurrency: Currency.currencyType,
  toCurrency: Currency.currencyType,
  amount: float,
  description: string,
  category: Budget.category,
  createdAt: int,
  updatedAt: int
};

/* an entry in a budget is either an income, expense or transfer */
type entryType =
  | Income income
  | Expense expense
  | SubscriptionExpense subscriptionExpense
  | Transfer transfer;

type entry = {
  id: string,
  date: int,
  source: Account.t,
  description: string,
  amount: float,
  createdAt: int,
  updatedAt: int,
  entryType,
}

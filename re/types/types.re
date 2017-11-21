type occurence =
  | Yearly
  | Monthly
  | Semimonthly
  | Biweekly
  | Weekly
  | Daily;

type subscription = {
  id: string,
  name: string,
  description: string,
  amount: float,
  account: Account.t, /* The account that this will be debited from */
  currency: Currency.currencyType, /* The currency that the subscription is in */
  occurence, /* How often this subscription occurs */
  defaultCategory: Category.t /* A default category for the subscription expense */
};

type latlng = (float, float);

/* Let you tag entries, eg. under "Ibiza Trip" to see totals for the ibiza trip */
type tag = string;

/*** Entry types **/
type income = {
  payee: Recipient.t,
  amountIn: float
};

  /** TODO: Based on category and createdAt, is this worth double/triple points in your credit card etc?  */
type expense = {
  recipient: option(Recipient.t), /* Possible to have no recipients, but not very cool */
  expensable: bool,
  tags: list(tag)
};

type subscriptionExpense = {
  subscription,
  expense
};

type transfer = {
  id: string,
  destination: Account.t,
  toCurrency: Currency.currencyType
};

/* an entry in a budget is either an income, expense or transfer */
type entryType =
  | Income(income)
  | Expense(expense)
  | SubscriptionExpense(subscriptionExpense)
  | Transfer(transfer);

type entry = {
  id: string,
  date: float,
  source: Account.t,
  category: option(Category.t), /* None means uncategorized */
  description: option(string),
  amount: float,
  currency: Currency.currencyType,
  createdAt: float,
  updatedAt: float,
  entryType
};

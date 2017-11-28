type latlng = (float, float);

/* Let you tag entries, eg. under "Thanksgiving 2017" to see totals for thanksgiving 2017 */
type tag = string;

/*** Entry types **/
type income = {
  payee: Recipient.t,
  amountIn: float
};

type expense = {
  recipient: option(Recipient.t), /* Possible to have no recipients, but not very cool */
  expensable: bool,
  tags: list(tag)
};

type subscriptionExpense = {
  subscription: Subscription.t,
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

type t = {
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

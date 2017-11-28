type occurence =
  | Yearly
  | Monthly
  | Semimonthly
  | Biweekly
  | Weekly
  | Daily;

type t = {
  id: string,
  name: string,
  description: string,
  amount: float,
  account: Account.t, /* The account that this will be debited from */
  currency: Currency.currencyType, /* The currency that the subscription is in */
  occurence, /* How often this subscription occurs */
  recipient: Recipient.t,
  defaultCategory: Category.t /* A default category for the subscription expense */
};

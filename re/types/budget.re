type t = {
  settings: Settings.t,
  accounts: array(Account.t),
  categories: list(Category.t),
  recipients: list(Recipient.t),
  /* TODO: Store this in a tree of YYYY-MM nodes? Or a tree of categories? */
  entries: list(Entry.t)
};

let blankBudget = () => {
  settings: {defaultCurrency: Currency.defaultCurrencyType},
  categories: [],
  accounts: [||],
  recipients: [],
  entries: []
};

/** TODO: Based on category and createdAt, is this worth double/triple points in your credit card etc?  */
let entry = (t) : Entry.t => {
  id: Uuid.gen(),
  date: Js.Date.now(),
  description: None,
  amount: 0.,
  cleared: false,
  source: t.accounts[0],
  currency: t.settings.Settings.defaultCurrency,
  category: None,
  entryType: Expense({recipient: None, expensable: false, tags: []}),
  createdAt: 0.,
  updatedAt: 0.
};

let addRecipient = (t: t, recipient: option(Recipient.t)) : (t, option(Recipient.t)) =>
  switch recipient {
  | None => (t, None)
  | Some(r) =>
    let (l, recipient) = Recipient.append(t.recipients, r);
    ({...t, recipients: l}, Some(recipient))
  };

/* TODO: Add expense - should debit from the account */
let addExpense = (t: t, entry: Entry.t, expense: Expense.t) => {
  /** Ensure we add the recipient **/
  let (newBudget, recipient) = addRecipient(t, expense.recipient);
  /** The recipient may have a new ID, so recreate the entry with the new expense **/
  /** TODO: Can we tidy this up, maybe make this a tuple? **/
  let entry = {...entry, entryType: Entry.Expense({...expense, recipient})};
  /** Debit the amount from the account **/
  {
    ...newBudget,
    accounts:
      newBudget.accounts
      |> Array.to_list
      |> Account.addBalanceToList(entry.Entry.source.id, entry.amount *. (-1.))
      |> Array.of_list,
    entries: [entry, ...t.entries]
  }
};

let addIncome = (t: t, entry: Entry.t, income: Income.t) => {
  /* Ensure we add the payee */
  let (newBudget, payee) = addRecipient(t, income.payee);
  /* The recipient may have a new ID, so recreate the entry with the new expense */
  let entry = {...entry, entryType: Entry.Income({payee: payee})};
  {
    ...newBudget,
    accounts:
      newBudget.accounts
      |> Array.to_list
      |> Account.addBalanceToList(entry.Entry.source.id, entry.amount)
      |> Array.of_list,
    entries: [entry, ...t.entries]
  }
};

let addEntry = (t: t, entry: Entry.t) : t => {
  let budget =
    switch entry.entryType {
    | Entry.Expense(e) => addExpense(t, entry, e)
    | Entry.Income(i) => addIncome(t, entry, i)
    | _ => t
    };
  {
    ...budget,
    entries:
      budget.entries
      |> List.stable_sort(
           (a: Entry.t, b: Entry.t) =>
             if (b.date -. a.date > 0.) {
               1
             } else if (b.date -. a.date == 0.) {
               0
             } else {
               (-1)
             }
         )
  }
};

module JSON = {
  let marshal = (data) =>
    Json.Encode.(
      object_([
        ("settings", Settings.JSON.marshal(data.settings)),
        ("categories", Json.Encode.list(Category.JSON.marshal, data.categories)),
        ("accounts", arrayOf(Account.JSON.marshal, data.accounts)),
        ("recipients", Json.Encode.list(Recipient.JSON.marshal, data.recipients)),
        ("entries", Json.Encode.list(Entry.JSON.marshal, data.entries))
      ])
    );
  let unmarshal = (json) => {
    open Json.Decode;
    let accounts = json |> field("accounts", array(Account.JSON.unmarshal));
    let recipients = json |> field("recipients", list(Recipient.JSON.unmarshal));
    let categories = json |> field("categories", list(Category.JSON.unmarshal));
    {
      settings: json |> field("settings", Settings.JSON.unmarshal),
      categories,
      accounts,
      recipients,
      entries:
        json
        |> field(
             "entries",
             list(
               Entry.JSON.unmarshal(~accounts=accounts |> Array.to_list, ~recipients, ~categories)
             )
           )
        |> List.stable_sort(
             (a: Entry.t, b: Entry.t) =>
               if (b.date -. a.date > 0.) {
                 1
               } else if (b.date -. a.date == 0.) {
                 0
               } else {
                 (-1)
               }
           )
    }
  };
};

module Examples = {
  let basic: list(Category.t) = [
    Category.category(~name="Housing", ()),
    Category.category(~name="Bills & Utilities", ()),
    Category.category(~name="Food", ()),
    Category.category(~name="Family", ()),
    Category.category(~name="Health", ()),
    Category.category(~name="Debt", ()),
    Category.category(~name="Travel", ()),
    Category.category(~name="Spending money", ()),
    Category.category(~name="Rainy day & savings", ())
  ];
  let standard: array(Group.t) = [|
    Group.group(
      ~name="Monthly expenses",
      ~data=[|
        Category.category(~name="Housing", ()),
        Category.category(~name="Utilities", ~hint="(eg. phone, water)", ()),
        Category.category(~name="Travel", ~hint="(eg. car, parking, bus)", ()),
        Category.category(~name="Groceries", ()),
        Category.category(~name="Healthcare", ~hint="(eg. insurance, medication)", ()),
        Category.category(~name="Education", ()),
        Category.category(~name="Debt", ())
      |]
    ),
    Group.group(
      ~name="Everyday expenses",
      ~data=[|
        Category.category(~name="Entertainment", ~hint="(eg. netflix, shows)", ()),
        Category.category(~name="Food", ~hint="(eg. restaurants, lunch)", ()),
        Category.category(~name="Memberships", ~hint="(eg. gym)", ()),
        Category.category(~name="Clothing", ()),
        Category.category(~name="Misc", ())
      |]
    ),
    Group.group(
      ~name="Saving goals",
      ~data=[|
        Category.category(~name="Rainy day fund", ()),
        Category.category(~name="Vacation", ()),
        Category.category(~name="Gifts", ()),
        Category.category(~name="Savings", ()),
        Category.category(~name="Retirement", ())
      |]
    )
  |];
};

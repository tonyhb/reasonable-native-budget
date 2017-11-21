type t = {
  settings: Settings.t,
  budget: array(Group.t),
  accounts: array(Account.t)
};

let blankBudget = () => {
  settings: {defaultCurrency: Currency.defaultCurrencyType},
  budget: [||],
  accounts: [||]
};

module JSON = {
  let marshal = (data) =>
    Json.Encode.(
      object_([
        ("settings", Settings.JSON.marshal(data.settings)),
        ("budget", arrayOf(Group.JSON.marshal, data.budget)),
        ("accounts", arrayOf(Account.JSON.marshal, data.accounts))
      ])
    );
  let unmarshal = (json) =>
    Json.Decode.{
      settings: json |> field("settings", Settings.JSON.unmarshal),
      budget: json |> field("budget", array(Group.JSON.unmarshal)),
      accounts: json |> field("accounts", array(Account.JSON.unmarshal))
    };
};

let basic: array(Group.t) = [|
  Group.group(
    ~name="Expenses",
    ~data=[|
      Category.category(~name="Housing", ()),
      Category.category(~name="Bills & Utilities", ()),
      Category.category(~name="Food", ()),
      Category.category(~name="Family", ()),
      Category.category(~name="Health", ()),
      Category.category(~name="Debt", ()),
      Category.category(~name="Travel", ()),
      Category.category(~name="Spending money", ()),
      Category.category(~name="Rainy day & savings", ())
    |]
  )
|];

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

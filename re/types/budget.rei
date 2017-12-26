type t = {
  settings: Settings.t,
  accounts: array(Account.t),
  categories: list(Category.t),
  recipients: list(Recipient.t),
  entries: list(Entry.t)
};

let blankBudget: unit => t;

let entry: (t) => Entry.t;

let addRecipient: (t, option(Recipient.t)) => (t, option(Recipient.t));

let addEntry: (t, Entry.t) => t;

module JSON: {
  let marshal: (t) => Js.Json.t;
  let unmarshal: (Js.Json.t) => t;
};

module Examples: {
  let basic: list(Category.t);
  let standard: array(Group.t);
}

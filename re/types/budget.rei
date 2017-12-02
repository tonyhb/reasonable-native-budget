type t = {
  settings: Settings.t,
  budget: array(Group.t),
  accounts: array(Account.t),
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
  let basic: array(Group.t);
  let standard: array(Group.t);
}

type latlng = (float, float);

/* an entry in a budget is either an income, expense or transfer */
type entryType =
  | Income(Income.t)
  | Expense(Expense.t)
  | Transfer(Transfer.t);

let string_of_entryType =
  fun
  | Income(_i) => "income"
  | Expense(_e) => "expense"
  | Transfer(_t) => "transfer";

type t = {
  id: string,
  date: float,
  source: Account.t,
  category: option(Category.t), /* None means uncategorized */
  description: option(string),
  amount: float,
  currency: Currency.currencyType,
  cleared: bool,
  createdAt: float,
  updatedAt: float,
  entryType
};

let entry = (~amount, ~date, ~source, ~category, ~currency, ~description, ~entry) : t => {
  id: Uuid.gen(),
  date,
  source,
  category,
  amount,
  currency,
  description,
  cleared: false,
  createdAt: Js.Date.now(),
  updatedAt: Js.Date.now(),
  entryType: entry
};

let rec _takeItems = (~l: list(t), ~ret: list(t)=[], ~amount: int) : list(t) =>
  switch amount {
  | 0 => ret
  | _ =>
    switch l {
    | [] => ret
    | [head, ...rest] => _takeItems(~l=rest, ~ret=[head, ...ret], ~amount=amount - 1)
    }
  };

let takeItems = (amount: int, l: list(t)) : list(t) =>
  _takeItems(~l, ~ret=[], ~amount) |> List.rev;

let rec _takeByTime = (~l: list(t), ~startTime: float, ~endTime: float, ~ret: list(t)) : list(t) =>
  switch l {
  | [] => ret
  | [head, ...rest] =>
    /**
        Assume that the list is already sorted in reverse-chronological order
        from newest to oldest entry.  If the item is before the month, we can skip.
        ourselves and run our own find_all function, breaking early once we traverse
        past the requested month.
       **/
    (
      if (head.date < startTime) {
        _takeByTime(~l, ~startTime, ~endTime, ~ret)
      } else if (head.date > endTime) {
        ret
      } else {
        /** This must be within the month range; add it to ~ret and recurse **/
        _takeByTime(~l=rest, ~startTime, ~endTime, ~ret=[head, ...ret])
      }
    )
  };

/** takeMonth returns a new sublist containing entries for the given month. **/
let takeMonth = (month: Date.month, l: list(t)) : list(t) => {
  let startTime = Date.dateOfMonth(month) |> Js.Date.valueOf;
  let (y, m) = month;
  let endTime = Date.dateOfMonth((y, m +. 1.0)) |> Js.Date.valueOf;
  _takeByTime(~l, ~startTime, ~endTime, ~ret=[]) |> List.rev
};

let takeCurrentMonth = (l: list(t)) : list(t) => {
  let now = Js.Date.make();
  let month = (now |> Js.Date.getFullYear, now |> Js.Date.getMonth);
  l |> takeMonth(month)
};

/** takeYear returns a new sublist containing entries for the given year. **/
let takeYear = (year: float, l: list(t)) : list(t) => {
  let startTime = Date.dateOfMonth((year, 0.)) |> Js.Date.valueOf;
  let endTime = Date.dateOfMonth((year, 12.0)) |> Js.Date.valueOf;
  _takeByTime(~l, ~startTime, ~endTime, ~ret=[]) |> List.rev
};

let takeCurrentYear = (l: list(t)) : list(t) =>
  l |> takeYear(Js.Date.make() |> Js.Date.getFullYear);

type entriesByCategory = {
  id: option(string),
  entries: list(t),
  total: float
};

let rec _byCategory =
        (~l: list(t), ~ret: list(entriesByCategory))
        : list(entriesByCategory) =>
  switch l {
  | [] => ret
  | [head, ...rest] =>
    /* Get the category ID as an option */
    let catId =
      switch head.category {
      | None => None
      | Some(cat) => Some(cat.id)
      };
    /** Add head to the specific category within ret, if it exists,
          or create a new list for this category containing head.

          The boolean is mutated to true if the cateogry exists and has
          been prepended with "head".

          If the value is false we assume that the category has not yet
          been seen and we must create a new entriesByCategory for this category ID.
         **/
    let mut = ref(false);
    let copy =
      ret
      |> List.map(
           (cl) =>
             if (catId == cl.id) {
               /** This category is the same as a previous entry;  prepend it to
              the category's list of items */
               (mut.contents = true);
               {...cl, entries: [head, ...cl.entries], total: cl.total +. head.amount}
             } else {
               cl
             }
         );
    if (mut.contents == false) {
      /** This category has not yet been seen.  Recurse into the next list entry
          and ensure that return list has the category id/list prepended. **/
      _byCategory(~l=rest, ~ret=[{id: catId, entries: [head], total: head.amount}, ...ret])
    } else {
      _byCategory(~l=rest, ~ret=copy)
    }
  };

/** byCategory takes a list of entries and returns Js.Object.t keyed by category
  ID each pointing to a list of entries for that category **/
/** TODO: Reverse each category's list? **/
let byCategory = (l: list(t)) => _byCategory(~l, ~ret=[]);

module JSON = {
  let json_of_date = (d: float) : Js.Json.t =>
    d |> Js.Date.fromFloat |> Js.Date.toISOString |> Json.Encode.string;
  let date_of_json = (json: Js.Json.t) : float =>
    Json.Decode.string(json) |> Js.Date.fromString |> Js.Date.valueOf;
  let marshal = (data: t) : Js.Json.t =>
    Json.Encode.(
      object_([
        ("id", string(data.id)),
        ("date", json_of_date(data.date)),
        ("source", string(data.source.id)), /* Only store ID */
        (
          "category",
          switch data.category {
          | Some(c) => string(c.id)
          | None => Json.Encode.null
          }
        ),
        ("description", nullable(string, data.description)),
        ("amount", Json.Encode.float(data.amount)),
        ("currency", Currency.JSON.marshal(data.currency)),
        ("cleared", Json.Encode.bool(data.cleared)),
        ("createdAt", json_of_date(data.date)),
        ("updatedAt", json_of_date(data.date)),
        ("entryType", data.entryType |> string_of_entryType |> string),
        (
          "entryData",
          switch data.entryType {
          | Income(i) => Income.JSON.marshal(i)
          | Expense(e) => Expense.JSON.marshal(e)
          | Transfer(t) => Transfer.JSON.marshal(t)
          }
        )
      ])
    );
  let unmarshal =
      (
        ~accounts: list(Account.t),
        ~recipients: list(Recipient.t),
        ~categories: list(Category.t),
        json: Js.Json.t
      ) =>
    Json.Decode.{
      id: json |> field("id", string),
      date: json |> field("date", date_of_json),
      source: {
        let id = json |> field("source", string);
        accounts |> List.find((acc: Account.t) => acc.id == id)
      },
      category: {
        let id = json |> field("category", optional(string));
        switch id {
        | Some(id) => Some(categories |> List.find((cat: Category.t) => cat.id == id)) /* TODO: Not found */
        | None => None
        }
      },
      description: json |> field("description", optional(string)),
      amount: json |> field("amount", Json.Decode.float),
      currency: json |> field("currency", Currency.JSON.unmarshal),
      cleared: json |> field("cleared", Json.Decode.bool),
      createdAt: json |> field("createdAt", date_of_json),
      updatedAt: json |> field("updatedAt", date_of_json),
      entryType:
        switch (json |> field("entryType", string)) {
        | s when s == "income" =>
          Income(json |> field("entryData", Income.JSON.unmarshal(recipients)))
        | s when s == "transfer" =>
          Transfer(json |> field("entryData", Transfer.JSON.unmarshal(accounts)))
        | s when s == "expense" =>
          Expense(json |> field("entryData", Expense.JSON.unmarshal(recipients)))
        | _ => Expense(json |> field("entryData", Expense.JSON.unmarshal(recipients))) /* Default to expense */
        }
    };
};

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

let rec _take = (~l: list(t), ~ret: list(t)=[], ~amount: int): list(t) => {
  switch amount {
    | 0 => ret
    | _ => {
      switch l {
        |[] => ret
        | [head, ...rest] => _take(~l=rest, ~ret=[head, ...ret], ~amount=(amount - 1))
      }
    }
  }
};


let take = (amount: int, l: list(t)): list(t) => {
  _take(~l=l, ~ret=[], ~amount=amount) |> List.rev
};

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
        /** TODO: Move entry types into their own files, write JSON encodes/decoders for them and add a switch here invoking the correct encoding */
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

type category = {
  id: string,
  name: string,
  amount: float,
  hint: option string
};

let category ::name ::hint=? ::amount=? () => {
  id: Uuid.gen (),
  name,
  amount:
    switch amount {
    | Some a => a
    | None => 0.
    },
  hint
};

type group = {
  id: string,
  data: array category,
  name: string
};

let group ::name ::data => {id: Uuid.gen (), name, data};

let updateCategoryInGroup group (cat: category) => {
  ...group,
  data: group.data |> Array.map (fun (item: category) => item.id == cat.id ? cat : item)
};

let removeCategoryFromGroup group (cat: category) => {
  ...group,
  data:
    group.data
    |> Array.to_list
    |> List.filter (fun (item: category) => item.id != cat.id)
    |> Array.of_list
};

let basic: array group = [|
  group
    name::"Expenses"
    data::[|
      category name::"Housing" (),
      category name::"Bills & Utilities" (),
      category name::"Food" (),
      category name::"Family" (),
      category name::"Health" (),
      category name::"Debt" (),
      category name::"Travel" (),
      category name::"Spending money" (),
      category name::"Rainy day & savings" ()
    |]
|];

let standard: array group = [|
  group
    name::"Monthly expenses"
    data::[|
      category name::"Housing" (),
      category name::"Utilities" hint::"(eg. phone, water)" (),
      category name::"Travel" hint::"(eg. car, parking, bus)" (),
      category name::"Groceries" (),
      category name::"Healthcare" hint::"(eg. insurance, medication)" (),
      category name::"Education" (),
      category name::"Debt" ()
    |],
  group
    name::"Everyday expenses"
    data::[|
      category name::"Entertainment" hint::"(eg. netflix, shows)" (),
      category name::"Food" hint::"(eg. restaurants, lunch)" (),
      category name::"Memberships" hint::"(eg. gym)" (),
      category name::"Clothing" (),
      category name::"Misc" ()
    |],
  group
    name::"Saving goals"
    data::[|
      category name::"Rainy day fund" (),
      category name::"Vacation" (),
      category name::"Gifts" (),
      category name::"Savings" (),
      category name::"Retirement" ()
    |]
|];

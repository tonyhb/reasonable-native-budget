module Category = {
	type t = {
		id: string,
		name: string,
		amount: float,
		hint: option(string)
	};

	let category = (~name, ~hint=?, ~amount=?, ()) => {
		id: Uuid.gen(),
		name,
		amount:
			switch amount {
			| Some(a) => a
			| None => 0.
			},
		hint
	};

	module JSON = {
		let marshal = (cat: t) =>
			Json.Encode.(
				object_([
					("id", string(cat.id)),
					("name", string(cat.name)),
					("amount", Json.Encode.float(cat.amount)),
					("hint", nullable(string, cat.hint))
				])
			);
		let unmarshal = (json) =>
			Json.Decode.{
				id: json |> field("id", string),
				name: json |> field("name", string),
				amount: json |> field("amount", Json.Decode.float),
				hint: json |> field("hint", optional(string))
			};
	};
};

module Group = {
	type t = {
		id: string,
		data: array(Category.t),
		name: string
	};
	let group = (~name, ~data) => {id: Uuid.gen(), name, data};
	let updateCategoryInGroup = (group, cat: Category.t) => {
		...group,
		data: group.data |> Array.map((item: Category.t) => item.id == cat.id ? cat : item)
	};

	let removeCategoryFromGroup = (group, cat: Category.t) => {
		...group,
		data:
			group.data
			|> Array.to_list
			|> List.filter((item: Category.t) => item.id != cat.id)
			|> Array.of_list
	};

	let groupTotal = (group: t) =>
		group.data |> Array.fold_left((total, cat) => total +. cat.Category.amount, 0.);

	let total = (groups: array(t)) =>
		groups |> Array.fold_left((total, group) => total +. groupTotal(group), 0.);

	let module JSON = {
	let marshal = (g) =>
		Json.Encode.(
			object_([
				("id", string(g.id)),
				("name", string(g.name)),
				("data", arrayOf(Category.JSON.marshal, g.data))
			])
		);
	let unmarshal = (json) =>
		Json.Decode.{
			id: json |> field("id", string),
			name: json |> field("name", string),
			data: json |> field("data", array(Category.JSON.unmarshal))
		};
	};
};

type t = {
		settings: Settings.t,
		budget: array(Group.t),
		accounts: array(Account.t)
};

let blankBudget = () => {
  settings: {defaultCurrency: Currency.defaultCurrencyType},
  budget: [||],
  accounts: [||],
};

module JSON = {
		let marshal = (data) => {
				Json.Encode.(
						object_([
								("settings", Settings.JSON.marshal(data.settings)),
								("budget", arrayOf(Group.JSON.marshal, data.budget)),
								("accounts", arrayOf(Account.JSON.marshal, data.accounts)),
						])
				);
		};

		let unmarshal = (json) => Json.Decode.({
				settings: json |> field("settings", Settings.JSON.unmarshal),
				budget: json |> field("budget", array(Group.JSON.unmarshal)),
				accounts: json |> field("accounts", array(Account.JSON.unmarshal)),
		});
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

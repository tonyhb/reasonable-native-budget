open ReactNative;

type state = {
	budget: option(Budget.t),
};

type action =
	| Update(Budget.t)
	| Load(Budget.t);

let component = ReasonReact.reducerComponent("Root");

let make = (_children) => {
	let updateBudget = (self, budget) => {
		self.ReasonReact.reduce(fun () => Update(budget));
	};

	{
		...component,
		initialState: () => {budget: None},
		didMount: (self) => {
			AsyncStorage.getItem("budget", ())
			|> Js.Promise.then_(
					(optStore) => {
						switch optStore {
						| None => ()
						| Some(json) =>
							self.reduce(
								() => Load(json |> Js.Json.parseExn |> Budget.JSON.unmarshal), ()
							)
						};
						Js.Promise.resolve()
					}
				)
			|> ignore;
			ReasonReact.NoUpdate
		},
		reducer: (action, _state) =>
			switch action {
			| Load(budget) => ReasonReact.Update({budget: budget})
			},
		render: (self) =>
			<Navigator budget=self.state.budget updateBudget={updateBudget(self)} />
	};

};
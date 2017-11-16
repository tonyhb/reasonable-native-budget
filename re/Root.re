open ReactNative;

type state = {
  loading: bool,
  hasBudget: bool,
  budget: Budget.t
};

type action =
  | Update(Budget.t, option((unit => unit)))
  | Load(Budget.t)
  | NoBudget;

let component = ReasonReact.reducerComponent("Root");

let make = (_children) => {
  /**
    updateBudget is called from any component in the app to save budget information.
	  a side effect cna be passed to updateBudget, specifically intended for use when we need to
	  navigate to a new route after updating the budget state.Account

	  This will be called within UpdateWithSideEffects after the reducer stores the new budget
	  in state, ensuring that if we change routes via this side effect the new components have the
	  updated budget passed to them from our fresh state.
	*/
  let updateBudget = (~self, ~budget, ~sideEffect: option((unit => unit))) =>
    self.ReasonReact.reduce(() => Update(budget, sideEffect), ());
  {
    ...component,
    initialState: () => {budget: Budget.blankBudget(), hasBudget: false, loading: true},
    didMount: (self) => {
      /**
        immediately retrieve the budget information from localstorage;  if it doesn't exist
        call the noBudget action such that we can render children knowing we are in a fresh
        state.
        */
      AsyncStorage.getItem("budget", ())
      |> Js.Promise.then_(
           (optStore) => {
             switch optStore {
             | None => self.reduce(() => NoBudget, ())
             | Some(json) =>
               self.reduce(() => Load(json |> Js.Json.parseExn |> Budget.JSON.unmarshal), ())
             };
             Js.Promise.resolve()
           }
         )
      |> ignore;
      ReasonReact.NoUpdate
    },
    reducer: (action, state) =>
      switch action {
      | NoBudget => ReasonReact.Update({...state, hasBudget: false, loading: false})
      | Load(budget) => ReasonReact.Update({budget, hasBudget: true, loading: false})
      | Update(budget, sideEffect) =>
        ReasonReact.UpdateWithSideEffects(
          {budget, hasBudget: true, loading: false},
          (
            (_self) => {
              /* Any time we update the budget we want to store it locally */
              let json = budget |> Budget.JSON.marshal |> Js.Json.stringify;
              AsyncStorage.setItem(
                "budget",
                json,
                ~callback=
                  (err) =>
                    switch err {
                    | Some(_e) =>
                      Alert.alert(
                        ~title="Uh oh",
                        ~message="We couldn't save your budget.  Is your phone full?",
                        ()
                      )
                    | None => ()
                    },
                ()
              )
              |> ignore;
              /* if we were passed a side effect of updating the budget - for example, pushing
                 a new state onto history, call it here */
              switch sideEffect {
              | Some(f) => f()
              | None => ()
              }
            }
          )
        )
      },
    render: (self) =>
      /* TODO: Nicer loading screen */
      self.state.loading ?
        <ReactNative.Text value="loading" /> :
        <Navigator
          budget=self.state.budget
          hasBudget=self.state.hasBudget
          updateBudget=(updateBudget(~self))
        />
  }
};

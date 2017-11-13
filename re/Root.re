open ReactNative;

type state = {
  hasBudget: bool,
  budget: Budget.t
};

type action =
  | Update(Budget.t)
  | Load(Budget.t)
  | NoBudget;

let component = ReasonReact.reducerComponent("Root");

let make = (_children) => {
  let updateBudget = (self, budget) => self.ReasonReact.reduce(() => Update(budget), ());
  {
    ...component,
    initialState: () => {budget: Budget.blankBudget(), hasBudget: false},
    didMount: (self) => {
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
      | NoBudget => ReasonReact.Update({...state, hasBudget: false})
      | Load(budget) => ReasonReact.Update({budget, hasBudget: true})
      | Update(budget) =>
        ReasonReact.UpdateWithSideEffects(
          {budget, hasBudget: true},
          (
            (_self) => {
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
                    }
              )
              |> ignore;
              ()
            }
          )
        )
      },
    render: (self) => <Navigator budget=self.state.budget updateBudget=(updateBudget(self)) />
  }
};

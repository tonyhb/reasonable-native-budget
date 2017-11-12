open ReactNative;

type state = {budget: array(Budget.group)};

type action =
  | Load(array(Budget.group));

let component = ReasonReact.reducerComponent("Root");

let make = (_children) => {
  ...component,
  initialState: () => {budget: [||]},
  didMount: (self) => {
    AsyncStorage.getItem("budget", ())
    |> Js.Promise.then_(
         (optStore) => {
           switch optStore {
           | None => ()
           | Some(json) =>
             self.reduce(
               () =>
                 Load(json |> Js.Json.parseExn |> Json.Decode.array(Budget.JSON.unmarshalGroup)),
               ()
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
    switch (Array.length(self.state.budget)) {
    | 0 => <OnboardingNav />
    | _ => <Navigator budget=self.state.budget />
    }
};

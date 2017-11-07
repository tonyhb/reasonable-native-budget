open ReactNative;

type state = {budget: array Budget.group};

type action =
  | Load (array Budget.group);

let component = ReasonReact.reducerComponent "Root";

let make _children => {
  ...component,
  initialState: fun () => {budget: [||]},
  didMount: fun self => {
    AsyncStorage.getItem "budget" ()
    |> Js.Promise.then_ (
         fun optStore => {
           switch optStore {
           | None => ()
           | Some json =>
             self.reduce
               (
                 fun () =>
                   Load (json |> Js.Json.parseExn |> Json.Decode.array Budget.JSON.unmarshalGroup)
               )
               ()
           };
           Js.Promise.resolve ()
         }
       )
    |> ignore;
    ReasonReact.NoUpdate
  },
  reducer: fun action _state =>
    switch action {
    | Load budget => ReasonReact.Update {budget: budget}
    },
  render: fun self =>
    switch (Array.length self.state.budget) {
    | 0 => <OnboardingNav />
    | _ => <Navigator />
    }
};

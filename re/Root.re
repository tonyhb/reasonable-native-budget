open ReactNative;

type state = {settings: option Settings.settings};

type action =
  | Load Settings.settings;

let updateSettings self settings => {
  AsyncStorage.setItem "settings" (settings |> Settings.JSON.marshal |> Js.Json.stringify) ();
  self.ReasonReact.reduce (fun _state => Load settings) ()
};

let component = ReasonReact.reducerComponent "Root";

let make _children => {
  ...component,
  initialState: fun () => ({settings: None}: state),
  didMount: fun self => {
    AsyncStorage.getItem "settings" ()
    |> Js.Promise.then_ (
         fun optStore => {
           switch optStore {
           | None => ()
           | Some string =>
             Js.Json.parseExn string |> Settings.JSON.unmarshal |> updateSettings self
           };
           Js.Promise.resolve ()
         }
       )
    |> ignore;
    ReasonReact.NoUpdate
  },
  reducer: fun action _state =>
    switch action {
    | Load settings => ReasonReact.Update {settings: Some settings}
    },
  render: fun self =>
    switch self.state.settings {
    /*| Some _settings => <Navigator />*/
    | Some _settings => <Welcome onSubmit=(updateSettings self) />
    | None => <Welcome onSubmit=(updateSettings self) />
    }
};

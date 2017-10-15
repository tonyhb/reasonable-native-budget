open ReactNative;

type state = {settings: option Settings.settings};

type action =
  | Load Settings.settings;

let component = ReasonReact.reducerComponent "Root";

let make _children => {
  ...component,
  initialState: fun () => ({settings: None}: state),
  didMount: fun self => {
	  ReactNative.AsyncStorage.getItem "settings" ()
	  	|> Js.Promise.then_ ( fun optStore => {
			  switch optStore {
				| None => ()
				| Some string => {
					let json = Js.Json.parseExn(string) |> Settings.parseSettingsJson;
					self.reduce (fun _ => Load json);
					();
				}
			  };
			  Js.Promise.resolve();
		  })
		|> ignore;
	  ReasonReact.NoUpdate
  },
  reducer: fun action _state =>
	switch action {
	| Load settings => ReasonReact.Update {settings: Some settings}
	},
  render: fun { state: { settings } } => 
  switch settings {
  | Some _settings => <Text value="settings exist" />
  | None => <Welcome />
  }
};
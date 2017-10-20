open ReactNative;

let styles =
  StyleSheet.create
    Style.(
      {
        "wrapper": style [marginTop 15.0, marginBottom 15.0],
        "header":
          style [
            fontSize 12.,
            textAlign `center,
            letterSpacing 1.8,
            fontWeight `bold,
            color "#888",
            margin 15.
          ],
        "footer":
          style [height 45., backgroundColor "#f8f8f8", flexDirection `row, alignItems `center]
      }
    );

module NoEntries = {
  let component = ReasonReact.statelessComponent "NoEntries";
  let make ::nav _children => {
    let onAddAccount _evt => NavigationRe.navigate nav routeName::"NewAccount" ();
    {
      ...component,
      render: fun self =>
        <View style=styles##wrapper>
          <Text style=styles##header value="NO ENTRIES" />
          <View style=styles##footer>
            <Text
              style=Style.(style [color "#474747", textAlign `center, flex 1.])
              value="Add an account to start"
            />
          </View>
        </View>
    }
  };
};

module Card = {
  type state = {entries: list Types.entry};
  type action =
    | Load (list Types.entry);
  let component = ReasonReact.reducerComponent "LatestEntries";
  let make nav::(nav: ReactNavigation.Navigation.t {.}) _children => {
    ...component,
    initialState: fun () => {entries: []},
    didMount: fun _self => ReasonReact.NoUpdate,
    reducer: fun action _state =>
      switch action {
      | Load entries => ReasonReact.Update {entries: entries}
      },
    render: fun self =>
      /** TODO:stack navigator, button to make new account, navigate :D */
      switch self.state.entries {
      | [] => <NoEntries nav />
      | _ => <Text value="Wau many accounts" />
      }
  };
};

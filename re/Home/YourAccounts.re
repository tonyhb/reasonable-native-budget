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
          style [
            height 45.,
            backgroundColor "#f8f8f8",
            flexDirection `row,
            justifyContent `flexEnd
          ]
      }
    );

module NewAccount = {
  type state = {account: Account.account};
  type action =
    | UpdateName string
    | UpdateType Account.accountType
    | UpdateBusiness string
    | UpdateStartingBalance float;
  let component = ReasonReact.reducerComponent "NewAccount";
  let make _children => {
    ...component,
    initialState: fun () => {
      account: {id: "", accountType: Account.Checking, balance: 0.0, name: ""}
    },
    reducer: fun action state =>
      switch action {
      | UpdateName name => ReasonReact.Update {...state, account: {...state.account, name}}
      | UpdateType type_ =>
        ReasonReact.Update {...state, account: {...state.account, accountType: type_}}
      | _ => ReasonReact.NoUpdate
      },
    render: fun self =>
      <View style=Style.(style [backgroundColor "#fff", padding 15.])>
        <Text value="Account type" />
        <Picker
          selectedValue=self.state.account.accountType
          onValueChange=(self.reduce (fun type_ => UpdateType type_))>
          (
            Account.typeArray
            |> Array.map (
                 fun item =>
                   <Picker.Item
                     key=item.Account.text
                     label=item.Account.text
                     value=item.Account.type_
                   />
               )
            |> ReasonReact.arrayToElement
          )
        </Picker>
        <Text value="Account name" />
        <TextInput
          value=self.state.account.name
          onChangeText=(self.reduce (fun v => UpdateName v))
        />
      </View>
  };
};

module NoAccounts = {
  let component = ReasonReact.statelessComponent "NoAccounts";
  let make ::nav _children => {
    let onAddAccount _evt => NavigationRe.navigate nav routeName::"NewAccount" ();
    {
      ...component,
      render: fun self =>
        <View style=styles##wrapper>
          <Text style=styles##header value="NO ACCOUNTS" />
          <View style=styles##footer>
            <TouchableOpacity
              onPress=onAddAccount style=Style.(style [padding 10., paddingRight 15.])>
              <Text
                style=Style.(style [color "#474747", textAlign `right, flex 1., fontSize 14.])
                value="Add an account"
              />
            </TouchableOpacity>
          </View>
        </View>
    }
  };
};

module Card = {
  type state = {accounts: list Account.account};
  type action =
    | Load (list Account.account);
  let component = ReasonReact.reducerComponent "Accounts";
  let make nav::(nav: ReactNavigation.Navigation.t {.}) _children => {
    ...component,
    initialState: fun () => {accounts: []},
    didMount: fun self => ReasonReact.NoUpdate,
    reducer: fun action _state =>
      switch action {
      | Load accounts => ReasonReact.Update {accounts: accounts}
      },
    render: fun self =>
      /** TODO:stack navigator, button to make new account, navigate :D */
      switch self.state.accounts {
      | [] => <NoAccounts nav />
      | _ => <Text value="Wau many accounts" />
      }
  };
};

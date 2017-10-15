open ReactNative;

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
      account: {
        id: "",
        accountType: Account.Checking,
        balance: 0.0,
        name: "",
        business: None,
        openedOn: None
      }
    },
    reducer: fun action state =>
      switch action {
      | UpdateName name => ReasonReact.Update {...state, account: {...state.account, name}}
      | UpdateType type_ =>
        ReasonReact.Update {...state, account: {...state.account, accountType: type_}}
      | _ => ReasonReact.NoUpdate
      },
    render: fun self =>
      <View>
        <Text value="Account type" />
        <Picker
          selectedValue=self.state.account.accountType
          onValueChange=(self.reduce (fun type_ => UpdateType type_))>
          (
            Account.typeArray
            |> Array.map (
                 fun item => <Picker.Item label=item.Account.text value=item.Account.type_ />
               )
            |> ReasonReact.arrayToElement
          )
        </Picker>
      </View>
  };
};

module Card = {
  type state = {accounts: list Account.account};
  type action =
    | Load (list Account.account);
  let component = ReasonReact.reducerComponent "Accounts";
  let make _children => {
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
      | [] => <NewAccount />
      | _ => <Text value="Wau many accounts" />
      }
  };
};

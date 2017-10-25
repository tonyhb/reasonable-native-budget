open ReactNative;

module OnboadringAccountPreview = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "info": style [fontFamily "LFTEtica", fontSize 20., color "#528060", marginBottom 2.],
          "_type": style [fontFamily "LFTEtica", fontSize 12., color "#85A58F"],
          "row": style [flexDirection `row, justifyContent `spaceBetween]
        }
      );
  let c = ReasonReact.statelessComponent "OnboardingAccountPreview";
  let make ::account _children => {
    ...c,
    render: fun _self =>
      <Card>
        <Card.Content>
          <View style=styles##row>
            <Text style=styles##info value=account.Account.name />
            <Text style=styles##info value=("$" ^ Printf.sprintf "%.2f" account.Account.balance) />
          </View>
          <View style=styles##row>
            <Text
              style=styles##_type
              value=(Account.string_of_accountType account.Account.accountType)
            />
          </View>
        </Card.Content>
      </Card>
  };
};

/* OnboardingAccount is used to create accounts during onboarding */
module OnboardingAccount = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "info": style [fontFamily "LFTEtica", fontSize 20., color "#528060", marginBottom 2.],
          "_type": style [fontFamily "LFTEtica", fontSize 12., color "#85A58F"],
          "row": style [flexDirection `row, justifyContent `spaceBetween]
        }
      );
  type state = {
    editing: bool,
    account: Account.t
  };
  type action =
    | ToggleEdit bool
    | UpdateName string
    | UpdateType Account.accountType
    | UpdateBalance float;
  let c = ReasonReact.reducerComponent "OnboardingAccount";
  let make ::onSave ::onRemove ::account _children => {
    ...c,
    initialState: fun () => {account, editing: false},
    reducer: fun action state =>
      switch action {
      | ToggleEdit editing => ReasonReact.Update {...state, editing}
      | UpdateName name => ReasonReact.Update {...state, account: {...state.account, name}}
      | UpdateType accountType =>
        ReasonReact.Update {...state, account: {...state.account, accountType}}
      | UpdateBalance balance =>
        ReasonReact.Update {...state, account: {...state.account, balance}}
      },
    render: fun self => {
      let {account} = self.state;
      switch self.state.editing {
      | false =>
        <TouchableOpacity onPress=(self.reduce (fun _evt => ToggleEdit true))>
          <OnboadringAccountPreview account />
        </TouchableOpacity>
      | true =>
        <Card>
          <Card.Content>
            <View style=Style.(style [flexDirection `row])>
              <Form.Field style=Style.(style [flex 5., marginRight 15.])>
                <Form.Label value="Account name" />
                <Form.TextInput
                  onChangeText=(self.reduce (fun text => UpdateName text))
                  value=self.state.account.name
                  selectTextOnFocus=true
                />
              </Form.Field>
              <Form.Field style=Style.(style [flex 2.])>
                <Form.Label value="Starting Balance" />
                <Form.MoneyInput
                  onChangeFloat=(self.reduce (fun value => UpdateBalance value))
                  value=("$" ^ Printf.sprintf "%.2f" self.state.account.balance)
                  selectTextOnFocus=true
                  style=(Some Style.(style [textAlign `right]))
                />
              </Form.Field>
            </View>
            <Form.Field>
              <Form.Label value="Account type" />
              <Form.Picker
                onValueChange=(
                  self.reduce (fun type_ => UpdateType (Account.accountType_of_string type_))
                )
                selectedValue=(Account.string_of_accountType self.state.account.accountType)>
                (
                  Account.accountDefaults
                  |> Js.Array.map (
                       fun pa =>
                         <Picker.Item
                           key=pa.Account.text
                           value=pa.Account.text
                           label=pa.Account.text
                           color="#528060"
                         />
                     )
                  |> ReasonReact.arrayToElement
                )
              </Form.Picker>
            </Form.Field>
            <View
              style=Style.(
                      style [
                        flexDirection `row,
                        justifyContent `spaceBetween,
                        paddingLeft 15.,
                        paddingRight 15.
                      ]
                    )>
              <Form.DestructiveButton value="Remove" onPress=(fun _e => onRemove account) />
              <Form.PrimaryButton
                value="Save"
                onPress=(
                  self.reduce (
                    fun _e => {
                      onSave self.state.account;
                      ToggleEdit false
                    }
                  )
                )
              />
            </View>
          </Card.Content>
        </Card>
      /* Common account elements */
      }
    }
  };
};

let styles =
  StyleSheet.create
    Style.(
      {
        "wrapper": style [flex 1., backgroundColor "#79BD8F"],
        "wrapperInner":
          style [
            flexDirection `column,
            justifyContent `center,
            padding 15.,
            backgroundColor "#79BD8F"
          ],
        "header": style [height 45., marginBottom 30.],
        "title":
          style [
            fontFamily "LFTEticaDisplayHv",
            fontSize 30.,
            textAlign `center,
            marginBottom 10.,
            color "#fff"
          ],
        "subtitle":
          style [fontFamily "LFTEtica-Bold", fontSize 16., textAlign `center, color "#fff"],
        "content": style [flex 1., flexDirection `column, marginTop 60., marginBottom 60.],
        "small":
          style [
            fontFamily "LFTEtica",
            fontSize 12.,
            textAlign `center,
            color "#fff",
            marginBottom 10.
          ],
        "hint": style [opacity 0.75],
        "add": style [marginTop 10., textDecorationLine `underline]
      }
    );

type state = {accounts: list Account.t};

type actions =
  | Remove Account.t
  | Update Account.t
  | Add;

let comp = ReasonReact.reducerComponent "Welcome";

let make ::onSubmit _children => {
  ...comp,
  initialState: fun () => {
    accounts: [
      {
        id: Uuid.gen (),
        balance: 100.,
        name: "My checking account",
        currency: Currency.defaultCurrencyType,
        accountType: Account.Checking Checking.default
      },
      {
        id: Uuid.gen (),
        balance: 100.,
        name: "Credit Card",
        currency: Currency.defaultCurrencyType,
        accountType: Account.CreditCard CreditCard.default
      },
      {
        id: Uuid.gen (),
        balance: 100.,
        name: "Savings",
        currency: Currency.defaultCurrencyType,
        accountType: Account.Savings Savings.default
      }
    ]
  },
  reducer: fun action state =>
    switch action {
    | Remove item =>
      ReasonReact.Update {accounts: state.accounts |> List.filter (fun acc => acc !== item)}
    | Update item =>
      ReasonReact.SilentUpdate {
        accounts:
          state.accounts |> List.map (fun acc => acc.Account.id == item.Account.id ? item : acc)
      }
    | Add =>
      ReasonReact.Update {
        accounts:
          [
            {
              Account.id: Uuid.gen (),
              balance: 0.,
              name: "Another account",
              currency: Currency.defaultCurrencyType,
              accountType: Account.Checking Checking.default
            }
          ]
          |> List.append state.accounts
      }
    },
  render: fun self =>
    <ScrollView style=styles##wrapper contentContainerStyle=styles##wrapperInner>
      <View style=styles##header />
      <View>
        <Text style=styles##title value="Let's get started!" />
        <Text style=styles##subtitle value="First, let's add some accounts\nto your budget." />
      </View>
      <View style=styles##content>
        <Text
          style=(StyleSheet.flatten [styles##small, styles##hint])
          value="Tap to edit or remove"
        />
        (
          self.state.accounts
          |> List.map (
               fun acc =>
                 <OnboardingAccount
                   key=acc.Account.id
                   account=acc
                   onSave=(self.reduce (fun acct => Update acct))
                   onRemove=(self.reduce (fun acct => Remove acct))
                 />
             )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
        <TouchableOpacity onPress=(self.reduce (fun _ev => Add))>
          <Text style=(StyleSheet.flatten [styles##add, styles##small]) value="Add another" />
        </TouchableOpacity>
      </View>
    </ScrollView>
};

open ReactNative;

module OnboadringAccountPreview = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "info": style [fontFamily "LTFEtica", fontSize 20., color "#528060", marginBottom 2.],
          "_type": style [fontFamily "LTFEtica", fontSize 12., color "#85A58F"],
          "row": style [flexDirection `row, justifyContent `spaceBetween]
        }
      );
  let c = ReasonReact.statelessComponent "OnboardingAccountPreview";
  let make ::name ::accountType ::balance _children => {
    ...c,
    render: fun _self =>
      <Card>
        <Card.Content>
          <View style=styles##row>
            <Text style=styles##info value=name />
            <Text style=styles##info value=("$" ^ string_of_float balance) />
          </View>
          <View style=styles##row>
            <Text style=styles##_type value=(Account.printableAccountName accountType) />
          </View>
        </Card.Content>
      </Card>
  };
};

/* OnboardingAccount is used to create accounts during onboarding */
module OnboardingAccount = {
  let handleUpdateBalance text::(text_: string) =>
    text_ |> Js.String.replace "$" "" |> float_of_string;
  let styles =
    StyleSheet.create
      Style.(
        {
          "info": style [fontFamily "LTFEtica", fontSize 20., color "#528060", marginBottom 2.],
          "_type": style [fontFamily "LTFEtica", fontSize 12., color "#85A58F"],
          "row": style [flexDirection `row, justifyContent `spaceBetween]
        }
      );
  type state = {
    editing: bool,
    account: Account.account
  };
  type action =
    | ToggleEdit bool
    | UpdateName string
    | UpdateType Account.accountType
    | UpdateBalance float
    | UpdateCardType string;
  let c = ReasonReact.reducerComponent "OnboardingAccount";
  let make ::name ::accountType _children => {
    ...c,
    initialState: fun () => {account: {id: "", accountType, balance: 100., name}, editing: false},
    reducer: fun action state =>
      switch action {
      | ToggleEdit editing => ReasonReact.Update {...state, editing}
      | UpdateName name => ReasonReact.Update {...state, account: {...state.account, name}}
      | UpdateType accountType =>
        ReasonReact.Update {...state, account: {...state.account, accountType}}
      | UpdateBalance balance =>
        ReasonReact.Update {...state, account: {...state.account, balance}}
      | UpdateCardType _type =>
        ReasonReact.Update {
          ...state,
          account: {...state.account, accountType: Account.CreditCard (Some _type)}
        }
      },
    render: fun self =>
      switch self.state.editing {
      | false =>
        <TouchableOpacity onPress=(self.reduce (fun _evt => ToggleEdit true))>
          <OnboadringAccountPreview name accountType balance=self.state.account.balance />
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
                  value=("$" ^ string_of_float self.state.account.balance)
                  selectTextOnFocus=true
                  style=(Some Style.(style [textAlign `right]))
                />
              </Form.Field>
            </View>
            <Form.Field>
              <Form.Label value="Account type" />
              <Form.Picker
                onValueChange=(self.reduce (fun type_ => UpdateType type_))
                selectedValue=self.state.account.accountType>
                (
                  Account.typeArray
                  |> Js.Array.map (
                       fun pa =>
                         <Picker.Item
                           key=pa.Account.text
                           value=pa.Account.type_
                           label=pa.Account.text
                           color="#528060"
                         />
                     )
                  |> ReasonReact.arrayToElement
                )
              </Form.Picker>
            </Form.Field>
          </Card.Content>
        </Card>
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

let comp = ReasonReact.statelessComponent "Welcome";

let make ::onCreate _children => {
  let onPressNew _event _self => {
    let settings: Settings.settings = {defaultCurrency: Currency.defaultCurrencyType};
    onCreate settings;
    ()
  };
  {
    ...comp,
    render: fun _self =>
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
          <OnboardingAccount name="My checking account" accountType=Account.Checking />
          <OnboardingAccount name="Savings city" accountType=Account.Savings />
          <OnboardingAccount name="Credit card central" accountType=(Account.CreditCard None) />
          <TouchableOpacity>
            <Text style=(StyleSheet.flatten [styles##add, styles##small]) value="Add another" />
          </TouchableOpacity>
        </View>
      </ScrollView>
  }
};

open ReactNative;

/* OnboardingAccount is used to create accounts during onboarding */
module OnboardingAccount = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "info": style [fontFamily "LTFEtica", fontSize 20., color "#528060", marginBottom 2.],
          "_type": style [fontFamily "LTFEtica", fontSize 12., color "#85A58F"],
          "row": style [flexDirection `row, justifyContent `spaceBetween]
        }
      );
  type state = {account: Account.account};
  type action =
    | UpdateName string
    | UpdateType Account.accountType
    | UpdateStartingBalance float
    | UpdateCardType string;
  let c = ReasonReact.reducerComponent "OnboardingAccount";
  let make ::name ::accountType _children => {
    ...c,
    initialState: fun () => {account: {id: "", accountType, balance: 100., name}},
    reducer: fun action state =>
      switch action {
      | UpdateName name => ReasonReact.Update {account: {...state.account, name}}
      | UpdateType accountType => ReasonReact.Update {account: {...state.account, accountType}}
      | UpdateStartingBalance balance => ReasonReact.Update {account: {...state.account, balance}}
      | UpdateCardType _type =>
        ReasonReact.Update {
          account: {...state.account, accountType: Account.CreditCard (Some _type)}
        }
      },
    render: fun self =>
      <Card>
        <Card.Content>
          <View style=styles##row>
            <Text style=styles##info value=name />
            <Text style=styles##info value=("$" ^ string_of_float self.state.account.balance) />
          </View>
          <View style=styles##row>
            <Text
              style=styles##_type
              value=(Account.printableAccountName self.state.account.accountType)
            />
          </View>
        </Card.Content>
      </Card>
  };
};

let styles =
  StyleSheet.create
    Style.(
      {
        "wrapper":
          style [
            flex 1.,
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
        "hint":
          style [
            fontFamily "LFTEtica",
            fontSize 12.,
            textAlign `center,
            color "#fff",
            marginBottom 10.
          ]
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
      <View style=styles##wrapper>
        <View style=styles##header />
        <View>
          <Text style=styles##title value="Let's get started!" />
          <Text style=styles##subtitle value="First, let's add some accounts\nto your budget." />
        </View>
        <View style=styles##content>
          <Text style=styles##hint value="Tap to edit or remove" />
          <OnboardingAccount name="My checking account" accountType=Account.Checking />
          <OnboardingAccount name="Savings city" accountType=Account.Savings />
          <OnboardingAccount name="Credit card central" accountType=(Account.CreditCard None) />
        </View>
      </View>
  }
};

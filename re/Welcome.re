open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper":
          style([
            flex(1.),
            backgroundColor("#79BD8F"),
            flexDirection(`column),
            justifyContent(`center),
            padding(15.)
          ]),
        "title":
          style([
            fontFamily("LFTEticaDisplayHv"),
            color("#fff"),
            fontSize(30.),
            textAlign(`center),
            padding(15.)
          ]),
        "cta": style([fontFamily("LFTEtica-Bold"), color("#85A58F"), textAlign(`center)])
      }
    )
  );

let c = ReasonReact.statelessComponent("Onboarding.Intro");

let make = (~hasBudget, ~budget, ~nav, _children) => {
  ...c,
  render: (_self) =>
    hasBudget ?
      <Wrapper nav> <Home budget nav key="home" /> </Wrapper> :
      <View style=styles##wrapper>
        <Text style=styles##title value="App Name" />
        <TouchableOpacity
          onPress=(
            (_evt) =>
              ReactNavigation.Navigation.navigate(nav, ~routeName="/onboarding/accounts", ())
          )>
          <Card>
            <Card.Content> <Text style=styles##cta value="Start new budget" /> </Card.Content>
          </Card>
        </TouchableOpacity>
      </View>
};

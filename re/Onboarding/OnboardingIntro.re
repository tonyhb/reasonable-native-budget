open ReactNative;

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

let make = (~nav: ReactNavigation.Navigation.t({.}), _children) => {
  let navToNew = NavigationRe.navigate(nav, ~routeName="NewAccounts");
  {
    ...c,
    render: (_self) =>
      <View style=styles##wrapper>
        <Text style=styles##title value="App Name" />
        <TouchableOpacity onPress=((_evt) => navToNew())>
          <Card>
            <Card.Content> <Text style=styles##cta value="Start new budget" /> </Card.Content>
          </Card>
        </TouchableOpacity>
      </View>
  }
};

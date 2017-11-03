open ReactNative;

module Wrapper = {
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
            ]
        }
      );
  let c = ReasonReact.statelessComponent "Onboarding.Wrapper";
  let make children => {
    ...c,
    render: fun _self =>
      <ScrollView style=styles##wrapper contentContainerStyle=styles##wrapperInner>
        (View.make children |> ReasonReact.element)
      </ScrollView>
  };
};

module Header = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "header": style [flex 1., marginTop 45.],
          "title":
            style [
              fontFamily "LFTEticaDisplayHv",
              color "#fff",
              fontSize 30.,
              textAlign `center,
              padding 15.
            ],
          "subtitle":
            style [fontFamily "LFTEtica-Bold", fontSize 16., textAlign `center, color "#fff"]
        }
      );
  let c = ReasonReact.statelessComponent "Onboarding.Header";
  let make ::title ::subtitle _children => {
    ...c,
    render: fun _self =>
      <View style=styles##header>
        <Text style=styles##title value=title />
        <Text style=styles##subtitle value=subtitle />
      </View>
  };
};

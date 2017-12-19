open BsReactNative;
let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([flex(1.)]),
        "nav": style([height(50.), backgroundColor("#F9F9F9"), flexDirection(`row)]),
        "button":
          style([
            padding(5.),
            paddingLeft(15.),
            paddingRight(15.),
            borderTopColor("#E7E7E7"),
            borderTopWidth(1.),
            flex(1.)
          ]),
        "buttonImage": style([height(25.)]),
        "buttonText": style([fontFamily("LFTEtica"), fontSize(10.), textAlign(`center)])
      }
    )
  );

module NavButton = {
  let c = ReasonReact.statelessComponent("Wrapper.NavButton");
  let make = (~text, ~isActive=false, _children) => {
    ...c,
    render: (_self) =>
      <View style=styles##button>
        <View style=styles##buttonImage />
        <Text style=styles##buttonText value=(text |> String.uppercase) />
      </View>
  };
};

let c = ReasonReact.statelessComponent("Wrapper");

let make = (~nav, children) => {
  ...c,
  render: (_self) =>
    <View style=styles##wrapper>
      <View style=styles##wrapper> (children |> ReasonReact.arrayToElement) </View>
      <View style=styles##nav>
        <NavButton text="Entries" />
        <NavButton text="Accounts" />
        <NavButton text="Budget" />
        <NavButton text="Reports" />
      </View>
    </View>
};

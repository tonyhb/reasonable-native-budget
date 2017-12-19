open BsReactNative;

module Subheader = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "subheader":
            style([
              fontFamily("LFTEtica"),
              fontSize(12.),
              color("#CCC"),
              letterSpacing(1.),
              marginTop(15.),
              marginBottom(5.)
            ])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Type.Header");
  let make = (~value, ~style=?, _children) => {
    ...c,
    render: (_self) => <Text value=(value |> String.uppercase) style=styles##subheader />
  };
};

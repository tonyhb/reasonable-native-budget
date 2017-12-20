open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper":
          style([
            height(35.),
            paddingTop(11.),
            flexDirection(`row),
            borderTopWidth(1.),
            borderBottomWidth(1.),
            borderTopColor("#F0F0F0"),
            borderBottomColor("#F0F0F0"),
            backgroundColor("#FAFAFA"),
            marginTop(30.),
          ]),
        "header": style([
            fontFamily("LFTEtica-Bold"),
            fontSize(10.),
            letterSpacing(1.),
            color("#A1A6A6"),
            textAlign(`center),
            flex(1.),
        ])
      }
    )
  );

let c = ReasonReact.statelessComponent("SectionHeader");

let make = (~value, ~filter=?, _children) => {
  ...c,
  render: (_self) =>
    <View style=styles##wrapper>
      <Text style=styles##header value=(value |> String.uppercase) />
    </View>
};


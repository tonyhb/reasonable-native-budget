open ReactNative;

module Content = {
  let c = ReasonReact.statelessComponent("Card.Content");
  let make = (children) => {
    ...c,
    render: (_self) =>
      <View style=Style.(style([padding(15.)]))>
        (View.make(children) |> ReasonReact.element)
      </View>
  };
};

let styles =
  StyleSheet.create(
    Style.(
      {
        "card":
          style([
            backgroundColor("#fff"),
            borderRadius(2.),
            elevation(2.),
            shadowColor("#000"),
            shadowOffset(~width=0., ~height=2.),
            shadowOpacity(0.15),
            marginTop(1.5),
            marginBottom(1.5)
          ])
      }
    )
  );

let c = ReasonReact.statelessComponent("Card");

let make = (~style=?, children) => {
  ...c,
  render: (_self) =>
    switch style {
    | Some(s) =>
      <View style=(StyleSheet.flatten([styles##card, s]))>
        (View.make(children) |> ReasonReact.element)
      </View>
    | None => <View style=styles##card> (View.make(children) |> ReasonReact.element) </View>
    }
};

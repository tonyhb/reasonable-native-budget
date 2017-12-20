open BsReactNative;

module Header = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "header":
            style([
              fontFamily("LFTEtica-Bold"),
              fontSize(18.),
              color("#79BD8F"),
              marginTop(30.),
              marginBottom(20.),
              textAlign(`center)
            ])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Type.Header");
  let make = (~value, _children) => {
    ...c,
    render: (_self) => <Text value style=styles##header />
  };
};

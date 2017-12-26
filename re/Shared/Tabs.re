open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([flexDirection(`row), backgroundColor("#79BD8F"), height(40.)]),
        "tab": style([flex(1.), backgroundColor("#79BD8F"), height(40.), paddingTop(13.)]),
        "activeTab": style([borderBottomWidth(6.), height(34.), borderBottomColor("#ffffffcc")]),
        "tabText":
          style([
            fontFamily("LFTEtica"),
            color("#fff"),
            textAlign(`center),
            fontSize(12.),
            letterSpacing(1.),
          ]),
        "activeTabText": style([fontFamily("LFTEtica-Bold"), color("#fff"), textAlign(`center),
        ])
      }
    )
  );

module Tab = {
  let c = ReasonReact.statelessComponent("Tabs.Tab");
  let make = (~value, ~onPress, ~isActive=false, _children) => {
    ...c,
    render: (_self) =>
      <TouchableOpacity onPress style=Style.(style([flex(1.)]))>
        <View
          style=(isActive ? StyleSheet.flatten([styles##activeTab, styles##tab]) : styles##tab)>
          <Text
            value=(value |> String.uppercase)
            style=(
              isActive ?
                StyleSheet.flatten([styles##tabText, styles##activeTabText]) : styles##tabText
            )
          />
        </View>
      </TouchableOpacity>
  };
};

let c = ReasonReact.statelessComponent("Tabs");

let make = (children) => {
  ...c,
  render: (_self) => <View style=styles##wrapper> (children |> ReasonReact.arrayToElement) </View>
};

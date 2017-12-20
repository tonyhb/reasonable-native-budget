open BsReactNative;

let styles = StyleSheet.create(
  Style.({
    "introChart": style([ height(70.)])
  })
);

let c = ReasonReact.statelessComponent("Budget");
let make = (~budget, ~nav, _children) => {
  ...c,
  render: (_self) =>
    <Wrapper nav>
      <View key="budget" style=Style.(style([flex(1.)]))>
        <Tabs>
          <Tabs.Tab key="view" value="View" onPress=(() => ()) isActive=true />
          <Tabs.Tab key="edit" value="Edit" onPress=(() => ()) isActive=false />
        </Tabs>

        <Type.Header value="Your Budget" />

        <View style=styles##introChart>
          <PieChart
            data=[|PieChart.value(45.), {value: 150., label: None}|]
            config=(
              PieChart.config(~colors=[|"#FFFF9D", "#5B98A0", "#79BD8F"|], ~valueTextSize=0., ())
            )
            style=styles##introChart
            rotationAngle=270
            holeRadius=62
            transparentCircleRadius=64
          />
        </View>

      </View>
    </Wrapper>
};

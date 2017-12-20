open BsReactNative;

let c = ReasonReact.statelessComponent("Budget");

let make = (~budget, ~nav, _children) => {
  ...c,
  render: (_self) =>
    <Wrapper nav>
      <View key="budget" style=Style.(style([flex(1.)]))>
        <Text value="test" />
        <PieChart
          data=[|PieChart.value(50.), {value: 150., label: None}|]
          label="FOOOD"
          config=(
            PieChart.config(~colors=[|"#FFFF9D", "#5B98A0", "#79BD8F"|], ~valueTextSize=0., ())
          )
          style=Style.(style([flex(1.), height(100.)]))
          rotationAngle=270
          holeRadius=50
          transparentCircleRadius=52
        />
      </View>
    </Wrapper>
};

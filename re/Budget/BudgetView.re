open BsReactNative;

let c = ReasonReact.statelessComponent("Budget");
let make = (~budget, ~nav, _children) => {
  ...c,
  render: (_self) => {
    <Wrapper nav>
      <View key="budget" style=Style.(style([flex(1.)]))>
        <Text value="test" />
        <PieChart
          data=[|{value: 50., label: "first"}, {value: 150., label: "second"}|]
          label="FOOOD"
          config=PieChart.config(~colors=[|"#FFFF9D", "#5B98A0", "#79BD8F"|], ())
          style=Style.(style([flex(1.), height(200.)]))
          rotationAngle=270
          holeRadius=50
          transparentCircleRadius=55
        />
      </View>
    </Wrapper>
  }
}


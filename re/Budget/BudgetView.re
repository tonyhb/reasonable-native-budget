open BsReactNative;

module CategoryItem = {
  let c = ReasonReact.statelessComponent("BudgetView.CategoryItem");
  let make = (~category, _children) => {...c, render: (_self) => <View />};
};

let styles =
  StyleSheet.create(
    Style.(
      {
        "introChartWrapper": style([height(70.), marginTop(40.)]),
        "introChart": style([height(70.)]),
        "totals": style([marginTop(15.), flexDirection(`column)]),
        "row": style([marginTop(5.), flexDirection(`row), justifyContent(`center)]),
        "spent": style([fontFamily("LFTEtica"), color("#528060")]),
        "bold": style([fontFamily("LFTEtica-Bold"), color("#528060")]),
        "small": style([opacity(0.9), fontSize(11.)])
      }
    )
  );

let c = ReasonReact.statelessComponent("Budget");

let make = (~budget: Budget.t, ~nav, _children) => {
  ...c,
  render: (_self) =>
    <Wrapper value="Your Budget" nav>
      <View key="budget" style=Style.(style([flex(1.)]))>
        <Tabs>
          <Tabs.Tab key="view" value="View" onPress=(() => ()) isActive=true />
          <Tabs.Tab key="edit" value="Edit" onPress=(() => ()) isActive=false />
        </Tabs>
        <View style=styles##introChartWrapper>
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
        <View style=styles##totals>
          <View style=styles##row>
            <Text style=styles##bold value="$3,211" />
            <Text style=styles##spent value=" of $6,500 spent this month" />
          </View>
          <View style=styles##row>
            <Text style=styles##small>
              <Text style=styles##bold value="$9,123" />
              <Text style=styles##spent value=" spent this year" />
            </Text>
          </View>
        </View>
        <SectionHeader value="Categories" />
        (
          budget.categories
          |> List.map((c: Category.t) => <CategoryItem category=c key=c.id />)
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )

      </View>
    </Wrapper>
};

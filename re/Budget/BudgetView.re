open BsReactNative;

module CategoryItem = {
  let c = ReasonReact.statelessComponent("BudgetView.CategoryItem");
  let make =
      (~category: Category.t, ~summary: Entry.entriesByCategory, ~budget: Budget.t, _children) => {
    ...c,
    render: (_self) =>
      <View>
        <Type.Header value=category.name />
        <Text value=("$" ++ Printf.sprintf("%.0f", summary.total)) />
      </View>
  };
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
  let entriesByMonth = budget.entries |> Entry.takeCurrentMonth;
  let total = entriesByMonth |> List.fold_left((sum: float, i: Entry.t) => sum +. i.amount, 0.);
  let budgetTotal =
    budget.categories |> List.fold_left((sum: float, i: Category.t) => sum +. i.amount, 0.);
  let mc = entriesByMonth |> Entry.byCategory;
  {
    ...c,
    render: (_self) =>
      <Wrapper value="Your Budget" nav>
        <Tabs key="tabnav">
          <Tabs.Tab key="view" value="View" onPress=(() => ()) isActive=true />
          <Tabs.Tab key="edit" value="Edit" onPress=(() => ()) isActive=false />
        </Tabs>
        <ScrollView key="budget" contentContainerStyle=Style.(style([flexGrow(1.)]))>
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
              <Text style=styles##bold value=(Printf.sprintf("$%.2f", total)) />
              <Text
                style=styles##spent
                value=(Printf.sprintf(" of $%.2f spent this month", budgetTotal))
              />
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
            |> List.map(
                 (c: Category.t) => {
                   /** Find the entries for this category as provided by Entry.byCategory above **/
                   let summary =
                     switch (mc |> List.find((i: Entry.entriesByCategory) => i.id == Some(c.id))) {
                     | exception Not_found => {id: Some(c.id), entries: [], total: 0.}
                     | c => c
                     };
                   <CategoryItem category=c key=c.id summary budget />
                 }
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </ScrollView>
      </Wrapper>
  }
};

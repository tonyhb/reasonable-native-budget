open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "introChartWrapper": style([height(70.), marginTop(40.)]),
        "introChart": style([height(70.)]),
        "totals": style([marginTop(15.), flexDirection(`column)]),
        "rowCenter": style([marginTop(5.), flexDirection(`row), justifyContent(`center)]),
        "row": style([marginTop(5.), flexDirection(`row)]),
        "spent": style([fontFamily("LFTEtica"), color("#528060")]),
        "bold": style([fontFamily("LFTEtica-Bold"), color("#528060")]),
        "small": style([opacity(0.9), fontSize(12.)])
      }
    )
  );

module CategoryItem = {
  let catStyles =
    StyleSheet.create(
      Style.(
        {
          "wrapper":
            style([
              padding(15.),
              paddingTop(15.),
              paddingBottom(15.),
              borderBottomColor("#f5f5f5"),
              borderBottomWidth(1.)
            ]),
          "name": style([fontFamily("LFTEtica"), fontSize(16.), color("#528060")])
        }
      )
    );
  let c = ReasonReact.statelessComponent("BudgetView.CategoryItem");
  let make =
      (~name: string, ~summary: Entry.entriesByCategory, _children) => {
    ...c,
    render: (_self) =>
      <View style=catStyles##wrapper>
        <Text value=name style=catStyles##name />
        <View style=styles##row>
          <Text value=(Printf.sprintf("$%.2f", summary.total)) style=styles##bold />
          <Text value=" spent this month" style=styles##spent />
        </View>
      </View>
  };
};

let c = ReasonReact.statelessComponent("Budget");

let make = (~budget: Budget.t, ~nav, _children) => {
  let entriesByMonth = budget.entries |> Entry.takeCurrentMonth;
  let entriesByYear = budget.entries |> Entry.takeCurrentYear;
  let monthTotal =
    entriesByMonth |> List.fold_left((sum: float, i: Entry.t) => sum +. i.amount, 0.);
  let budgetTotal =
    budget.categories |> List.fold_left((sum: float, i: Category.t) => sum +. i.amount, 0.);
  let yearTotal = entriesByYear |> List.fold_left((sum, i: Entry.t) => sum +. i.amount, 0.);
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
              data=(
                /** Iterate through all budget totals and add them to the pie chart **/ mc
                |> List.map((i: Entry.entriesByCategory) => PieChart.value(i.total))
                |> List.append([PieChart.value(budgetTotal -. monthTotal)])
                |> List.filter((amt: PieChart.value) => amt.value > 0.)
                |> List.rev
                |> Array.of_list
              )
              config=(
                /**
                  TODO: Make the number of colors match up with categories; manually wrap
                  modulo # colors if more eategories than colors.  Last color for "unspent"
                  should always be grey
                 **/
                PieChart.config(~colors=[|"#FFFF9D", "#5B98A0", "#79BD8F"|], ~valueTextSize=0., ())
              )
              style=styles##introChart
              rotationAngle=270
              holeRadius=62
              transparentCircleRadius=64
            />
          </View>
          <View style=styles##totals>
            <View style=styles##rowCenter>
              <Text style=styles##bold value=(Printf.sprintf("$%.2f", monthTotal)) />
              <Text
                style=styles##spent
                value=(Printf.sprintf(" of $%.2f spent this month", budgetTotal))
              />
            </View>
            <View style=styles##rowCenter>
              <Text style=styles##small>
                <Text style=styles##bold value=(Printf.sprintf("$%.2f", yearTotal)) />
                <Text style=styles##spent value=" spent this year" />
              </Text>
            </View>
          </View>
          <SectionHeader value="Categories" />
          /***
            Uncategorized items should always come first - see if we have any uncategorized
            expenditure and if so render a category block for these
           **/
          (switch ((mc |> List.hd).id) {
            | None => <CategoryItem name="Uncategorized items" summary=(mc |> List.hd) />
            | _ => ReasonReact.nullElement
          })

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
                   <CategoryItem name=c.name key=c.id summary />
                 }
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </ScrollView>
      </Wrapper>
  }
};

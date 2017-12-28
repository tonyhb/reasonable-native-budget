open BsReactNative;

module Header = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "header":
            style([
              flexDirection(`row),
              justifyContent(`center),
              paddingTop(10.),
              height(110.),
              alignItems(`center),
              backgroundColor("#72B387")
            ]),
          "amount":
            style([
              fontFamily("LFTEticaDisplayHv"),
              color("#fff"),
              fontSize(30.),
              textAlign(`center)
            ]),
          "cent":
            style([
              fontFamily("LFTEticaDisplayHv"),
              color("#fff"),
              fontSize(15.),
              marginTop((-15.)),
              marginLeft(1.),
              opacity(0.8)
            ])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Home.Header");
  let make = (~amount: float, _children) => {
    let centAmount = Printf.sprintf("%.0f", mod_float(amount, 1.) *. 100.);
    {
      ...c,
      render: (_self) =>
        <View style=styles##header>
          <Text value=("$" ++ Printf.sprintf("%.0f", floor(amount))) style=styles##amount />
          <Text value=(centAmount == "0" ? "00" : centAmount) style=styles##cent />
        </View>
    }
  };
};

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([backgroundColor("#fff")]),
        "wrapperInner":
          style([flexGrow(1.), flexDirection(`column), justifyContent(`center), backgroundColor("#fff")]),
        "content": style([padding(15.)]),
        "add":
          style([flexDirection(`column), alignItems(`center), paddingTop(30.), paddingBottom(30.)]),
        "addHeader":
          style([fontFamily("LFTEtica-Bold"), fontSize(24.), color("#528060"), textAlign(`center)]),
        "addText": style([fontFamily("LFTEtica"), fontSize(14.), color("#528060")])
      }
    )
  );

let component = ReasonReact.statelessComponent("Home");

let make = (~budget: Budget.t, ~nav, _children) => {
  ...component,
  render: (_self) =>
    <ScrollView style=styles##wrapper contentContainerStyle=styles##wrapperInner>
      <Header amount=(Account.sum(budget.Budget.accounts |> Array.to_list)) />
      <View style=styles##content>
        <TouchableOpacity
          onPress=(() => ReactNavigation.Navigation.navigate(nav, ~routeName="/entries/new", ()))>
          <Card style=styles##add>
            <Text value="Add" style=styles##addHeader />
            <Text value="an entry to your budget" style=styles##addText />
          </Card>
        </TouchableOpacity>
      </View>
      <View style=Style.(style([backgroundColor("#fff"), flex(1.), padding(15.)]))>
        /* TODO: Zero entry, blank slate */
        <EntryList entries=(budget.entries |> Entry.takeItems(10)) />
      </View>
    </ScrollView>
};

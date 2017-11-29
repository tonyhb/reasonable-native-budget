open ReactNative;

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
    };
  };
};

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([backgroundColor("#79BD8F")]),
        "wrapperInner":
          style([flexDirection(`column), justifyContent(`center), backgroundColor("#79BD8F")]),
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

let make = (~budget, ~nav, _children) => {
  ...component,
  render: (_self) =>
    <ScrollView style=styles##wrapper contentContainerStyle=styles##wrapperInner>
      <Header
        amount=(
          budget.Budget.accounts
          |> Array.fold_left((total, acc) => total +. acc.Account.balance, 0.)
        )
      />
      <View style=styles##content>
        <TouchableOpacity onPress=(fun () => ReactNavigation.Navigation.navigate(nav, ~routeName="/entries/new", ()))>
          <Card style=styles##add>
            <Text value="Add" style=styles##addHeader />
            <Text value="an entry to your budget" style=styles##addText />
          </Card>
        </TouchableOpacity>
        <Text value=(Printf.sprintf("%d", List.length(budget.entries)) ++ " entries") />
      </View>
    </ScrollView>
};

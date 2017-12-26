open BsReactNative;

module GroupHeader = {
  let c = ReasonReact.statelessComponent("Onboarding.GroupHeader");
  let make = (~value, _children) => {...c, render: (_self) => <Text value />};
};

module GroupFooter = {
  let c = ReasonReact.statelessComponent("Onboarding.GroupHeader");
  let make = (_children) => {
    ...c,
    render: (_self) =>
      <View style=Style.(style([flexDirection(`row), paddingLeft(22.), marginTop((-10.))]))>
        <Form.Button
          style=Style.(style([fontFamily("LFTEtica"), fontSize(12.), color("#79BD8F")]))
          value="Add a subcategory"
          onPress=(() => ())
        />
      </View>
  };
};

module BudgetCategory = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "wrapper":
            style([
              flexDirection(`row),
              alignItems(`center),
              paddingLeft(15.),
              paddingRight(15.),
              paddingBottom(15.),
              flex(1.)
            ]),
          "remove": style([marginRight(5.)]),
          "name": style([flex(5.), marginRight(15.)]),
          "amount": style([flex(3.), textAlign(`right)])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Onboarding.BudgetParent");
  let make = (~category: Category.t, ~onChange, ~onRemove, _children) => {
    let updateName = (name) => onChange({...category, name}, ());
    let updateAmount = (amount) => onChange({...category, amount}, ());
    {
      ...c,
      render: (_self) =>
        <View style=styles##wrapper>
          <TouchableOpacity
            onPress=onRemove style=Style.(style([padding(5.), paddingLeft(0.), paddingRight(8.)]))>
            <Image
              style=styles##remove
              source=(Image.Required(Packager.require("../../../../data/img/close.png")))
            />
          </TouchableOpacity>
          <Form.TextInput onEndEditing=updateName value=category.name style=styles##name />
          <Form.MoneyInput
            onChangeFloat=updateAmount
            value=("$" ++ Printf.sprintf("%.2f", category.amount))
            style=styles##amount
          />
        </View>
    }
  };
};

type state = {categories: list(Category.t)};

type actions =
  | ResetBudget(list(Category.t))
  | UpdateCategory(Category.t)
  | RemoveCategory(Category.t);

let styles =
  StyleSheet.create(
    Style.(
      {
        "content": style([marginTop(20.), flex(1.)]),
        "total":
          style([
            textAlign(`center),
            color("#fff"),
            fontFamily("LFTEtica-Bold"),
            fontSize(12.),
            padding(10.),
            paddingRight(15.),
            letterSpacing(1.)
          ])
      }
    )
  );

let c = ReasonReact.reducerComponent("Onboarding.BudgetWrapper");

let make = (~budget, ~updateBudget, ~nav, _children) => {
  {
    ...c,
    initialState: () => {categories: Budget.Examples.basic},
    reducer: (action, state) =>
      switch action {
      | UpdateCategory(cat) =>
        ReasonReact.Update({
          categories: state.categories |> List.map((c: Category.t) => c.id == cat.id ? cat : c)
        })
      | RemoveCategory(cat) =>
        ReasonReact.Update({
          categories: state.categories |> List.filter((c: Category.t) => c.id !== cat.id)
        })
      | ResetBudget(categories) => ReasonReact.Update({categories: categories})
      },
    render: (self) =>
      <OnboardingCommon.Wrapper>
        <OnboardingCommon.Header
          title="Make your budget"
          subtitle="You can change this any time - and\nyou should, to reflect your spending."
        />
        <View style=styles##content>
          <Card>
            <View style=Style.(style([paddingTop(15.), paddingBottom(10.)]))>
              <ScrollView>
              (self.state.categories |> List.map((c: Category.t) =>
                      <BudgetCategory
                        onChange=((c) => self.reduce((_evt) => UpdateCategory(c)))
                        onRemove=(self.reduce((_evt) => RemoveCategory(c)))
                        category=c
                      />
                ) |> Array.of_list |> ReasonReact.arrayToElement
              )
              </ScrollView>
            </View>
          </Card>
          <Text
            style=styles##total
            value=(
              "BUDGETED  TOTAL, MONTHLY: $"
              ++ Printf.sprintf("%.2f", (self.state.categories |> List.fold_left((total, c) => total +. c.Category.amount, 0.0)))
            )
          />
          <View style=Style.(style([flexDirection(`row), justifyContent(`center)]))>
            <Form.Button
              style=Style.(style([fontFamily("LFTEtica-Bold"), color("#fff")]))
              value="Continue"
              onPress=(
                self.handle(
                  (_t, self) => {
                    let newBudget: Budget.t = {...budget, categories: self.state.categories};
                    updateBudget(
                      ~budget=newBudget,
                      ~sideEffect=
                        Some(() => ReactNavigation.Navigation.navigate(nav, ~routeName="/", ()))
                    )
                  }
                )
              )
            />
          </View>
        </View>
      </OnboardingCommon.Wrapper>
  }
};

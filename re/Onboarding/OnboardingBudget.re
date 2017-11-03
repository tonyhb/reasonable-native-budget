open ReactNative;

open SectionList;

let defaultStandardBudget =
  Budget.basic
  |> Array.map (
       fun group => SectionList.section data::group.Budget.data key::group.Budget.name ()
     )
  |> SectionList.sections;

module GroupHeader = {
  let c = ReasonReact.statelessComponent "Onboarding.GroupHeader";
  let make value _children => {...c, render: fun _self => <Text value />};
};

module BudgetCategory = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "wrapper":
            style [
              flexDirection `row,
              alignItems `center,
              paddingLeft 15.,
              paddingRight 15.,
              paddingBottom 15.,
              flex 1.
            ],
          "remove": style [marginRight 5.],
          "name": style [flex 5., marginRight 15.],
          "amount": style [flex 3., textAlign `right]
        }
      );
  let c = ReasonReact.statelessComponent "Onboarding.BudgetParent";
  let make category::(category: Budget.category) ::onChange ::onRemove _children => {
    let updateName name => onChange {...category, name} ();
    let updateAmount amount => onChange {...category, amount} ();
    {
      ...c,
      render: fun _self =>
        <View style=styles##wrapper>
          <TouchableOpacity
            onPress=onRemove style=Style.(style [padding 5., paddingLeft 0., paddingRight 8.])>
            <Image
              style=styles##remove
              source=(Image.Required (Packager.require "../../../../data/img/close.png"))
            />
          </TouchableOpacity>
          <Form.TextInput onEndEditing=updateName value=category.name style=styles##name />
          <Form.MoneyInput
            onChangeFloat=updateAmount
            value=("$" ^ Printf.sprintf "%.2f" category.amount)
            style=styles##amount
          />
        </View>
    }
  };
};

type state = {budget: array Budget.group};

type actions =
  | ResetBudget (array Budget.group)
  | UpdateGroup Budget.group
  | UpdateCategory Budget.category
  | RemoveCategory Budget.category;

let styles = StyleSheet.create Style.({"content": style [marginTop 20., flex 1.]});

let c = ReasonReact.reducerComponent "Onboarding.BudgetWrapper";

let make nav::(_nav: ReactNavigation.Navigation.t {.}) _children => {
  let sectionsOfBudget budget =>
    budget
    |> Array.map (
         fun group => SectionList.section data::group.Budget.data key::group.Budget.name ()
       )
    |> SectionList.sections;
  {
    ...c,
    initialState: fun () => {budget: Budget.basic},
    reducer: fun action state =>
      switch action {
      | UpdateGroup group =>
        ReasonReact.Update {
          budget: state.budget |> Array.map (fun item => item.Budget.id == group.id ? group : item)
        }
      | UpdateCategory cat =>
        ReasonReact.Update {
          budget: state.budget |> Array.map (fun group => Budget.updateCategoryInGroup group cat)
        }
      | RemoveCategory cat =>
        ReasonReact.Update {
          budget: state.budget |> Array.map (fun group => Budget.removeCategoryFromGroup group cat)
        }
      | ResetBudget item => ReasonReact.Update {budget: item}
      },
    render: fun self =>
      <OnboardingCommon.Wrapper>
        <OnboardingCommon.Header
          title="Make your budget"
          subtitle="You can change this any time - and\nyou should, to reflect your spending."
        />
        <View style=styles##content>
          <Card>
            <View style=Style.(style [paddingTop 15., paddingBottom 10.])>
              <SectionList
                sections=(self.state.budget |> sectionsOfBudget)
                renderItem=(
                  renderItem (
                    fun data =>
                      <BudgetCategory
                        onChange=(fun item => self.reduce (fun _evt => UpdateCategory item))
                        onRemove=(self.reduce (fun _evt => RemoveCategory data.item))
                        category=data.item
                      />
                  )
                )
                keyExtractor=(fun cat _index => cat.name)
                stickySectionHeadersEnabled=true
              />
            </View>
          </Card>
          <View style=Style.(style [flexDirection `row, justifyContent `center])>
            <Form.Button
              style=Style.(style [fontFamily "LFTEtica-Bold", color "#fff"])
              value="Continue"
              onPress=(fun () => ())
            />
          </View>
        </View>
      </OnboardingCommon.Wrapper>
  }
};

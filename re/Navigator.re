/** TODO: Allow `/onboarding/accounts` to pass accounts/budget as navigation param for `/onboarding/budget` **/
type navParams = {. budget: option(Budget.t)};

type screenProps = {
  budget: Budget.t,
  hasBudget: bool,
  updateBudget: (~budget: Budget.t, ~sideEffect: option((unit => unit))) => unit
};

let budget = (nav, screenProps) => {
  let budget = ReactNavigation.Navigation.state(nav).params##budget;
  switch budget {
  | Some(b) => b
  | None => screenProps.budget
  }
};

let home = (nav: ReactNavigation.Navigation.t(navParams), screenProps: screenProps) =>
  <Welcome budget=screenProps.budget hasBudget=screenProps.hasBudget nav />;

let navOptions = StackNavigatorRe.navigationOptions(~header=`notVisible, ());

let routes: StackNavigatorRe.routes(screenProps, navParams) =
  StackNavigatorRe.(
    routes([
      (
        "/",
        route(
          ~screen=({navigation, screenProps}) => home(navigation, screenProps),
          ~path="/",
          ~navigationOptions=`static(navOptions),
          ()
        )
      ),
      (
        "/onboarding/accounts",
        route(
          ~screen=
            ({navigation, screenProps}) =>
              <OnboardingAccounts budget=screenProps.budget nav=navigation />,
          ~path="/onboarding/accounts",
          ~navigationOptions=`static(navOptions),
          ()
        )
      ),
      (
        "/onboarding/budget",
        route(
          ~screen=
            ({navigation, screenProps}) => {
              <OnboardingBudget
                budget=budget(navigation, screenProps)
                updateBudget=screenProps.updateBudget
                nav=navigation
              />
            },
          ~path="/onboarding/budget",
          ~navigationOptions=`static(navOptions),
          ()
        )
      ),
      (
        "/entries/new",
        route(
          ~screen=
            ({navigation, screenProps}) =>
              <NewEntry
                budget=screenProps.budget
                updateBudget=screenProps.updateBudget
                nav=navigation
              />,
          ~path="/entries/new",
          ~navigationOptions=`static(navOptions),
          ()
        )
      )
    ])
  );

let stackRouter = StackNavigatorRe.create(routes, None);

let make = (~budget, ~hasBudget, ~updateBudget, children) =>
  stackRouter(~screenProps={budget, hasBudget, updateBudget}, children);

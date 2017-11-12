type navParams = {.};

let welcome = (navigation: ReactNavigation.Navigation.t(navParams)) =>
  <OnboardingIntro nav=navigation />;

let accounts = (navigation: ReactNavigation.Navigation.t(navParams)) =>
  <OnboardingAccounts nav=navigation />;

let budget = (navigation: ReactNavigation.Navigation.t(navParams)) =>
  <OnboardingBudget nav=navigation />;

let navOptions = StackNavigatorRe.navigationOptions(~header=`notVisible, ());

let routes: StackNavigatorRe.routes(string, {.}) =
  StackNavigatorRe.(
    routes([
      (
        "Welcome",
        route(
          ~screen=({navigation}) => welcome(navigation),
          ~navigationOptions=`static(navOptions),
          ()
        )
      ),
      (
        "NewAccounts",
        route(
          ~screen=({navigation}) => accounts(navigation),
          ~navigationOptions=`static(navOptions),
          ()
        )
      ),
      (
        "NewBudget",
        route(
          ~screen=({navigation}) => budget(navigation),
          ~navigationOptions=`static(navOptions),
          ()
        )
      )
    ])
  );

let stackRouter = StackNavigatorRe.create(routes, None);

let make = (children) => stackRouter(~screenProps=(), children);

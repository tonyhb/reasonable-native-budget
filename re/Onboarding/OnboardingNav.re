open ReactNative;

type navParams = {.};

let welcome (navigation: ReactNavigation.Navigation.t navParams) =>
  <OnboardingIntro nav=navigation />;

let accounts (navigation: ReactNavigation.Navigation.t navParams) =>
  <OnboardingAccounts nav=navigation />;

let navOptions = StackNavigatorRe.navigationOptions header::`notVisible ();

let routes: StackNavigatorRe.routes string {.} =
  StackNavigatorRe.(
    routes [
      (
        "Welcome",
        route
          screen::(fun {navigation} => welcome navigation)
          navigationOptions::(`static navOptions)
          ()
      ),
      (
        "NewAccounts",
        route
          screen::(fun {navigation} => accounts navigation)
          navigationOptions::(`static navOptions)
          ()
      )
    ]
  );

let stackRouter = StackNavigatorRe.create routes None;

let make children => stackRouter screenProps::() children;

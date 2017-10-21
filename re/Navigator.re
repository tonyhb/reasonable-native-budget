open ReactNative;

type navParams = {.};

let home (navigation: ReactNavigation.Navigation.t navParams) => <Home nav=navigation />;

let navOptions = StackNavigatorRe.navigationOptions header::`notVisible ();

let routes: StackNavigatorRe.routes string {.} =
  StackNavigatorRe.(
    routes [
      (
        "Home",
        route
          screen::(fun {navigation} => home navigation) navigationOptions::(`static navOptions) ()
      )
    ]
  );

let stackRouter = StackNavigatorRe.create routes None;

let make children => stackRouter screenProps::() children;

open BsReactNative;

module TopBar = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "topbar":
            style([
              backgroundColor("#72B387"),
              flexDirection(`row),
              Platform.os == Platform.IOS ? paddingTop(15.) : paddingTop(0.),
              Platform.os == Platform.IOS ? height(60.) : height(45.)
            ]),
          "back": style([margin(15.), width(22.), height(22.)]),
          "settings": style([margin(13.), width(22.), height(22.)]),
          "spacer": style([width(22.)]),
          "text":
            style([
              flex(1.),
              fontFamily("LFTEtica-Bold"),
              color("#FFF"),
              fontSize(16.),
              textAlign(`center),
              marginTop(15.)
            ])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Wrapper.TopBar");
  let make = (~value: option(string), ~showBackButton=false, ~nav, _children) => {
    ...c,
    render: (_self) =>
      <View style=styles##topbar>
        (
          showBackButton ?
            <TouchableOpacity
              style=styles##back onPress=((_evt) => ReactNavigation.Navigation.goBack(nav, ()))>
              <Image source=(Image.Required(Packager.require("../../../../data/img/back.png"))) />
            </TouchableOpacity> :
            <View style=styles##spacer />
        )
        (
          switch value {
          | Some(text) => <Text value=text style=styles##text />
          | None => <Text value="" style=styles##text />
          }
        )
        <TouchableOpacity
          style=styles##settings
          onPress=((_evt) => ReactNavigation.Navigation.navigate(nav, ~routeName="/settings", ()))>
          <Image source=(Image.Required(Packager.require("../../../../data/img/settings.png"))) />
        </TouchableOpacity>
      </View>
  };
};

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([flex(1.), backgroundColor("#fff")]),
        "nav": style([height(50.), backgroundColor("#F9F9F9"), flexDirection(`row)]),
        "button":
          style([
            padding(5.),
            paddingLeft(15.),
            paddingRight(15.),
            borderTopColor("#E7E7E7"),
            borderTopWidth(1.),
            flex(1.)
          ]),
        "buttonImage": style([height(25.)]),
        "buttonText": style([fontFamily("LFTEtica"), fontSize(10.), textAlign(`center)]),
        "activeButtonText": style([fontFamily("LFTEtica-Bold"), fontSize(10.), textAlign(`center)])
      }
    )
  );

module NavButton = {
  let c = ReasonReact.statelessComponent("Wrapper.NavButton");
  let make = (~text, ~isActive=false, ~onPress, _children) => {
    ...c,
    render: (_self) =>
      <TouchableOpacity onPress style=styles##wrapper>
        <View style=styles##button>
          <View style=styles##buttonImage />
          <Text
            value=(text |> String.uppercase)
            style=(isActive ? styles##activeButtonText : styles##buttonText)
          />
        </View>
      </TouchableOpacity>
  };
};

let c = ReasonReact.statelessComponent("Wrapper");

let make = (~nav, ~value=?, ~showTopBar=true, ~showBackButton=true, children) => {
  ...c,
  render: (_self) =>
    <View style=styles##wrapper>
      (showTopBar ? <TopBar value showBackButton nav /> : ReasonReact.nullElement)
      <View style=styles##wrapper> (children |> ReasonReact.arrayToElement) </View>
      <View style=styles##nav>
        <NavButton
          text="Budget"
          onPress=(() => ReactNavigation.Navigation.navigate(nav, ~routeName="/budget", ()))
        />
        <NavButton text="Accounts" onPress=(() => ()) />
        <NavButton text="Subscriptions" onPress=(() => ()) />
      </View>
    </View>
};

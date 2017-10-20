open ReactNative;

let styles =
  StyleSheet.create
    Style.(
      {
        "wrapper":
          style [flex 1., flexDirection `column, justifyContent `flexStart, backgroundColor "#fff"],
        "header":
          style [flexDirection `row, height 120., alignItems `center, backgroundColor "#f4f4f4"],
        "content": style []
      }
    );

let component = ReasonReact.statelessComponent "Home";

let make nav::(nav: ReactNavigation.Navigation.t {.}) _children => {
  ...component,
  render: fun _self =>
    <View style=styles##wrapper>
      <View style=styles##header />
      <View style=styles##content> <YourEntries.Card nav /> <YourAccounts.Card nav /> </View>
    </View>
};

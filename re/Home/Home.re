open ReactNative;

let component = ReasonReact.statelessComponent "Home";

let make _children => {
  ...component,
  render: fun _self =>
    <View> <Text> (ReasonReact.stringToElement "FOO") </Text> <YourAccounts.Card /> </View>
};

open ReactNative;

let comp = ReasonReact.statelessComponent "Welcome";

let make ::onCreate _children => {
  let onPressNew _event _self => {
    let settings: Settings.settings = {defaultCurrency: Currency.defaultCurrencyType};
    onCreate settings;
    ()
  };
  {
    ...comp,
    render: fun self =>
      <View>
        <Button onPress=(self.handle onPressNew) title="Get started">
          <Text value="Welcome!" />
        </Button>
      </View>
  }
};

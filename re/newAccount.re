open ReactNative;

let styles =
  StyleSheet.create
    Style.({"wrapper": style [flex 1., flexDirection `row, width 100.], "input": style [flex 1.]});

type action =
  | Change string;

type state = {value: string};

let comp = ReasonReact.reducerComponent "NewAccount";

let make _children => {
  ...comp,
  initialState: fun () => {value: ""},
  reducer: fun action _state =>
    switch action {
    | Change value => ReasonReact.Update {value: value}
    },
  render: fun self =>
    <View style=styles##wrapper>
      <TextInput
        onChangeText=(self.reduce (fun value => Change value))
        value=self.state.value
        style=styles##input
      />
    </View>
};

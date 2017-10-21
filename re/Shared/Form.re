open ReactNative;

module Field = {
  let styles =
    StyleSheet.create Style.({"field": style [flexDirection `column, paddingBottom 20.]});
  let c = ReasonReact.statelessComponent "Form.Field";
  let make ::style=? children => {
    ...c,
    render: fun _self =>
      switch style {
      | Some s =>
        View.make style::(StyleSheet.flatten [styles##field, s]) children |> ReasonReact.element
      | None => View.make style::styles##field children |> ReasonReact.element
      }
  };
};

module Label = {
  let styles =
    StyleSheet.create
      Style.(
        {"label": style [fontFamily "LTFEtica", color "#A1A6A6", letterSpacing 1., fontSize 10.]}
      );
  let c = ReasonReact.statelessComponent "Form.Label";
  let make ::value _children => {
    ...c,
    render: fun _self => <Text style=styles##label value=(Js.String.toUpperCase value) />
  };
};

module TextInput = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "input":
            style [
              fontFamily "LTFEtica",
              fontSize 14.0,
              color "#528060",
              borderBottomWidth 2.,
              borderBottomColor "#EBEBEB",
              padding 0.,
              paddingBottom 6.,
              paddingTop 5.,
              paddingLeft 10.
            ]
        }
      );
  let c = ReasonReact.statelessComponent "TextInput";
  let make ::value ::onChangeText ::selectTextOnFocus=false _children => {
    ...c,
    render: fun _self =>
      <ReactNative.TextInput
        underlineColorAndroid="transparent"
        onChangeText
        value
        selectTextOnFocus
        style=styles##input
      />
  };
};

/*
 MoneyInp0ut is a text input which stores the text representation of a float in
 its internal state and propagates the changed value to the given onChangeFloat handler
 after the monetary value has finished editing.
 */
module MoneyInput = {
  type state = string;
  type action =
    | Change string;
  /* with retained props as the blur handler calls a prop which passes new props -
     the formatted string */
  let c = ReasonReact.reducerComponent "Form.MoneyInput";
  let make ::value ::onChangeFloat ::selectTextOnFocus=false _children => {
    let handleBlur _evt self => {
      let regex = [%bs.re "/[^0-9.]/g"];
      self.ReasonReact.state |> Js.String.replaceByRe regex "" |> float_of_string |> onChangeFloat
    };
    {
      ...c,
      willReceiveProps: fun _self => value,
      initialState: fun () => value,
      reducer: fun action state =>
        switch action {
        | Change value => ReasonReact.Update value
        },
      render: fun self =>
        <ReactNative.TextInput
          style=TextInput.styles##input
          keyboardType=`numeric
          value=self.state
          selectTextOnFocus
          onChangeText=(self.reduce (fun text => Change text))
          onBlur=(self.handle handleBlur)
        />
    }
  };
};

module Picker = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "picker": style [borderBottomWidth 2., borderBottomColor "#EBEBEB", padding 0.],
          "item": style [fontFamily "LTFEtica"]
        }
      );
  let c = ReasonReact.statelessComponent "Form.Picker";
  let make ::onValueChange ::selectedValue ::enabled=true ::mode=`dropdown children => {
    ...c,
    render: fun _self =>
      <Picker onValueChange enabled selectedValue mode style=styles##picker itemStyle=styles##item>
        (children |> ReasonReact.arrayToElement)
      </Picker>
  };
};
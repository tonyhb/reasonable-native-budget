open ReactNative;

module Button = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "text":
            style [
              fontFamily "LFTEtica",
              color "#6AA67D",
              textDecorationLine `underline,
              padding 15.
            ]
        }
      );
  let c = ReasonReact.statelessComponent "Form.Button";
  let make value::(value: string) ::onPress ::style=? _children => {
    ...c,
    render: fun _self =>
      <TouchableOpacity onPress>
        <Text
          style=(
            switch style {
            | Some s => StyleSheet.flatten [styles##text, s]
            | None => styles##text
            }
          )
          value
        />
      </TouchableOpacity>
  };
};

module PrimaryButton = {
  let c = ReasonReact.statelessComponent "Form.DesructiveButton";
  let make value::(value: string) ::onPress _children => {
    ...c,
    render: fun _self => <Button value onPress style=Style.(style [fontFamily "LFTEtica-Bold"]) />
  };
};

module DestructiveButton = {
  let c = ReasonReact.statelessComponent "Form.DesructiveButton";
  let make value::(value: string) ::onPress _children => {
    ...c,
    render: fun _self => <Button value onPress style=Style.(style [color "#A92B3A"]) />
  };
};

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
        {"label": style [fontFamily "LFTEtica", color "#A1A6A6", letterSpacing 1., fontSize 10.]}
      );
  let c = ReasonReact.statelessComponent "Form.Label";
  let make ::rightAlign=? ::value _children => {
    ...c,
    render: fun _self => {
      let style =
        switch rightAlign {
        | Some _v => StyleSheet.flatten [styles##label, Style.(style [textAlign `right])]
        | None => styles##label
        };
      <Text style value=(Js.String.toUpperCase value) />
    }
  };
};

type textEvt = Js.t {. nativeEvent : {. text : string}};

module TextInput = {
  let styles =
    StyleSheet.create
      Style.(
        {
          "input":
            style [
              fontFamily "LFTEtica",
              fontSize 14.0,
              color "#528060",
              borderBottomWidth 1.,
              borderBottomColor "#EBEBEB",
              padding 0.,
              paddingBottom 6.,
              paddingTop 5.,
              paddingLeft 0.
            ]
        }
      );
  type state = {value: string};
  type actions =
    | Change string;
  let c = ReasonReact.reducerComponent "TextInput";
  let make ::style=? ::value ::onChangeText=? ::onEndEditing=? ::selectTextOnFocus=false _children => {
    /* onEnd is a function that calls onEndEditing with the current value, if passed from the parent */
    let onEnd _evt self =>
      switch onEndEditing {
      | Some f => f self.ReasonReact.state
      | None => ()
      };
    {
      ...c,
      initialState: fun () => value,
      reducer: fun action _state =>
        switch action {
        | Change s => ReasonReact.Update s
        },
      render: fun self =>
        <ReactNative.TextInput
          underlineColorAndroid="transparent"
          onChangeText=(
            self.reduce (
              fun text => {
                /* Trigger the onChangeText event from the parent if passed down */
                switch onChangeText {
                | Some t => t text
                | _ => ()
                };
                /* Update the component's internal value */
                Change text
              }
            )
          )
          onEndEditing=(self.handle onEnd)
          value=self.state
          selectTextOnFocus
          style=(
            switch style {
            | Some s => StyleSheet.flatten [styles##input, s]
            | None => styles##input
            }
          )
        />
    }
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
  let make ::style=? ::value ::onChangeFloat ::selectTextOnFocus=true _children => {
    let handleBlur _evt self => {
      let regex = [%bs.re "/[^0-9.]/g"];
      self.ReasonReact.state |> Js.String.replaceByRe regex "" |> float_of_string |> onChangeFloat
    };
    {
      ...c,
      willReceiveProps: fun _self => value,
      initialState: fun () => value,
      reducer: fun action _state =>
        switch action {
        | Change value => ReasonReact.Update value
        },
      render: fun self =>
        <ReactNative.TextInput
          keyboardType=`numeric
          value=self.state
          selectTextOnFocus
          onChangeText=(self.reduce (fun text => Change text))
          onBlur=(self.handle handleBlur)
          style=(
            switch style {
            | Some s => StyleSheet.flatten [TextInput.styles##input, s]
            | None => TextInput.styles##input
            }
          )
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
          "item": style [fontFamily "LFTEtica"]
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

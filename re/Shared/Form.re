open ReactNative;

let applyStyle = (default, extra) =>
  switch extra {
  | None => default
  | Some(s) => StyleSheet.flatten([default, s])
  };

module Button = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "text":
            style([
              fontFamily("LFTEtica"),
              color("#6AA67D"),
              textDecorationLine(`underline),
              padding(15.)
            ])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Form.Button");
  let make = (~value: string, ~onPress, ~style=?, _children) => {
    ...c,
    render: (_self) =>
      <TouchableOpacity onPress>
        <Text style=(applyStyle(styles##text, style)) value />
      </TouchableOpacity>
  };
};

module PrimaryButton = {
  let c = ReasonReact.statelessComponent("Form.DesructiveButton");
  let make = (~value: string, ~onPress, _children) => {
    ...c,
    render: (_self) => <Button value onPress style=Style.(style([fontFamily("LFTEtica-Bold")])) />
  };
};

module DestructiveButton = {
  let c = ReasonReact.statelessComponent("Form.DesructiveButton");
  let make = (~value: string, ~onPress, _children) => {
    ...c,
    render: (_self) => <Button value onPress style=Style.(style([color("#A92B3A")])) />
  };
};

module Field = {
  let styles =
    StyleSheet.create(Style.({"field": style([flexDirection(`column), paddingBottom(20.)])}));
  let c = ReasonReact.statelessComponent("Form.Field");
  let make = (~style=?, children) => {
    ...c,
    render: (_self) =>
      View.make(~style=applyStyle(styles##field, style), children) |> ReasonReact.element
  };
};

module Label = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "label":
            style([fontFamily("LFTEtica"), color("#A1A6A6"), letterSpacing(1.), fontSize(10.)])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Form.Label");
  let make = (~textAlign=?, ~value, _children) => {
    ...c,
    render: (_self) => {
      let style =
        switch textAlign {
        | Some(`right) =>
          StyleSheet.flatten([styles##label, Style.style([Style.textAlign(`right)])])
        | Some(`center) =>
          StyleSheet.flatten([styles##label, Style.style([Style.textAlign(`center)])])
        | _ => styles##label
        };
      <Text style value=(Js.String.toUpperCase(value)) />
    }
  };
};

type textEvt = {. "nativeEvent": {. text: string}};

module TextInput = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "input":
            style([
              fontFamily("LFTEtica"),
              fontSize(14.0),
              color("#528060"),
              borderBottomWidth(1.),
              borderBottomColor("#EBEBEB"),
              padding(0.),
              paddingBottom(6.),
              paddingTop(5.),
              paddingLeft(0.)
            ])
        }
      )
    );
  type state = {
    /* The typed in value */
    value: string,
    /* The autocomplete value */
    autocomplete: option(string)
  };
  type actions =
    | Change(string);
  let c = ReasonReact.reducerComponent("TextInput");
  /**
    TODO: Make autocomplete a prefix trie for speed
   */
  let make =
      (
        ~style=?,
        ~value,
        ~onChangeText=?,
        ~onEndEditing=?,
        ~selectTextOnFocus=false,
        ~autocomplete: option(list(string))=?,
        _children
      ) => {
    /* onEnd is a function that calls onEndEditing with the current value, if passed from the parent */
    let onEnd = (_evt, self) =>
      switch onEndEditing {
      | Some(f) => f(self.ReasonReact.state.value)
      | None => ()
      };
    {
      ...c,
      initialState: () => {value, autocomplete: None},
      reducer: (action, state) =>
        switch action {
        | Change(newValue) =>
          /* Find whether there's an autocomplete that satisfies our text */
          switch autocomplete {
          | Some(items) =>
            /*
              Only find a new autocomplete if we've added extra input to our value;
              if we're deleting characters we shouldn't attempt to autocomplete
             */
            if (Js.String.length(state.value) < Js.String.length(newValue)) {
              switch (
                items
                |> List.find(
                     (item) => item |> Js.String.indexOf(newValue |> Js.String.toLowerCase) > (-1)
                   )
              ) {
              | item => ReasonReact.Update({value: newValue, autocomplete: Some(item)})
              | exception Not_found => ReasonReact.Update({value: newValue, autocomplete: None})
              }
            } else {
              ReasonReact.Update({value: newValue, autocomplete: None})
            }
          | None => ReasonReact.Update({value: newValue, autocomplete: None})
          }
        },
      render: (self) =>
        <ReactNative.TextInput
          underlineColorAndroid="transparent"
          onChangeText=(
            self.reduce(
              (text) => {
                /* Trigger the onChangeText event from the parent if passed down */
                switch onChangeText {
                | Some(t) => t(text)
                | _ => ()
                };
                /* Update the component's internal value */
                Change(text)
              }
            )
          )
          onEndEditing=(self.handle(onEnd))
          value=(
            switch self.state.autocomplete {
            | Some(value) => value /* If we have autocomplete render it as the value */
            | None => self.state.value
            }
          )
          selection=(
            switch self.state.autocomplete {
            | Some(value) => {
                "start": self.state.value |> Js.String.length,
                "_end": value |> Js.String.length
              }
            | None => {
                "start": self.state.value |> Js.String.length,
                "_end": self.state.value |> Js.String.length
              }
            }
          )
          selectTextOnFocus
          style=(applyStyle(styles##input, style))
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
    | Change(string);
  /* with retained props as the blur handler calls a prop which passes new props -
     the formatted string */
  let c = ReasonReact.reducerComponent("Form.MoneyInput");
  let make = (~style=?, ~value, ~onChangeFloat, ~selectTextOnFocus=true, _children) => {
    let handleBlur = (_evt, self) => {
      let regex = [%bs.re "/[^0-9.]/g"];
      self.ReasonReact.state
      |> Js.String.replaceByRe(regex, "")
      |> float_of_string
      |> onChangeFloat
    };
    {
      ...c,
      willReceiveProps: (_self) => value,
      initialState: () => value,
      reducer: (action, _state) =>
        switch action {
        | Change(value) => ReasonReact.Update(value)
        },
      render: (self) =>
        <ReactNative.TextInput
          keyboardType=`numeric
          value=self.state
          selectTextOnFocus
          onChangeText=(self.reduce((text) => Change(text)))
          onBlur=(self.handle(handleBlur))
          style=(applyStyle(TextInput.styles##input, style))
        />
    }
  };
};

module Picker = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "picker": style([borderBottomWidth(2.), borderBottomColor("#EBEBEB"), padding(0.)]),
          "item": style([fontFamily("LFTEtica")])
        }
      )
    );
  let c = ReasonReact.statelessComponent("Form.Picker");
  let make = (~onValueChange, ~selectedValue, ~enabled=true, ~mode=`dropdown, children) => {
    ...c,
    render: (_self) =>
      <Picker onValueChange enabled selectedValue mode style=styles##picker itemStyle=styles##item>
        (children |> ReasonReact.arrayToElement)
      </Picker>
  };
};

/*
  ModalPicker renders a picker which looks like a TextInput field but renders a Modal overlay
  showing available optioons within a list according to the options specified.

  TODO: move the values into a SectionList type and render a SectionList of
  values here.
 */
module ModalPicker = {
  let styles =
    StyleSheet.create(
      Style.(
        {
          "input":
            style([
              fontFamily("LFTEtica"),
              fontSize(14.0),
              color("#528060"),
              borderBottomWidth(1.),
              borderBottomColor("#EBEBEB"),
              padding(0.),
              paddingBottom(6.),
              paddingTop(5.),
              paddingLeft(0.)
            ]),
          "modalHint":
            style([
              fontFamily("LFTEtica"),
              fontSize(14.0),
              color("#528060"),
              textAlign(`center),
              margin(15.0)
            ]),
          "list": style([flex(1.), flexDirection(`column)]),
          "listItem": style([padding(15.0), borderBottomWidth(1.0), borderBottomColor("#ebebeb")]),
          "listItemText": style([fontFamily("LFTEtica")])
        }
      )
    );
  type value('item) = {
    item: 'item,
    text: string
  };
  type state = bool;
  type action =
    | ShowModal
    | HideModal;
  let c = ReasonReact.reducerComponent("Form.ModalPicker");
  let make = (~selectedValue, ~onValueChange, ~values, ~textAlign=?, ~modalHint=?, _children) => {
    ...c,
    initialState: fun () => (false: state),
    reducer: (action, _state) =>
      switch action {
      | ShowModal => ReasonReact.Update(true)
      | HideModal => ReasonReact.Update(false)
      },
    render: (self) =>
      <View>
        <TouchableOpacity onPress=(self.reduce(() => ShowModal))>
          <Text
            value=selectedValue
            style=(
              switch textAlign {
              | Some(`right) =>
                StyleSheet.flatten([styles##input, Style.style([Style.textAlign(`right)])])
              | Some(`center) =>
                StyleSheet.flatten([styles##input, Style.style([Style.textAlign(`center)])])
              | _ => styles##input
              }
            )
          />
        </TouchableOpacity>
        <Modal visible=self.state>
          <ScrollView contentContainerStyle=styles##list>
            (
              switch modalHint {
              | Some(text) => <Text value=text style=styles##modalHint />
              | None => ReasonReact.nullElement
              }
            )
            (
              values
              |> List.map(
                   (value) =>
                     <TouchableOpacity
                       key=value.text
                       onPress=(
                         () => {
                           onValueChange(value.item);
                           self.reduce(() => HideModal, ())
                         }
                       )>
                       <View style=styles##listItem>
                         <Text value=value.text style=styles##listItemText />
                       </View>
                     </TouchableOpacity>
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </ScrollView>
        </Modal>
      </View>
  };
};

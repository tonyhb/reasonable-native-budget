open ReactNative;

let applyStyle = (style, extra) =>
  switch extra {
  | None => style
  | Some(s) => StyleSheet.flatten([style, s])
  };

let applyTextAlign = (style, textAlign) =>
  switch textAlign {
  | None => style
  | Some(`left) => style
  | Some(`right) => StyleSheet.flatten([style, Style.style([Style.textAlign(`right)])])
  | Some(`center) => StyleSheet.flatten([style, Style.style([Style.textAlign(`center)])])
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
  let make = (~value: string, ~onPress, ~textAlign=?, ~style=?, _children) => {
    ...c,
    render: (_self) =>
      <TouchableOpacity onPress>
        <Text style=(textAlign |> applyTextAlign(applyStyle(styles##text, style))) value />
      </TouchableOpacity>
  };
};

module PrimaryButton = {
  let c = ReasonReact.statelessComponent("Form.DesructiveButton");
  let make = (~value: string, ~onPress, ~textAlign=`left, _children) => {
    ...c,
    render: (_self) =>
      <Button value onPress textAlign style=Style.(style([fontFamily("LFTEtica-Bold")])) />
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
    render: (_self) =>
      <Text
        style=(textAlign |> applyTextAlign(styles##label))
        value=(Js.String.toUpperCase(value))
      />
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
  type state = string;
  type actions =
    | Change(string);
  let c = ReasonReact.reducerComponent("TextInput");
  /**
    TODO: Make autocomplete a prefix trie for speed
   */
  let make =
      (
        ~value: string,
        ~onChangeText=?,
        ~onEndEditing=?,
        ~textAlign=?,
        ~autoFocus=false,
        ~style=?,
        ~selectTextOnFocus=false,
        _children
      ) => {
    /* onEnd is a function that calls onEndEditing with the current value, if passed from the parent */
    let onEnd = (_evt, self) =>
      switch onEndEditing {
      | Some(f) => f(self.ReasonReact.state)
      | None => ()
      };
    {
      ...c,
      initialState: () => value,
      reducer: (action, _state) =>
        switch action {
        | Change(newValue) => ReasonReact.Update(newValue)
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
          value=self.state
          autoFocus
          selectTextOnFocus
          style=(style |> applyStyle(textAlign |> applyTextAlign(styles##input)))
        />
    }
  };
};

/*
 MoneyInput is a text input which stores the text representation of a float in
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
  let make =
      (~style=?, ~value, ~onChangeFloat, ~selectTextOnFocus=true, ~autoFocus=false, _children) => {
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
          autoFocus
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
  values here.  Also, we may want to render our own content here - for example, a left and
  right panel.
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
  let showModal = (_evt) => ShowModal;
  let hideModal = (_evt) => HideModal;
  let c = ReasonReact.reducerComponent("Form.ModalPicker");
  let make = (~selectedValue, ~onValueChange, ~values, ~textAlign=?, ~modalHint=?, _children) => {
    ...c,
    initialState: fun () => (false: state),
    reducer: (action, _state) =>
      switch action {
      | ShowModal => ReasonReact.Update(true)
      | HideModal => ReasonReact.Update(false)
      },
    render: ({state, reduce}) =>
      <View>
        <TouchableOpacity onPress=(reduce(showModal))>
          <Text value=selectedValue style=(textAlign |> applyTextAlign(styles##input)) />
        </TouchableOpacity>
        <Modal visible=state onRequestClose=(reduce(hideModal))>
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
                           reduce(hideModal, ())
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

module DatePicker = {
  type state = {renderIosPicker: bool};
  type action =
    | Show
    | Hide;
  let c = ReasonReact.reducerComponent("Form.DatePicker");
  let make = (~date, ~onChange, ~textAlign=?, _children) => {
    let handleClick = (_evt, self) =>
      switch Platform.os {
      | Platform.IOS => self.ReasonReact.reduce(() => Show, ())
      | Platform.Android =>
        DatePickerAndroid.open_(~date, ())
        |> Js.Promise.then_(
             fun
             | DatePickerAndroid.Set((resp: DatePickerAndroid.response)) => {
                 onChange(
                   Js.Date.makeWithYMD(
                     ~year=float_of_int(resp.year),
                     ~month=float_of_int(resp.month),
                     ~date=float_of_int(resp.day),
                     ()
                   )
                 );
                 Js.Promise.resolve()
               }
             | Dismissed => Js.Promise.resolve()
           )
        |> ignore
      };
    {
      ...c,
      initialState: () => {renderIosPicker: false},
      reducer: (action, _state) =>
        switch action {
        | Show => ReasonReact.Update({renderIosPicker: true})
        | Hide => ReasonReact.Update({renderIosPicker: false})
        },
      render: ({state, handle}) =>
        switch state.renderIosPicker {
        | false =>
          <TouchableOpacity onPress=(handle(handleClick))>
            <Text
              value=(DateFormat.long(date))
              style=(textAlign |> applyTextAlign(TextInput.styles##input))
            />
          </TouchableOpacity>
        | true => <DatePickerIOS date mode=`date onDateChange=onChange />
        }
    }
  };
};

/*
    The module interface for an autocomplete component.  `item` is the type of
    item we're autocomplete searching for.
 */
module type Autocompleter = {type item;};

/*
  AutocompleteMaker is a functor which generates an Autocomplete component for
  a given type
 */
module AutocompleteMaker = (Res: Autocompleter) => {
  type autocompleteItem = {
    item: Res.item,
    textValue: string /*** The text value used to match autocompletes */
  };
  type state = {
    value: string, /* The typed in value */
    autocomplete: option(autocompleteItem) /* The autocomplete value, if one is selected */
  };
  type actions =
    | Change(string);
  let change = (text) => Change(text);
  let c = ReasonReact.reducerComponent("Autocomplete");
  /**
    TODO: Make autocomplete a prefix trie for speed
   */
  let make =
      (
        ~value: string,
        ~autocomplete: list(autocompleteItem),
        ~onChangeText=?,
        ~onEndEditing=?,
        ~textAlign=?,
        ~style=?,
        ~selectTextOnFocus=false,
        _children
      ) => {
    /**
      onEnd is a function that calls onEndEditing from props with the current text value
      of the input box and, if available, the autocomplete item used to fill the entry.
    */
    let onEnd = (_evt, self) =>
      switch onEndEditing {
      | Some(f) =>
        switch self.ReasonReact.state.autocomplete {
        | Some(ac) => f(ac.textValue, Some(ac.item)) /* Use the autocomplete text value; we may have not typed in the full value */
        | None => f(self.ReasonReact.state.value, None)
        }
      | None => ()
      };
    {
      ...c,
      initialState: () => {value, autocomplete: None},
      reducer: (action, state) => {
        let newState =
          switch action {
          | Change(newValue) =>
            /*
              Find whether there's an autocomplete that satisfies our text.
              Only find a new autocomplete if we've added extra input to our value;
              if we're deleting characters we shouldn't attempt to autocomplete
             */
            if (Js.String.length(state.value) < Js.String.length(newValue)) {
              switch (
                autocomplete
                |> List.find(
                     (item: autocompleteItem) =>
                       item.textValue |> Js.String.indexOf(newValue |> Js.String.toLowerCase) == 0
                   )
              ) {
              | item => {value: newValue, autocomplete: Some(item)}
              | exception Not_found => {value: newValue, autocomplete: None}
              }
            } else {
              {value: newValue, autocomplete: None}
            }
          };
        ReasonReact.UpdateWithSideEffects(
          newState,
          (_self) =>
            switch onChangeText {
            | Some(f) => f(newState.value)
            | None => ()
            }
        )
      },
      render: ({reduce, state, handle}) =>
        <ReactNative.TextInput
          underlineColorAndroid="transparent"
          onChangeText=(reduce(change))
          onEndEditing=(handle(onEnd))
          value=(
            switch state.autocomplete {
            | Some(autocompleteItem) => autocompleteItem.textValue /* If we have autocomplete render it as the value */
            | None => state.value
            }
          )
          selection=(
            switch state.autocomplete {
            | Some(autocompleteItem) => {
                "start": state.value |> Js.String.length,
                "_end": autocompleteItem.textValue |> Js.String.length
              }
            | None => {
                "start": state.value |> Js.String.length,
                "_end": state.value |> Js.String.length
              }
            }
          )
          selectTextOnFocus
          style=(style |> applyStyle(textAlign |> applyTextAlign(TextInput.styles##input)))
        />
    }
  };
};

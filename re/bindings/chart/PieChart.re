type value = {
  value: float,
  label: option(string)
};

type valueJs = {. "value": float, "label": Js.Nullable.t(string)};

let value = (~label=?, value) => {value, label};

let values = (vals: array(value)) =>
  vals |> Array.map((v) => {"value": v.value, "label": Js.Nullable.from_opt(v.label)});

type animationJs = {. "durationX": int, "durationY": int, "easingX": int, "easingY": int};

type descriptionJs = {
  .
  "text": string, "textSize": Js.Nullable.t(float), "textColor": Js.Nullable.t(string)
};

let description = (~textSize=?, ~textColor=?, text) => {
  "text": text,
  "textSize": Js.Nullable.from_opt(textSize),
  "textColor": Js.Nullable.from_opt(textColor)
};

let defaultDescription = description("");

type pieChartDataConfigJs = {
  .
  "colors": array(string),
  "drawValues": bool,
  "drawLabels": bool,
  "highlightEnabled": bool,
  "valueTextSize": Js.Nullable.t(float),
  "valueTextColor": Js.Nullable.t(string),
  "visible": Js.boolean,
  "selectionShift": float, /* Pie only */
  "sliceSpace":
    float /* Pie only */
};

let config =
    (
      ~colors: array(string),
      ~drawValues=false,
      ~drawLabels=true,
      ~highlightEnabled=true,
      ~valueTextSize: option(float)=?,
      ~valueTextColor: option(string)=?,
      ~visible=true,
      ~sliceSpace=0.,
      ~selectionShift=0.,
      ()
    )
    : pieChartDataConfigJs => {
  "colors": colors |> Array.map((v) => BsReactNative.StyleSheet.processColor(`String(v))),
  "drawValues": drawValues,
  "drawLabels": drawLabels,
  "highlightEnabled": highlightEnabled,
  "valueTextSize": Js.Nullable.from_opt(valueTextSize),
  "valueTextColor": Js.Nullable.from_opt(valueTextColor),
  "visible": Js.Boolean.to_js_boolean(visible),
  "selectionShift": selectionShift,
  "sliceSpace": sliceSpace
};

type pieChartDataJs = {
  .
  "values": array(valueJs),
  "label": string, /*** Needs to be present for android, even if its empty **/
  "config": Js.Nullable.t(pieChartDataConfigJs)
};

let pieChartData =
    (~label: option(string), ~config: option(pieChartDataConfigJs), ~data: array(valueJs))
    : pieChartDataJs => {
  "values": data,
  "label":
    switch label {
    | Some(s) => s
    | _ => ""
    },
  "config": Js.Nullable.from_opt(config)
};

[@bs.module "react-native-charts-wrapper"] external pieChart : ReasonReact.reactClass = "PieChart";

let make =
    (
      ~data: array(value),
      ~label=?,
      ~config: option(pieChartDataConfigJs)=?,
      ~chartDescription: descriptionJs=defaultDescription,
      /* These props are added to the dataSet's config */
      /* These props are passed to the component */
      ~holeRadius=0,
      ~holeColor=?,
      ~transparentCircleRadius=0,
      ~transparentCircleColor=?,
      ~rotationEnabled=false,
      ~rotationAngle=0,
      ~drawSliceText=true,
      ~usePercentValues=false,
      ~style,
      children
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=pieChart,
    ~props={
      "data": {"dataSets": [|pieChartData(~label, ~config, ~data=values(data))|]},
      "style": style,
      "holeRadius": holeRadius,
      "holeColor": Js.Nullable.from_opt(holeColor),
      "transparentCircleRadius": transparentCircleRadius,
      "transparentCircleColor": Js.Nullable.from_opt(transparentCircleColor),
      "rotationEnabled": Js.Boolean.to_js_boolean(rotationEnabled),
      "rotationAngle": rotationAngle,
      "drawSliceText": Js.Boolean.to_js_boolean(drawSliceText),
      "usePercentValues": Js.Boolean.to_js_boolean(usePercentValues),
      "chartDescription": chartDescription,
      "legend": {"enabled": Js.Boolean.to_js_boolean(false)},
      "animation": {
        /* TODO */
        "durationX": 300,
        "durationY": 300,
        "easingX": 50,
        "easingY": 50
      }
      /***
         VALUES THAT BREAK IOS
         chartDescription
         drawEntryLabels
       **/
      /*"drawEntryLabels": Js.Boolean.to_js_boolean(drawEntryLabels): TODO: BREAKS iOS */
    },
    children
  );

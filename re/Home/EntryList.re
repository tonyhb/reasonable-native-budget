open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([backgroundColor("#fff"), flex(1.)]),
        "itemWrapper":
          style([
            flexDirection(`row),
            height(44.),
            paddingTop(2.),
            paddingLeft(15.),
            paddingRight(15.),
            alignItems(`center),
            backgroundColor("#fff"),
            marginTop((-1.0))
          ]),
        "left": style([flex(1.), flexDirection(`column), justifyContent(`center)]),
        "recipient": style([fontFamily("LFTEtica-Bold"), color("#3D4840")]),
        "category": style([fontFamily("LFTEtica"), fontSize(12.), opacity(0.6)]),
        "amountWrapper":
          style([flexDirection(`row), flex(1.), justifyContent(`flexEnd), alignItems(`flexStart)]),
        "amount":
          style([fontFamily("LFTEtica-Bold"), fontSize(14.), textAlign(`right), color("#528060")]),
        "cent":
          style([
            fontFamily("LFTEtica-Bold"),
            color("#528060"),
            opacity(0.65),
            fontSize(9.),
            textAlign(`right),
            marginLeft(2.),
            marginTop(1.)
          ]),
        "dateSeparator":
          style([
            fontFamily("LFTEtica"),
            color("#A1A6A6"),
            fontSize(10.),
            marginTop(15.),
            marginBottom(5.),
            letterSpacing(1.)
          ]),
        "debit": style([color("#C92A01")]),
        "credit": style([color("#528060")])
      }
    )
  );

module IncomeItem = {
  let c = ReasonReact.statelessComponent("EntryList.DisplayItem");
  let make = (~entry: Entry.t, ~income: Income.t, _children) => {
    let centAmount = Printf.sprintf("%.0f", mod_float(entry.amount, 1.) *. 100.);
    {
      ...c,
      render: (_self) =>
        <View style=styles##itemWrapper>
          <View style=styles##left>
            <Text
              value=(
                switch income.payee {
                | Some(r) => r.name
                | None => "-"
                }
              )
              style=styles##recipient
            />
            <Text
              value=(
                switch entry.category {
                | Some(c) => c.name
                | None => "(uncategorized)"
                }
              )
              style=styles##category
            />
          </View>
          <View style=styles##amountWrapper>
            <Text
              value=("$" ++ Printf.sprintf("%.0f", floor(entry.amount)))
              style=styles##amount
            />
            <Text value=(centAmount == "0" ? "00" : centAmount) style=styles##cent />
          </View>
        </View>
    }
  };
};

module ExpenseItem = {
  let c = ReasonReact.statelessComponent("EntryList.DisplayItem");
  let make = (~entry: Entry.t, ~expense: Expense.t, _children) => {
    let centAmount = Printf.sprintf("%.0f", mod_float(entry.amount, 1.) *. 100.);
    {
      ...c,
      render: (_self) =>
        <View style=styles##itemWrapper>
          <View style=styles##left>
            <Text
              value=(
                switch expense.recipient {
                | Some(r) => r.name
                | None => "-"
                }
              )
              style=styles##recipient
            />
            <Text
              value=(
                switch entry.category {
                | Some(c) => c.name
                | None => "(uncategorized)"
                }
              )
              style=styles##category
            />
          </View>
          <View style=styles##amountWrapper>
            <Text
              value=("$" ++ Printf.sprintf("%.0f", floor(entry.amount)))
              style=(StyleSheet.flatten([styles##amount, styles##debit]))
            />
            <Text
              value=(centAmount == "0" ? "00" : centAmount)
              style=(StyleSheet.flatten([styles##cent, styles##debit]))
            />
          </View>
        </View>
    }
  };
};

let c = ReasonReact.statelessComponent("EntryList");

let make = (~entries, _children) => {
  let list =
    switch entries {
    | [] => <Text value="No entries" />
    | _ =>
      let currentEntry = ref(entries |> List.hd);
      entries
      |> List.mapi(
           (i, entry: Entry.t) => {
             let view =
               switch entry.entryType {
               | Expense(e) => <ExpenseItem entry expense=e key=entry.id />
               | Income(i) => <IncomeItem entry income=i key=entry.id />
               | Transfer(_t) => ReasonReact.nullElement
               };
             if (i == 0 || DateFormat.isDifferentDay(entry.date, currentEntry^.Entry.date)) {
               currentEntry := entry;
               <View key=entry.id>
                 <Text
                   value=(DateFormat.dayMonth(entry.date |> Js.Date.fromFloat) |> String.uppercase)
                   style=styles##dateSeparator
                 />
                 view
               </View>
             } else {
               currentEntry := entry;
               view
             }
           }
         )
      |> Array.of_list
      |> ReasonReact.arrayToElement
    };
  {...c, render: (_self) => <View style=styles##wrapper> list </View>}
};

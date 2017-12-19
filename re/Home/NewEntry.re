open BsReactNative;

type state = {entry: Entry.t};

type action =
  | UpdateAmount(float)
  | UpdateDate(float)
  | UpdateAccount(Account.t)
  | UpdateCategory(Category.t)
  | UpdateDescription(string)
  | UpdateRecipient(Recipient.t);

let updateRecipient = (recip) => UpdateRecipient(recip);

let updateDate = (date: Js.Date.t) => UpdateDate(date |> Js.Date.valueOf);

let c = ReasonReact.reducerComponent("NewEntry");

module RecipientAutocomplete =
  Form.AutocompleteMaker(
    {
      type item = Recipient.t;
    }
  );

let recipientList = (recipients) => 
  recipients
  |> List.map(fun (r) => ({textValue: r.name, item: r}: RecipientAutocomplete.autocompleteItem));

let make = (~budget, ~updateBudget, ~nav, _children) => {
  ...c,
  initialState: () => {entry: Budget.entry(budget)},
  reducer: (action, state) =>
    switch action {
    | UpdateAmount(amount) => ReasonReact.Update({entry: {...state.entry, amount}})
    | UpdateAccount(account) => ReasonReact.Update({entry: {...state.entry, source: account}})
    | UpdateCategory(cat) => ReasonReact.Update({entry: {...state.entry, category: Some(cat)}})
    | UpdateDate(date) => ReasonReact.Update({entry: {...state.entry, date}})
    | UpdateDescription(desc) =>
      ReasonReact.Update({entry: {...state.entry, description: Some(desc)}})
    | UpdateRecipient(r) =>
      ReasonReact.Update({
        entry: {
          ...state.entry,
          category: r.defaultCategory,
          entryType: Entry.Expense({recipient: Some(r), expensable: false, tags: []})
        }
      })
    },
  render: (self) =>
    <View style=Style.(style([paddingTop(30.), padding(15.)]))>
      <Form.Field>
        <Form.Label value="Amount" textAlign=`center />
        <Form.MoneyInput
          onChangeFloat=(self.reduce((value) => UpdateAmount(value)))
          value=("$" ++ Printf.sprintf("%.2f", self.state.entry.amount))
          selectTextOnFocus=true
          autoFocus=true
          style=Style.(style([fontFamily("LFTEticaDisplayHv"), fontSize(30.), textAlign(`center)]))
        />
      </Form.Field>
      <View style=Style.(style([flexDirection(`row)]))>
        <View style=Style.(style([flex(1.)]))>
          <Form.Field>
            <Form.Label value="From your" textAlign=`center />
            <Form.ModalPicker
              textAlign=`center
              selectedValue=self.state.entry.source.Account.name
              values=(
                budget.Budget.accounts
                |> Array.map(
                     fun (acct: Account.t) => (
                       {text: acct.name, item: acct}: Form.ModalPicker.value(Account.t)
                     )
                   )
                |> Array.to_list
              )
              onValueChange=((acct) => self.reduce(() => UpdateAccount(acct), ()))
              modalHint="Select the account for this entry"
            />
          </Form.Field>
        </View>
        <View style=Style.(style([flex(1.)]))>
          <Form.Field>
            <Form.Label value="Place or name" textAlign=`center />
            <RecipientAutocomplete
              value=""
              autocomplete=(budget.Budget.recipients |> recipientList)
              textAlign=`center
              onEndEditing=(
                (text, item) =>
                  switch item {
                  | Some(recipient) => self.reduce(() => updateRecipient(recipient), ())
                  /* This may have been caused by a backspace in autocomplete, which means an item wouldn't
                     match despite existing.  newRecipientByName attempts to use an existing recipient if
                     possible */
                  | _ => self.reduce(() => UpdateRecipient(text |> Recipient.newRecipientByName(budget.recipients)), ())
                  }
              )
            />
          </Form.Field>
        </View>
      </View>
      <View style=Style.(style([flexDirection(`row)]))>
        <View style=Style.(style([flex(1.)]))>
          <Form.Field>
            <Form.Label value="On" textAlign=`center />
            <Form.DatePicker
              date=(Js.Date.fromFloat(self.state.entry.date))
              onChange=(self.reduce(updateDate))
              textAlign=`center
            />
          </Form.Field>
        </View>
        <View style=Style.(style([flex(1.)]))>
          <Form.Field>
            <Form.Label value="In category" textAlign=`center />
            <Form.ModalPicker
              textAlign=`center
              modalHint="Choose a category for this expense"
              selectedValue=(
                switch self.state.entry.category {
                | Some(cat) => cat.Category.name
                | None => "(uncategorized)"
                }
              )
              onValueChange=((cat) => self.reduce(() => UpdateCategory(cat), ()))
              values=(
                Group.categories(budget.Budget.budget)
                |> Array.map(
                     fun (cat) => (
                       {text: cat.Category.name, item: cat}: Form.ModalPicker.value(Category.t)
                     )
                   )
                |> Array.to_list
              )
            />
          </Form.Field>
        </View>
      </View>
      <Form.Field>
        <Form.Label value="Any notes?" textAlign=`center />
        <Form.TextInput
          value=""
          onChangeText=((desc) => self.reduce(() => UpdateDescription(desc), ()))
          style=Style.(style([textAlign(`center)]))
        />
      </Form.Field>
      <Form.PrimaryButton
        value="Save"
        textAlign=`center
        onPress=(
          () =>
            updateBudget(
              ~budget=Budget.addEntry(budget, self.state.entry),
              ~sideEffect=Some(() => ReactNavigation.Navigation.navigate(nav, ~routeName="/", ()))
            )
        )
      />
    </View>
};

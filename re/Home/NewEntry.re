open ReactNative;

type state = {entry: Types.entry};

type action =
  | UpdateAmount(float)
  | UpdateDate(int)
  | UpdateAccount(Account.t)
  | UpdateCategory(Category.t)
  | UpdateDescription(string)
  | UpdateRecipientName(string);

let c = ReasonReact.reducerComponent("NewEntry");

                let recipient: Recipient.t = {id: "", name: "trader joes", defaultCategory: None};

let make = (~budget, ~push, _children) => {
  ...c,
  initialState: () => {
    entry: {
      id: Uuid.gen(),
      date: 0.,
      description: None,
      amount: 0.,
      source: budget.Budget.accounts[0],
      currency: budget.Budget.settings.defaultCurrency,
      category: None,
      entryType: Types.Expense({recipient: None, expensable: false, tags: []}),
      createdAt: 0.,
      updatedAt: 0.
    }
  },
  reducer: (action, state) =>
    switch action {
    | UpdateAmount(amount) => ReasonReact.Update({entry: {...state.entry, amount}})
    | UpdateAccount(account) => ReasonReact.Update({entry: {...state.entry, source: account}})
    | UpdateCategory(cat) => ReasonReact.Update({entry: {...state.entry, category: Some(cat)}})
    | UpdateDescription(desc) =>
      ReasonReact.Update({entry: {...state.entry, description: Some(desc)}})
    | _ => ReasonReact.NoUpdate
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
      <View style=Style.(style([flexDirection(`row)]))>
        <View style=Style.(style([flex(1.)]))>
          <Form.Field>
            <Form.Label value="Place or name" textAlign=`center />
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
      <Form.PrimaryButton value="Save" onPress=(() => push("/", Js.Obj.empty())) />
    </View>
};

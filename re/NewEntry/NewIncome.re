open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([flex(1.), padding(15.), paddingTop(30.)]),
        "amount": style([fontFamily("LFTEticaDisplayHv"), fontSize(30.), textAlign(`center)])
      }
    )
  );

type state = {
  amount: float,
  date: float,
  category: option(Category.t),
  account: Account.t
};

type actions =
  | UpdateAmount(float)
  | UpdateCategory(option(Category.t))
  | UpdateDate(float)
  | UpdateAccount(Account.t);

let updateDate = (date: Js.Date.t) => UpdateDate(date |> Js.Date.valueOf);

let c = ReasonReact.reducerComponent("NewIncome");

let make = (~budget: Budget.t, ~nav, ~updateBudget, _children) => {
  ...c,
  initialState: () => {
    amount: 0.,
    date: Js.Date.now(),
    category: None,
    account: budget.accounts[0]
  },
  reducer: (action, state) =>
    switch action {
    | UpdateAmount(a) => ReasonReact.Update({...state, amount: a})
    | UpdateAccount(a) => ReasonReact.Update({...state, account: a})
    | UpdateDate(d) => ReasonReact.Update({...state, date: d})
    | UpdateCategory(c) => ReasonReact.Update({...state, category: c})
    },
  render: (self) =>
    <View style=styles##wrapper>
      <Form.Field>
        <Form.Label value="On" textAlign=`center />
        <Form.DatePicker
          date=(Js.Date.fromFloat(self.state.date))
          onChange=(self.reduce(updateDate))
          textAlign=`center
        />
      </Form.Field>
      <Form.Field>
        <Form.Label value="Amount" textAlign=`center />
        <Form.MoneyInput
          value=("$" ++ Printf.sprintf("%.2f", self.state.amount))
          selectTextOnFocus=true
          autoFocus=true
          onChangeFloat=(self.reduce((amt) => UpdateAmount(amt)))
          style=styles##amount
        />
      </Form.Field>
      <Form.Field>
        <Form.Label value="Into account" textAlign=`center />
        <Form.ModalPicker
          textAlign=`center
          selectedValue=self.state.account.Account.name
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
          modalHint="Select the account for this income/deposit"
        />
      </Form.Field>

      <Form.PrimaryButton
        value="Save"
        textAlign=`center
        onPress=(
          () =>
            updateBudget(
              ~budget=Budget.addEntry(budget, Entry.entry(
                ~amount=self.state.amount,
                ~date=self.state.date,
                ~source=self.state.account,
                ~category=self.state.category,
                ~currency=budget.settings.defaultCurrency,
                ~description=None,
                ~entry=Entry.Income({payee: None})
              )),
              ~sideEffect=Some(() => ReactNavigation.Navigation.navigate(nav, ~routeName="/", ()))
            )
        )
      />
    </View>
};

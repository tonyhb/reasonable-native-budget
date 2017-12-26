open BsReactNative;

let styles = StyleSheet.create(Style.({
  "wrapper": style([flex(1.), backgroundColor("#fff")])
}));

type entryTabTypes =
  | Expense
  | Income
  | Transfer;

let c = ReasonReact.reducerComponent("NewEntry");

let make = (~budget: Budget.t, ~updateBudget, ~nav, _children) => {
  ...c,
  initialState: () => Expense,
  reducer: (action, _state) =>
    switch action {
    | Expense => ReasonReact.Update(Expense)
    | Income => ReasonReact.Update(Income)
    | Transfer => ReasonReact.Update(Transfer)
    },
  render: (self) =>
    <Wrapper value="Add an entry" showTopBar=true showBackButton=true nav>
      <View style=styles##wrapper key="addEntry">
        <Tabs>
          <Tabs.Tab
            value="Expense"
            onPress=(self.reduce(() => Expense))
            isActive=(self.state == Expense)
            key="expense"
          />
          <Tabs.Tab
            value="Transfer"
            onPress=(self.reduce(() => Transfer))
            isActive=(self.state == Transfer)
            key="transfer"
          />
          <Tabs.Tab
            value="Income"
            onPress=(self.reduce(() => Income))
            isActive=(self.state == Income)
            key="income"
          />
        </Tabs>
        (
          switch self.state {
          | Expense => <NewExpense budget updateBudget nav />
          | Transfer => <Text value="Transfer" />
          | Income => <NewIncome budget updateBudget nav />
          }
        )
      </View>
    </Wrapper>
};

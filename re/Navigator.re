let c = ReasonReact.statelessComponent("Navigator");

/* If hasBudget is false we need to onboard the user */
let make = (~budget: Budget.t, ~hasBudget: bool, ~updateBudget, _children) => {
	...c,
	render: (_self) =>
		<RRNavigation.NativeRouter>
				/* The initial welcome/loading screen.  This shows a loading indicator
					 when attempting to load the budget from async storage, and if we're
					 done loading will either navigate to "Home" or show a button to get started */
							<RRNavigation.Navigation hideNavBar=true>
								<RRNavigation.Card
									exact=true
									path="/"
									render=(({history}) => <Welcome hasBudget budget push=history.push />)
								/>
								<RRNavigation.Card
									exact=true
									path="/onboarding/accounts"
									render=(
										({history}) => <OnboardingAccounts push=history.push budget updateBudget />
									)
								/>
								<RRNavigation.Card
									exact=true
									path="/onboarding/budget"
									render=(
										({history, location}) =>
											<OnboardingBudget
												push=history.push
												budget=location.state##budget
												updateBudget
											/>
									)
								/>
								<RRNavigation.Card
									exact=true
									path="/home"
									render=(({history}) => <Home budget push=history.push />)
								/>
						</RRNavigation.Navigation>
		</RRNavigation.NativeRouter>
};

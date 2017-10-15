open ReactNative;

let app () =>
	<View style=Style.(style [flex 1., flexDirection `row, justifyContent `center, alignItems `center])>
		<Root />
	</View>;

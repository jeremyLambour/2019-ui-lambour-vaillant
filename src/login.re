type action =
  | Login
  | Register
  | UpdateEmailField(string)
  | UpdatePasswordField(string);

type state = {
  email: string,
  password: string,
};

let component = ReasonReact.reducerComponent("Login");

let make = _children => {
  ...component,
  initialState: () => {email: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Login =>
      ReasonReact.Update({email: state.email, password: state.password})
    | Register =>
      ReasonReact.Update({email: state.email, password: state.password})
    | UpdateEmailField(email) => ReasonReact.Update({...state, email})
    | UpdatePasswordField(password) =>
      ReasonReact.Update({...state, password})
    },
  render: self =>
    <div>
      <h1> {ReasonReact.string("Register")} </h1>
      <form>
        <input
          type_="text"
          value={self.state.email}
          placeholder="Email"
          onChange={event =>
            self.send(
              UpdateEmailField(ReactEvent.Form.target(event)##value),
            )
          }
        />
        <input
          type_="password"
          value={self.state.password}
          onChange={event =>
            self.send(
              UpdatePasswordField(ReactEvent.Form.target(event)##value),
            )
          }
          placeholder="password"
        />
        <button onClick={_ => self.send({Login})}>
          {ReasonReact.string("Login")}
        </button>
      </form>
      <div> {ReasonReact.string(self.state.email)} </div>
      <div> {ReasonReact.string(self.state.password)} </div>
    </div>,
};
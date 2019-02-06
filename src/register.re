type action =
  | Register
  | UpdateEmailField(string)
  | UpdatePasswordField(string);

type state = {
  email: string,
  password: string,
};

let component = ReasonReact.reducerComponent("Register");

let make = _children => {
  ...component,
  initialState: () => {email: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
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
        <button onSubmit={_ => self.send({Register})}>
          {ReasonReact.string("Register")}
        </button>
      </form>
      <div> {ReasonReact.string(self.state.email)} </div>
      <div> {ReasonReact.string(self.state.password)} </div>
    </div>,
};
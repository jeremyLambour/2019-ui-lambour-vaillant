type state = {
  email: string,
  password: string,
};
type action =
  | Login
  | UpdateEmailField(string)
  | UpdatePasswordField(string)
  | LoggedIn(state)
  | NotLoggedIn;

let login = user => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "email", Js.Json.string(user.email));
  Js.Dict.set(payload, "password", Js.Json.string(user.password));
  Js.Promise.(
    Fetch.fetchWithInit(
      "http://localhost:8080/api/v1/users/login",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
    |> then_(json =>
         Json.Decode.{email: field("email", string, json), password: ""}
         |> (users => Some(users) |> resolve)
       )
  );
};
let component = ReasonReact.reducerComponent("Login");

let make = _children => {
  ...component,
  initialState: () => {email: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Login =>
      ReasonReact.UpdateWithSideEffects(
        state,
        self =>
          Js.Promise.(
            login(state)
            |> then_(result =>
                 switch (result) {
                 | Some(user) => resolve(self.send(LoggedIn(user)))
                 | None => resolve(self.send(NotLoggedIn))
                 }
               )
            |> ignore
          ),
      )
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
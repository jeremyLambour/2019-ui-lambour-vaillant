type page =
  | Login
  | Register;

module type Mapper = {
  let toPage: ReasonReact.Router.url => page;
  let toUrl: page => string;
};

module Mapper: Mapper = {
  let toPage = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | ["register"] => Register
    | ["login"] => Login
    | _ => Login
    };
  let toUrl = page =>
    switch (page) {
    | Register => "register"
    | _ => "login"
    };
};
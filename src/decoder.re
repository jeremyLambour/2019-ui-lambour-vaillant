open SessionUser;
type userResponse = {
  id: string,
  firstName: string,
  lastName: string,
  email: string,
  createdAt: string,
  updatedAt: string,
  deletedAt: option(string),
};

type response = {
  success: bool,
  token: string,
  profile: userResponse,
  message: string,
};

let decodeProfile = json =>
  Json.Decode.{
    id: json |> field("id", string),
    firstName: json |> field("firstName", string),
    lastName: json |> field("lastName", string),
    email: json |> field("email", string),
    createdAt: json |> field("createdAt", string),
    updatedAt: json |> field("updatedAt", string),
    deletedAt: json |> optional(field("deletedAt", string)),
  };
let decodeResponse = json => {
  let user = Json.Decode.{
    success: json |> field("success", bool),
    token: json |> field("token", string),
    profile: json |> field("profile", decodeProfile),
    message: json |> field("message", string),
  };
  SessionUser.saveUser(user.token)
  }
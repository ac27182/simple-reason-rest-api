open Express;
type myJson = {
  name: string,
  age: int,
};

type jobDetails0 = {
  jobTitle: string,
  country: string,
  company: string,
  remote: bool,
  postingDate: int,
  closingDate: int,
  salaryLowerBound: int,
  salaryUpperBound: int,
  skills: list(string),
  description: string,
};

// very clunky....
// canonical way to create json using dicts
let makeJson = () => {
  let a0: myJson = {name: "alex", age: 22};
  let j = Js.Dict.empty();
  Js.Dict.set(j, "name", Js.Json.string(a0.name));
  Js.Dict.set(j, "age", Js.Json.number(a0.age |> float_of_int));
  Js.Json.object_(j);
};

// wat of encoding json using bs-json
let makeJson0 = () => {
  let a0: myJson = {name: "alex", age: 22};
  let obj =
    Json.Encode.(
      object_([("name", string(a0.name)), ("age", int(a0.age))])
    );
  obj;
};

// Js_array.from;

let listToArray = (l: list(string)): array(string) =>
  List.fold_left(
    (acc: array(string), x: string): array(string) =>
      [|x|] |> (acc |> Array.append),
    Array.make(0, ""),
    l,
  );

// generating job details
let jobDetailsEncoder = (j: jobDetails0): Js.Json.t =>
  Json.Encode.(
    object_([
      ("jobTitle", j.jobTitle |> string),
      ("country", j.country |> string),
      ("company", j.company |> string),
      ("remote", j.remote |> bool),
      ("postingDate", j.postingDate |> int),
      ("closingDate", j.closingDate |> int),
      ("salaryLowerBound", j.salaryLowerBound |> int),
      ("salaryUpperBound", j.salaryUpperBound |> int),
      ("skills", j.skills |> listToArray |> stringArray),
      ("description", j.description |> string),
    ])
  );

let listener = (e: Js.null_undefined(Js.Exn.t)): unit =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ ">server operational"
  };

let details: jobDetails0 = {
  jobTitle: "software engineer",
  country: "united kingdom",
  company: "tianlong capital",
  remote: true,
  postingDate: Js.Date.now() |> int_of_float,
  closingDate: Js.Date.now() |> int_of_float,
  salaryLowerBound: 30000,
  salaryUpperBound: 40000,
  skills: ["scala", "javascript", "kafka", "spark", "react"],
  description: "tianlong capital is hiring software engineers",
};

let f0 = Middleware.from((_, _) => Response.sendString("hello browser"));
let f1 = Middleware.from((_, _) => Response.sendJson(() |> makeJson0));
let f2 =
  Middleware.from((_, _) =>
    details |> jobDetailsEncoder |> Response.sendJson
  );

let app: Express.App.t = App.make();
let p: int = 8080;

App.get(app, ~path="/helloWorld", f0);
App.get(app, ~path="/getJson", f1);
App.get(app, ~path="/getJobDetails", f2);

App.listen(app, ~port=p, ~onListen=listener, ());
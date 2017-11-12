let name = "IRA";

type t = {taxType: [ | `Traditional | `Roth]};

let default = {taxType: `Traditional};

module JSON = {
  let marshal = (item) =>
    Json.Encode.(
      object_([
        (
          "taxType",
          string(
            switch item.taxType {
            | `Traditional => "Traditional"
            | `Roth => "Roth"
            }
          )
        )
      ])
    );
  let unmarshal = (json) =>
    Json.Decode.{
      taxType:
        switch (json |> field("taxType", string)) {
        | "Traditional" => `Traditional
        | "Roth" => `Roth
        | _ => failwith("Expected Roth or Traditional taxType")
        }
    };
};

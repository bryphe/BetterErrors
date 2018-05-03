/* Batteries library substitutes */
let listDrop = (n, lst) => {
  let lst = ref(lst);
  for (i in 1 to n) {
    lst := List.tl(lst^)
  };
  lst^
};

let listDropWhile = (f, lst) => {
  let lst = ref(lst);
  while (f(List.hd(lst^))) {
    lst := List.tl(lst^)
  };
  lst^
};

let listTake = (n, lst) => {
  let result = ref([]);
  let lst = ref(lst);
  for (i in 1 to n) {
    result := [List.hd(lst^), ...result^];
    lst := List.tl(lst^)
  };
  List.rev(result^)
};

let listTakeWhile = (f, lst) => {
  let result = ref([]);
  let lst = ref(lst);
  while (f(List.hd(lst^))) {
    result := [List.hd(lst^), ...result^];
    lst := List.tl(lst^)
  };
  List.rev(result^)
};

let optionGet = (a) =>
  switch a {
  | Some(n) => n
  | None => raise(Invalid_argument("optionGet"))
  };

let optionMap = (f, a) =>
  switch a {
  | Some(a') => Some(f(a'))
  | None => None
  };

let listFilterMap = (f, lst) =>
  List.map(f, lst)
  |> List.filter(
       fun
       | Some(a) => true
       | None => false
     )
  |> List.map(optionGet);

let listFindMap = (f, lst) =>
  lst
  |> List.find(
       (a) =>
         switch (f(a)) {
         | Some(x) => true
         | None => false
         }
     )
  |> f
  |> optionGet;

let stringSlice = (~first=0, ~last=?, str) => {
  let last =
    switch last {
    | Some(l) => min(l, String.length(str))
    | None => String.length(str)
    };
  if (last <= first) {
    ""
  } else {
    String.sub(str, first, last - first)
  }
};

let stringFind = (str, part) => {
  let rec find' = (str, part, idx) =>
    if (String.length(str) < String.length(part)) {
      raise(Not_found)
    } else if (stringSlice(str, ~last=String.length(part)) == part) {
      idx
    } else {
      find'(stringSlice(str, ~first=1), part, idx + 1)
    };
  find'(str, part, 0)
};

let stringNsplit = (str, ~by) =>
  if (String.length(str) == 0) {
    raise(Invalid_argument("stringNSplit: empty str not allowed"))
  } else if (str == "") {
    []
  } else {
    let rec split' = (str, ~by, accum, curr) => {
      let lengthBy = String.length(by);
      let lengthStr = String.length(str);
      if (lengthStr < lengthBy) {
        [curr ++ str, ...accum]
      } else if (String.sub(str, 0, lengthBy) == by) {
        split'(String.sub(str, lengthBy, lengthStr - lengthBy), ~by, [curr, ...accum], "")
      } else {
        split'(String.sub(str, 1, lengthStr - 1), ~by, accum, curr ++ String.sub(str, 0, 1))
      }
    };
    split'(str, ~by, [], "") |> List.rev
  };

let stringSplit = (str, ~by) =>
  if (by == "") {
    ("", str)
  } else if (str == "") {
    raise(Not_found)
  } else {
    switch (stringNsplit(str, ~by)) {
    | []
    | [_] => raise(Not_found)
    | [x, ...xs] => (x, String.concat(by, xs))
    }
  };

let linesOfChannelExn = (chan) => {
  let lines = ref([]);
  try {
    while (true) {
      lines := [input_line(chan), ...lines^]
    };
    lines^
  } {
  | End_of_file =>
    close_in(chan);
    List.rev(lines^)
  }
};

let fileLinesOfExn = (filePath) => linesOfChannelExn(open_in(filePath));

/* ============ */
let get_match_n = (n, pat, str) => {
  let rex = Re_pcre.regexp(pat);
  Re_pcre.get_substring(Re_pcre.exec(~rex, str), n)
};

/* get the first (presumably only) match in a string */
let get_match = get_match_n(1);

let get_match_maybe = (pat, str) => {
  let rex = Re_pcre.regexp(pat);
  try (Some(Re_pcre.get_substring(Re_pcre.exec(~rex, str), 1))) {
  | Not_found => None
  }
};

let get_match_n_maybe = (n, pat, str) => {
  let rex = Re_pcre.regexp(pat);
  try (Some(Re_pcre.get_substring(Re_pcre.exec(~rex, str), n))) {
  | _ => None
  }
};

let execMaybe = (pat, str) => {
  let rex = Re_pcre.regexp(pat);
  try (Some(Re_pcre.exec(~rex, str))) {
  | Not_found => None
  }
};

let getSubstringMaybe = (result, n) =>
  try (Some(Re_pcre.get_substring(result, n))) {
  | Not_found => None
  };

let split = (sep, str) => {
  let rex = Re_pcre.regexp(sep);
  Re_pcre.split(~rex, str)
};

let sub = (sep, cb, str) => {
  let rex = Re_pcre.regexp(sep);
  Re_pcre.substitute(~rex, ~subst=cb, str);
};

let rec splitInto = (~chunckSize, l: list('a)) : list(list('a)) =>
  if (List.length(l) <= chunckSize || chunckSize == 0) {
    [l]
  } else {
    [listTake(chunckSize, l), ...splitInto(~chunckSize, listDrop(chunckSize, l))]
  };

let resetANSI = "\027[0m";

let bold = (s) => "\027[1m" ++ s ++ resetANSI;

let boldCode = (b) => b ? "\027[1m" : "";

let invert = s => "\027[7m" ++ s ++ resetANSI;

let invertCode = (b) => b ? "\027[7m" : "";

let dim = (s) => "\027[2m" ++ s ++ resetANSI;

let dimCode = (b) => b ? "\027[2m" : "";

let underlineCode = (u) => u ? "\027[4m" : "";

let normalCode = "\027[39m";

let redCode = "\027[31m";

let yellowCode = "\027[33m";

let blueCode = "\027[34m";

let greenCode = "\027[32m";

let purpleCode = "\027[35m";

let cyanCode = "\027[36m";

let underline = (s) => underlineCode(true) ++ s ++ resetANSI;

let normal = (~underline=false, ~invert=false, ~dim=false, ~bold=false, s) =>
  underlineCode(underline) ++ invertCode(invert) ++ dimCode(dim) ++ boldCode(bold) ++ normalCode ++ s ++ resetANSI;

let red = (~underline=false, ~invert=false, ~dim=false, ~bold=false, s) =>
  underlineCode(underline) ++ invertCode(invert) ++ dimCode(dim) ++ boldCode(bold) ++ redCode ++ s ++ resetANSI;

let yellow = (~underline=false, ~invert=false, ~dim=false, ~bold=false, s) =>
  underlineCode(underline) ++  invertCode(invert) ++ dimCode(dim) ++ boldCode(bold) ++ yellowCode ++ s ++ resetANSI;

let blue = (~underline=false, ~invert=false, ~dim=false, ~bold=false, s) =>
  underlineCode(underline) ++  invertCode(invert) ++ dimCode(dim) ++ boldCode(bold) ++ blueCode ++ s ++ resetANSI;

let green = (~underline=false, ~invert=false, ~dim=false, ~bold=false, s) =>
  underlineCode(underline) ++  invertCode(invert) ++ dimCode(dim) ++ boldCode(bold) ++ greenCode ++ s ++ resetANSI;

let cyan = (~underline=false, ~invert=false, ~dim=false, ~bold=false, s) =>
  underlineCode(underline) ++  invertCode(invert) ++ dimCode(dim) ++ boldCode(bold) ++ cyanCode ++ s ++ resetANSI;

let purple = (~underline=false, ~invert=false, ~dim=false, ~bold=false, s) =>
  underlineCode(underline) ++  invertCode(invert) ++ dimCode(dim) ++ boldCode(bold) ++ purpleCode ++ s ++ resetANSI;

let mapcat = (sep, f, l) => String.concat(sep, List.map(f, l));

let sp = Printf.sprintf;

let highlight = (~underline=false, ~invert=false, ~dim=false, ~bold=false, ~color=normal, ~first=0, ~last=99999, str) =>
  stringSlice(~last=first, str)
  ++ color(~underline, ~dim, ~invert, ~bold, stringSlice(~first, ~last, str))
  ++ stringSlice(~first=last, str);

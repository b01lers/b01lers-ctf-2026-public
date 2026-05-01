main(_) ->
    {ok, [A]} = io:fread("", "~d"),
    {ok, [B]} = io:fread("", "~d"),
    {ok, [C]} = io:fread("", "~d"),
    R = powmod(A, B, C),
    io:format("~w~n", [R]).

powmod(A, 1, C) -> A rem C;
powmod(A, B, C) -> (A * powmod(A, B - 1, C)) rem C.

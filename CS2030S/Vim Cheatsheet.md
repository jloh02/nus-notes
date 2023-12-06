> Format: `gg=G`

#### Important Settings
```
:set number
:set mouse=a
```
#### Basic Commands / Navigation

|Mode|Command|
|---|---|
|Normal/Cmd Mode|Default / Esc|
|Insert Mode|`i`|
|Visual Mode|`v`|
|Visual Block|`Ctrl + v`|
|Visual Line|`Shift + v`|
|Save curr / all files|`w`,`:wa`|
|Exit curr / all files|`:q`, `:qa`|
|Combi (save & exit all)|`:wqa`|
|With force|`:wqa!`|
|Suspend & Resume|`Ctrl+Z` & `fg`|

#### Common Navigation Commands \[In Cmd Mode\]

|Command|Description|
|---|---|
|`GG`, `g`|Top / bottom of file|
|`w`, `b`|Next / prev word|
|`/<regex>`|Pair with `N` & `n` to navigate occurrences of `<regex>`|
|`:<line no.>`|Navigate to `<line no.>`-th line|
|Others|`PgUp`, `PgDown`, `mouse`|

#### Common Edit Commands \[In Cmd Mode\]

|Command|Description|
|---|---|
|`u`, `Ctrl + r`|Undo / Redo|
|`y`, `yy`, `y$`|Yank (copy) selection, whole line, from cursor to end of line respectively|
|`d`, `dd`, `d$`|Cut selection, whole line, from cursor to end of line respectively|
|`p`|Paste|
|`dd p`|Swap lines|
|`d4w`|Delete 4 words|

#### Split screen / tabs
- (From CLI) `vim -O A.java B.java C.java` opens files in split screen (to navigate between, use `ctrl + w + <arrow>`)
- (From CLI) `vim -p A.java B.java C.java` opens files as tabs (to navigate btwn tabs use `gt` & `gT` in Cmd mode)
- (In Cmd mode) `:vsp X.java` to laterally place X.java to the left as split screen 
	- ↑ If you want this to splitright instead of left, do `:set splitright` before `:vsp X.java`
- (In Cmd mode) `:split X.java` to place X.java on top of the existing file

- (To change size of a subscreen) `Ctrl + w → +` or `-`
- (To equalize the sizes of the subscreens) `Ctrl + w → =`
- (To navigate between subscreens w/o mouse) `Ctrl + w → Arrow key`

#### Open a new terminal in split screen / new tab, instead of suspending + resuming session
- (In Cmd mode) `:vsp` → *New subscreen opens* → Navigate to new subscreen → `:terminal` → Go to new non-terminal subscreen & `:q` → You now have your code on one side and terminal on the other
- (To scroll through the terminal) `Ctrl + w → Shift + n;`
- (To return to insert mode in terminal) `i`
- (To exit the terminal) Type exit
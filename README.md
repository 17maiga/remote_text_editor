# Design ideas
## Client
- Client defines various commands. User can run commands one after the other by
  typing them into a prompt. The syntax is as follows:
  ```
  operand [arguments] [value]
  ```
  If the operand takes in arguments, these are specified before the value. A
  command does not always take in a value.
- Commands:
  - `block|b[number]`: starts `[number|1]` new text block(s). If the number of 
    blocks gets too high, it will ignore the remaining blocks.
  - `unblock|B[number]`: attempts to close `[number|1]` text blocks. If the 
    number of open blocks is lower, all blocks are closed.
  - `mode|m<u|l|n>`: enters whichever mode is specified (`u` = uppercase,
    `l` = lowercase, `n` = normal).
  - `indent|i[n]`: indents the text by `[n|2]` spaces, or as many spaces as 
    possible.
  - `unindent|I[n]`: unindents the text by `[n|2]` spaces, or as many spaces as
    possible.
  - `list|l`: starts a new bullet point level. All new lines will be treated
    as bullet points.
  - `unlist|L[n]`: exits `[n|1]` bullet point level.
  - `write|w <value>`: writes the value specified.
- Commands can be chained together until text is specified. Commands should be
  linked to their arguments by dashes and should be separated by spaces. They
  can be specified in their long or in their short form. There should always be
  a space between the last command (write) and the text. As an example, the 
  command `b-2 m-l i b w This is text` would produce the following output:
  ```
  +------------------------------------------------------------------------------+
  |+----------------------------------------------------------------------------+|
  ||  - this is text                                                            ||
  |+----------------------------------------------------------------------------+|
  +------------------------------------------------------------------------------+
  ```
  It is equivalent to the following long-form command:
  `block-2 mode-l indent list write This is text`
  Commands cannot be repeated multiple times in the same line; instead, they
  should be used with their parameters when appropriate.
- More examples:
  - 
  ```
  Command: 
  > b m-u w this is an uppercase block
  > B l m-l this is lowercase text outside of a block. It is too long to fit in a line, therefore it has been wrapped at the last space to fit.
  > block-3 mode-n unlist write This is yet another block. However, this has kept its cAsE, aS WE are In NORMAL mode.

  Output:
  +------------------------------------------------------------------------------+
  |THIS IS AN UPPERCASE BLOCK                                                    |
  +------------------------------------------------------------------------------+
  - this is lowercase text outside of a block. it is too long to fit in a line, 
    therefore it has been wrapped at the last space to fit.
  +------------------------------------------------------------------------------+
  |+----------------------------------------------------------------------------+|
  ||+--------------------------------------------------------------------------+||
  |||This is yet another block. However, this has kept its cAsE, aS WE are In  |||
  |||NORMAL mode.                                                              |||
  ||+--------------------------------------------------------------------------+||
  |+----------------------------------------------------------------------------+|
  +------------------------------------------------------------------------------+
  
  ---
  Command:
  > l l l w hi
  
  Output:
  - hi
  ```
  

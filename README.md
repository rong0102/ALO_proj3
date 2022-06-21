# ALO_proj3:Tomasulo
>demo影片: https://youtu.be/X-Mbc7GUTOs

測試input資料
```
ADDI F1, F2, 1
SUB F1, F3, F4
DIV F1, F2, F3
MUL F2, F3, F4
ADD F2, F4, F2
ADDI F4, F1, 2
MUL F5, F5, F5
ADD F1, F4, F4
```

output for cycle X (output要輸出所有**有變化**cycle)
Cycle: 1
```
     _ RF __
  F1 |   0 |
  F2 |   2 |
  F3 |   4 |
  F4 |   6 |
  F5 |   8 |
     -------

     _ RAT __
  F1 |  RS1 |
  F2 |      |
  F3 |      |
  F4 |      |
  F5 |      |
     --------

    _ RS _________________
RS1 |    + |    2 |    1 |
RS2 |      |      |      |
RS3 |      |      |      |
    ----------------------
BUFFER: empty

    ______________________
RS4 |      |      |      |
RS5 |      |      |      |
    ----------------------
BUFFER: empty
```

Cycle 6
```
     _ RF __
  F1 |   0 |
  F2 |   2 |
  F3 |   4 |
  F4 |   6 |
  F5 |   8 |
     -------

     _ RAT __
  F1 |  RS4 |
  F2 |  RS1 |
  F3 |      |
  F4 |  RS2 |
  F5 |      |
     --------

    _ RS _________________
RS1 |    + |    6 |  RS5 |
RS2 |    + |  RS4 |    2 |
RS3 |      |      |      |
    ----------------------
BUFFER: empty

    ______________________
RS4 |    / |    2 |    4 |
RS5 |    * |    4 |    6 |
    ----------------------
BUFFER: (RS4) 2 / 4
```

Cycle 45
```
     _ RF __
  F1 |   0 |
  F2 |   2 |
  F3 |   4 |
  F4 |   6 |
  F5 |   8 |
     -------

     _ RAT __
  F1 |  RS3 |
  F2 |  RS1 |
  F3 |      |
  F4 |  RS2 |
  F5 |  RS4 |
     --------

    _ RS _________________
RS1 |    + |    6 |  RS5 |
RS2 |    + |    0 |    2 |
RS3 |    + |  RS2 |  RS2 |
    ----------------------
BUFFER: (RS2) 0 + 2

    ______________________
RS4 |    * |    8 |    8 |
RS5 |    * |    4 |    6 |
    ----------------------
BUFFER: (RS5) 4 * 6
```

Cycle 63
```
     _ RF __
  F1 |   4 |
  F2 |  30 |
  F3 |   4 |
  F4 |   2 |
  F5 |  64 |
     -------

     _ RAT __
  F1 |      |
  F2 |      |
  F3 |      |
  F4 |      |
  F5 |      |
     --------

    _ RS _________________
RS1 |      |      |      |
RS2 |      |      |      |
RS3 |      |      |      |
    ----------------------
BUFFER: empty

    ______________________
RS4 |      |      |      |
RS5 |      |      |      |
    ----------------------
BUFFER: empty
```

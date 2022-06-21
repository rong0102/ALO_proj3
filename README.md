# ALO_proj3:Tomasulo
>demo影片: https://youtu.be/X-Mbc7GUTOs

## project cotent
  1. 給予測試assembly code
  2. output for cycle X (output要輸出所有有變化cycle)

## process explanation
1. load intstruction file into program(.txt)
2. use loop to run evey cycle. Include issue,dispatch, write result.
3.print out the status

## code explanation
### *define*
定義不同instruction的cycle數
```
#define add_cyc  2; //add/addi/sub為2
#define mul_cyc  5;
#define div_cyc  10;
```
Instruction structure
```
struct Instruction
{
	string operation;       //儲存哪種instruction: add/addi...
	int rd, rs1, rs2, imm;  //將資料F1字串取1儲存
};
```
RS structure 
```
struct RS {
	string operand,rs1,rs2; //考慮到會儲存RS+數字的形式，全使用string
};
```
ALU buffer structure
```
struct Buffer{
	int c,RS_num; //Wr.result cycle/ which RS
	bool occupy;  //RS? is occupy or not
};
```
`cyc_count = 1`current cycle
`bool change;`//check cycle change or not
`vector<int> RF = { 0, 0, 2, 4, 6, 8 };`
`vector<RS> reservation(5);`	//RS1-3 is add/sub; RS4-5 is mul/div
`bool checkRS[5] = { true,true,true,true,true };`//check RS is emty or not(true=empty;false=not empty)
`vector<string>RAT(6);`
```
Buffer Abuff;
Buffer Mbuff;
```
`vector<Instruction>Data;``

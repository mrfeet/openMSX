/************************************************************************/
/* This file contains all the opcode tables that will be used       	*/
/* to specicly emulate an R800                                      	*/
/*                                                                  	*/
/* The timing cycles are stored in different files for each CPU.    	*/
/* Also instructions with hardcoded timing information are stored   	*/
/* in these seperate files                                          	*/
/*                                                                  	*/
/*    R800 extensions originally produced 				*/
/*    1997 by Lex Lechz and modified by      			        */
/*    David Heremans                         			        */
/*                                                                      */
/*    Special thanks to Marcel de Kogel  for permission to modify       */
/*    the sources of his Z80-emulator for openMSX                       */
/*                                                                      */
/*                                                                      */
/*    FileContent: R800 extension for Marcel de Kogel's Z80-Emulator    */
/*                                                                      */
/*                                                                  	*/
/*                                                 David Heremans   	*/
/*                                                                  	*/
/************************************************************************/

//typedef void (*opcode_fn) (void);

/****************************************************************************/
/*    PREPROCESSOR MACROS                                                   */
/****************************************************************************/

// Multiply results:
// C   = changed
// Z   = changed
// P/V = 0
// S   = 0
// N   = unchanged
// H   = unchanged


// Macros:

// Word operations: 36 T-Cycles (R800)
#define M_MULUW(Reg1,Reg2)          \
  {                                 \
    unsigned long  ulResult;        \
    ulResult=(unsigned long)(R.Reg1.W.l)*R.Reg2.W.l; \
    R.DE.W.l=(word)(ulResult>>16);         \
    R.HL.W.l=(word)ulResult;               \
    R.AF.B.l&=(S_FLAG & V_FLAG);                     \
    if (ulResult) R.AF.B.l|=Z_FLAG; else R.AF.B.l&=~Z_FLAG; \
    if (ulResult & 0x80000000) R.AF.B.l|=C_FLAG; else R.AF.B.l&=~C_FLAG; \
  }

// Byte operations: 14 T-Cycles (R800)
#define M_MULU(r1,r2)               \
  {                                 \
    R.HL.W.l=((word)(r1))*(r2);       \
    R.AF.B.l&=(S_FLAG & V_FLAG);    \
    if (R.HL.W.l) R.AF.B.l|=Z_FLAG; else R.AF.B.l&=~Z_FLAG; \
    if (R.HL.W.l & 0x8000) R.AF.B.l|=C_FLAG; else R.AF.B.l&=~C_FLAG; \
  }

/****************************************************************************/
/*    OPCODE IMPLEMENTATION                                                 */
/****************************************************************************/

static void             muluw_hl_bc(void)     // ED C3
  {
  M_MULUW(HL,BC);
  }

static void             muluw_hl_sp(void)     // ED F3
  {
  M_MULUW(HL,SP);
  }

static void             mulu_a_b(void)        // ED C1
  {
  M_MULU(R.AF.B.h, R.BC.B.h);
  }

static void             mulu_a_c(void)        // ED C9
  {
  M_MULU(R.AF.B.h, R.BC.B.l);
  }

static void             mulu_a_d(void)        // ED D1
  {
  M_MULU(R.AF.B.h, R.DE.B.h);
  }

static void             mulu_a_e(void)        // ED D9
  {
  M_MULU(R.AF.B.h, R.DE.B.l);
  }

/****************************************************************************/

static unsigned R800_cycles_main[256]=
{
// changed already 00xxx110 (ld(hl),n;ld r,n)
// changed already 00xx0001 (ld ss,nn )
// changed already 00xx1001 (add hl,ss )
// changed already 00xxx100 (inc r ,inc (hl)  )
// changed already #00,#02,#0A,#12,#1A,#22,#2A,#32,#3A         
  1, 3, 2,1,1, 1,1,1,
  1, 1, 2,2,1, 1,1,1,
  2, 3, 2,1,1, 1,1,1,
  3, 1, 2,2,1, 1,1,1,
  2, 3, 5,1,1, 1,1,1,
  2, 1, 5,5,1, 1,1,1,
  2, 3, 4,1,4, 4,3,1,
  2, 1, 4,4,1, 1,1,1,
//R800 changed ; ld r,r ; ld r,(hl) ; ld (hl),r and halt
  1,1,1,1,1,1,2,1,
  1,1,1,1,1,1,2,1,
  1,1,1,1,1,1,2,1,
  1,1,1,1,1,1,2,1,
  1,1,1,1,1,1,2,1,
  1,1,1,1,1,1,2,1,
  2,2,2,2,2,2,1,2,
  1,1,1,1,1,1,2,1,
//R800 changed
  1,1,1,1,1,1,2,1, // ADD
  1,1,1,1,1,1,2,1, // ADC
  1,1,1,1,1,1,2,1, // SUB
  1,1,1,1,1,1,2,1, // SBC
  1,1,1,1,1,1,2,1, // AND
  1,1,1,1,1,1,2,1, // XOR
  1,1,1,1,1,1,2,1, // OR
  1,1,1,1,1,1,2,1, // CP
//R800 changed
  1, 4, 3, 3, 3, 4,2, 4,
  1, 1, 3, 0, 3, 3,2, 4,
  1, 4, 3, 3, 3, 4,2, 4,
  1, 1, 3, 3, 3, 0,2, 4,
  1, 4, 3, 5, 3, 4,2, 4,
  1, 1, 3, 1, 3, 0,2, 4,
  1, 4, 3, 2, 3, 4,2, 4,
  1, 1, 3, 1, 3, 0,2, 4
};

static unsigned R800_cycles_cb[256]=
{
//R800 changed
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,

//R800 changed bit test instructions
  2,2,2,2,2,2,3,2,
  2,2,2,2,2,2,3,2,
  2,2,2,2,2,2,3,2,
  2,2,2,2,2,2,3,2,
  2,2,2,2,2,2,3,2,
  2,2,2,2,2,2,3,2,
  2,2,2,2,2,2,3,2,
  2,2,2,2,2,2,3,2,

//R800 changed bit clear instructions
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,

//R800 changed bit set instructions
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2
};
static unsigned R800_cycles_xx_cb[]=
{
//R800 changed
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
//R800 changed bit test ix,iy
  5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,
//R800 changed bit clear ix,iy
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
//R800 changed bit set ix,iy
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0,
  0,0,0,0,0,0,7,0
};
static unsigned R800_cycles_xx[256]=
{
  0, 0,0,0,0,0,0,0,
  0, 2,0,0,0,0,0,0,
  0, 0,0,0,0,0,0,0,
  0, 2,0,0,0,0,0,0,
  0, 4, 6, 3,2,2,3,0,
  0, 2, 6, 2,2,2,3,0,   //dec iy=2 and inc yi=3???
  0, 0,0,0, 7, 7, 5,0,
  0, 2,0,0,0,0,0,0,
//R800 changed bit

  0,0,0,0,2,2, 5,0,
  0,0,0,0,2,2, 5,0,
  0,0,0,0,2,2, 5,0,
  0,0,0,0,2,2, 5,0,
  2,2,2,2,2,2,5,2,
  2,2,2,2,2,2,5,2,
  5,5,5,5,5,5,5,5,
  0,0,0,0,2,2,5,0,
//R800 changed bit

  0,0,0,0,2,2,5,0,
  0,0,0,0,2,2,5,0,
  0,0,0,0,2,2,5,0,
  0,0,0,0,2,2,5,0,
  0,0,0,0,2,2,5,0,
  0,0,0,0,2,2,5,0,
  0,0,0,0,2,2,5,0,
  0,0,0,0,2,2,5,0,
//R800 changed bit

  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,4,0,6,0,5,0,0,
  0,2,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,2,0,0,0,0,0,0
//R800 changed bit
};
static unsigned R800_cycles_ed[256]=
{
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
//R800 changed bit

   3, 3, 2, 6,2,5,3,2,
   3, 3, 2, 6,2,5,3,2, //bestaan ed 4c en ed 4e wel ??
   3, 3, 2, 6,2,5,3,2,
   3, 3, 2, 6,2,5,3,2,
   3, 3, 2, 6,2,5,3, 5,
   3, 3, 2, 6,2,5,3, 5,
   3, 3, 2, 6,2,5,3,2,
   3, 3, 2, 6,2,5,3,2,
//R800 changed bit

  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  4,4,4,4,0,0,0,0,
  4,4,4,4,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
//R800 changed bit

  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
//R800 changed bit
};

static opcode_fn R800_opcode_dd_cb[256]=
{
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rlc_xix  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rrc_xix  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rl_xix   ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rr_xix   ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,sla_xix  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,sra_xix  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,sll_xix  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,srl_xix  ,no_op_xx ,
 bit_0_xix,bit_0_xix,bit_0_xix,bit_0_xix,bit_0_xix,bit_0_xix,bit_0_xix,bit_0_xix,
 bit_1_xix,bit_1_xix,bit_1_xix,bit_1_xix,bit_1_xix,bit_1_xix,bit_1_xix,bit_1_xix,
 bit_2_xix,bit_2_xix,bit_2_xix,bit_2_xix,bit_2_xix,bit_2_xix,bit_2_xix,bit_2_xix,
 bit_3_xix,bit_3_xix,bit_3_xix,bit_3_xix,bit_3_xix,bit_3_xix,bit_3_xix,bit_3_xix,
 bit_4_xix,bit_4_xix,bit_4_xix,bit_4_xix,bit_4_xix,bit_4_xix,bit_4_xix,bit_4_xix,
 bit_5_xix,bit_5_xix,bit_5_xix,bit_5_xix,bit_5_xix,bit_5_xix,bit_5_xix,bit_5_xix,
 bit_6_xix,bit_6_xix,bit_6_xix,bit_6_xix,bit_6_xix,bit_6_xix,bit_6_xix,bit_6_xix,
 bit_7_xix,bit_7_xix,bit_7_xix,bit_7_xix,bit_7_xix,bit_7_xix,bit_7_xix,bit_7_xix,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_0_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_1_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_2_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_3_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_4_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_5_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_6_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_7_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_0_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_1_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_2_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_3_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_4_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_5_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_6_xix,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_7_xix,no_op_xx
};

static opcode_fn R800_opcode_fd_cb[256]=
{
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rlc_xiy  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rrc_xiy  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rl_xiy   ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,rr_xiy   ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,sla_xiy  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,sra_xiy  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,sll_xiy  ,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,srl_xiy  ,no_op_xx ,
 bit_0_xiy,bit_0_xiy,bit_0_xiy,bit_0_xiy,bit_0_xiy,bit_0_xiy,bit_0_xiy,bit_0_xiy,
 bit_1_xiy,bit_1_xiy,bit_1_xiy,bit_1_xiy,bit_1_xiy,bit_1_xiy,bit_1_xiy,bit_1_xiy,
 bit_2_xiy,bit_2_xiy,bit_2_xiy,bit_2_xiy,bit_2_xiy,bit_2_xiy,bit_2_xiy,bit_2_xiy,
 bit_3_xiy,bit_3_xiy,bit_3_xiy,bit_3_xiy,bit_3_xiy,bit_3_xiy,bit_3_xiy,bit_3_xiy,
 bit_4_xiy,bit_4_xiy,bit_4_xiy,bit_4_xiy,bit_4_xiy,bit_4_xiy,bit_4_xiy,bit_4_xiy,
 bit_5_xiy,bit_5_xiy,bit_5_xiy,bit_5_xiy,bit_5_xiy,bit_5_xiy,bit_5_xiy,bit_5_xiy,
 bit_6_xiy,bit_6_xiy,bit_6_xiy,bit_6_xiy,bit_6_xiy,bit_6_xiy,bit_6_xiy,bit_6_xiy,
 bit_7_xiy,bit_7_xiy,bit_7_xiy,bit_7_xiy,bit_7_xiy,bit_7_xiy,bit_7_xiy,bit_7_xiy,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_0_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_1_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_2_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_3_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_4_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_5_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_6_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,res_7_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_0_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_1_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_2_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_3_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_4_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_5_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_6_xiy,no_op_xx ,
 no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,no_op_xx ,set_7_xiy,no_op_xx
};

static opcode_fn R800_opcode_cb[256]=
{
 rlc_b  ,rlc_c  ,rlc_d  ,rlc_e  ,rlc_h  ,rlc_l  ,rlc_xhl  ,rlc_a  ,
 rrc_b  ,rrc_c  ,rrc_d  ,rrc_e  ,rrc_h  ,rrc_l  ,rrc_xhl  ,rrc_a  ,
 rl_b   ,rl_c   ,rl_d   ,rl_e   ,rl_h   ,rl_l   ,rl_xhl   ,rl_a   ,
 rr_b   ,rr_c   ,rr_d   ,rr_e   ,rr_h   ,rr_l   ,rr_xhl   ,rr_a   ,
 sla_b  ,sla_c  ,sla_d  ,sla_e  ,sla_h  ,sla_l  ,sla_xhl  ,sla_a  ,
 sra_b  ,sra_c  ,sra_d  ,sra_e  ,sra_h  ,sra_l  ,sra_xhl  ,sra_a  ,
 sll_b  ,sll_c  ,sll_d  ,sll_e  ,sll_h  ,sll_l  ,sll_xhl  ,sll_a  ,
 srl_b  ,srl_c  ,srl_d  ,srl_e  ,srl_h  ,srl_l  ,srl_xhl  ,srl_a  ,
 bit_0_b,bit_0_c,bit_0_d,bit_0_e,bit_0_h,bit_0_l,bit_0_xhl,bit_0_a,
 bit_1_b,bit_1_c,bit_1_d,bit_1_e,bit_1_h,bit_1_l,bit_1_xhl,bit_1_a,
 bit_2_b,bit_2_c,bit_2_d,bit_2_e,bit_2_h,bit_2_l,bit_2_xhl,bit_2_a,
 bit_3_b,bit_3_c,bit_3_d,bit_3_e,bit_3_h,bit_3_l,bit_3_xhl,bit_3_a,
 bit_4_b,bit_4_c,bit_4_d,bit_4_e,bit_4_h,bit_4_l,bit_4_xhl,bit_4_a,
 bit_5_b,bit_5_c,bit_5_d,bit_5_e,bit_5_h,bit_5_l,bit_5_xhl,bit_5_a,
 bit_6_b,bit_6_c,bit_6_d,bit_6_e,bit_6_h,bit_6_l,bit_6_xhl,bit_6_a,
 bit_7_b,bit_7_c,bit_7_d,bit_7_e,bit_7_h,bit_7_l,bit_7_xhl,bit_7_a,
 res_0_b,res_0_c,res_0_d,res_0_e,res_0_h,res_0_l,res_0_xhl,res_0_a,
 res_1_b,res_1_c,res_1_d,res_1_e,res_1_h,res_1_l,res_1_xhl,res_1_a,
 res_2_b,res_2_c,res_2_d,res_2_e,res_2_h,res_2_l,res_2_xhl,res_2_a,
 res_3_b,res_3_c,res_3_d,res_3_e,res_3_h,res_3_l,res_3_xhl,res_3_a,
 res_4_b,res_4_c,res_4_d,res_4_e,res_4_h,res_4_l,res_4_xhl,res_4_a,
 res_5_b,res_5_c,res_5_d,res_5_e,res_5_h,res_5_l,res_5_xhl,res_5_a,
 res_6_b,res_6_c,res_6_d,res_6_e,res_6_h,res_6_l,res_6_xhl,res_6_a,
 res_7_b,res_7_c,res_7_d,res_7_e,res_7_h,res_7_l,res_7_xhl,res_7_a,
 set_0_b,set_0_c,set_0_d,set_0_e,set_0_h,set_0_l,set_0_xhl,set_0_a,
 set_1_b,set_1_c,set_1_d,set_1_e,set_1_h,set_1_l,set_1_xhl,set_1_a,
 set_2_b,set_2_c,set_2_d,set_2_e,set_2_h,set_2_l,set_2_xhl,set_2_a,
 set_3_b,set_3_c,set_3_d,set_3_e,set_3_h,set_3_l,set_3_xhl,set_3_a,
 set_4_b,set_4_c,set_4_d,set_4_e,set_4_h,set_4_l,set_4_xhl,set_4_a,
 set_5_b,set_5_c,set_5_d,set_5_e,set_5_h,set_5_l,set_5_xhl,set_5_a,
 set_6_b,set_6_c,set_6_d,set_6_e,set_6_h,set_6_l,set_6_xhl,set_6_a,
 set_7_b,set_7_c,set_7_d,set_7_e,set_7_h,set_7_l,set_7_xhl,set_7_a
};

static opcode_fn R800_opcode_dd[256]=
{
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,add_ix_bc ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,add_ix_de ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,ld_ix_word,ld_xword_ix,inc_ix   ,inc_ixh    ,dec_ixh    ,ld_ixh_byte,no_op   ,
  no_op   ,add_ix_ix ,ld_ix_xword,dec_ix   ,inc_ixl    ,dec_ixl    ,ld_ixl_byte,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,inc_xix    ,dec_xix    ,ld_xix_byte,no_op   ,
  no_op   ,add_ix_sp ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_b_ixh   ,ld_b_ixl   ,ld_b_xix   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_c_ixh   ,ld_c_ixl   ,ld_c_xix   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_d_ixh   ,ld_d_ixl   ,ld_d_xix   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_e_ixh   ,ld_e_ixl   ,ld_e_xix   ,no_op   ,
  ld_ixh_b,ld_ixh_c  ,ld_ixh_d   ,ld_ixh_e ,ld_ixh_ixh ,ld_ixh_ixl ,ld_h_xix   ,ld_ixh_a,
  ld_ixl_b,ld_ixl_c  ,ld_ixl_d   ,ld_ixl_e ,ld_ixl_ixh ,ld_ixl_ixl ,ld_l_xix   ,ld_ixl_a,
  ld_xix_b,ld_xix_c  ,ld_xix_d   ,ld_xix_e ,ld_xix_h   ,ld_xix_l   ,no_op      ,ld_xix_a,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_a_ixh   ,ld_a_ixl   ,ld_a_xix   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,add_a_ixh  ,add_a_ixl  ,add_a_xix  ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,adc_a_ixh  ,adc_a_ixl  ,adc_a_xix  ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,sub_ixh    ,sub_ixl    ,sub_xix    ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,sbc_a_ixh  ,sbc_a_ixl  ,sbc_a_xix  ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,and_ixh    ,and_ixl    ,and_xix    ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,xor_ixh    ,xor_ixl    ,xor_xix    ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,or_ixh     ,or_ixl     ,or_xix     ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,cp_ixh     ,cp_ixl     ,cp_xix     ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,dd_cb    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,pop_ix    ,no_op      ,ex_xsp_ix,no_op      ,push_ix    ,no_op      ,no_op   ,
  no_op   ,jp_ix     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,ld_sp_ix  ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op
};

static opcode_fn R800_opcode_ed[256]=
{
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 in_b_c,out_c_b,sbc_hl_bc,ld_xword_bc,neg,retn,im_0 ,ld_i_a,
 in_c_c,out_c_c,adc_hl_bc,ld_bc_xword,neg,reti,im_0 ,ld_r_a,
 in_d_c,out_c_d,sbc_hl_de,ld_xword_de,neg,retn,im_1 ,ld_a_i,
 in_e_c,out_c_e,adc_hl_de,ld_de_xword,neg,reti,im_2 ,ld_a_r,
 in_h_c,out_c_h,sbc_hl_hl,ld_xword_hl,neg,retn,im_0 ,rrd   ,
 in_l_c,out_c_l,adc_hl_hl,ld_hl_xword,neg,reti,im_0 ,rld   ,
 in_0_c,out_c_0,sbc_hl_sp,ld_xword_sp,neg,retn,im_1 ,nop   ,
 in_a_c,out_c_a,adc_hl_sp,ld_sp_xword,neg,reti,im_2 ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 ldi   ,cpi    ,ini      ,outi       ,nop,nop ,nop  ,nop   ,
 ldd   ,cpd    ,ind      ,outd       ,nop,nop ,nop  ,nop   ,
 ldir  ,cpir   ,inir     ,otir       ,nop,nop ,nop  ,nop   ,
 lddr  ,cpdr   ,indr     ,otdr       ,nop,nop ,nop  ,nop   , 
// This codes are different from the Z80 
 nop   ,mulu_a_b,nop     ,muluw_hl_bc,nop,nop ,nop  ,nop   ,
 nop   ,mulu_a_c,nop     ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,mulu_a_d,nop     ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,mulu_a_e,nop     ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,nop        ,nop,nop ,nop  ,nop   ,
 nop   ,nop    ,nop      ,muluw_hl_sp,nop,nop ,nop  ,nop   , 
//End of extra instructions 
 nop   ,nop    ,nop      ,nop        ,nop,nop ,patch,nop
};


static opcode_fn R800_opcode_fd[256]=
{
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,add_iy_bc ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,add_iy_de ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,ld_iy_word,ld_xword_iy,inc_iy   ,inc_iyh    ,dec_iyh    ,ld_iyh_byte,no_op   ,
  no_op   ,add_iy_iy ,ld_iy_xword,dec_iy   ,inc_iyl    ,dec_iyl    ,ld_iyl_byte,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,inc_xiy    ,dec_xiy    ,ld_xiy_byte,no_op   ,
  no_op   ,add_iy_sp ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_b_iyh   ,ld_b_iyl   ,ld_b_xiy   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_c_iyh   ,ld_c_iyl   ,ld_c_xiy   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_d_iyh   ,ld_d_iyl   ,ld_d_xiy   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_e_iyh   ,ld_e_iyl   ,ld_e_xiy   ,no_op   ,
  ld_iyh_b,ld_iyh_c  ,ld_iyh_d   ,ld_iyh_e ,ld_iyh_iyh ,ld_iyh_iyl ,ld_h_xiy   ,ld_iyh_a,
  ld_iyl_b,ld_iyl_c  ,ld_iyl_d   ,ld_iyl_e ,ld_iyl_iyh ,ld_iyl_iyl ,ld_l_xiy   ,ld_iyl_a,
  ld_xiy_b,ld_xiy_c  ,ld_xiy_d   ,ld_xiy_e ,ld_xiy_h   ,ld_xiy_l   ,no_op      ,ld_xiy_a,
  no_op   ,no_op     ,no_op      ,no_op    ,ld_a_iyh   ,ld_a_iyl   ,ld_a_xiy   ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,add_a_iyh  ,add_a_iyl  ,add_a_xiy  ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,adc_a_iyh  ,adc_a_iyl  ,adc_a_xiy  ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,sub_iyh    ,sub_iyl    ,sub_xiy    ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,sbc_a_iyh  ,sbc_a_iyl  ,sbc_a_xiy  ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,and_iyh    ,and_iyl    ,and_xiy    ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,xor_iyh    ,xor_iyl    ,xor_xiy    ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,or_iyh     ,or_iyl     ,or_xiy     ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,cp_iyh     ,cp_iyl     ,cp_xiy     ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,fd_cb    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,pop_iy    ,no_op      ,ex_xsp_iy,no_op      ,push_iy    ,no_op      ,no_op   ,
  no_op   ,jp_iy     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,no_op     ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op   ,
  no_op   ,ld_sp_iy  ,no_op      ,no_op    ,no_op      ,no_op      ,no_op      ,no_op
};


static opcode_fn R800_opcode_main[256]=
{
 nop     ,ld_bc_word,ld_xbc_a   ,inc_bc    ,inc_b   ,dec_b   ,ld_b_byte  ,rlca    ,
 ex_af_af,add_hl_bc ,ld_a_xbc   ,dec_bc    ,inc_c   ,dec_c   ,ld_c_byte  ,rrca    ,
 djnz    ,ld_de_word,ld_xde_a   ,inc_de    ,inc_d   ,dec_d   ,ld_d_byte  ,rla     ,
 jr      ,add_hl_de ,ld_a_xde   ,dec_de    ,inc_e   ,dec_e   ,ld_e_byte  ,rra     ,
 jr_nz   ,ld_hl_word,ld_xword_hl,inc_hl    ,inc_h   ,dec_h   ,ld_h_byte  ,daa     ,
 jr_z    ,add_hl_hl ,ld_hl_xword,dec_hl    ,inc_l   ,dec_l   ,ld_l_byte  ,cpl     ,
 jr_nc   ,ld_sp_word,ld_xbyte_a ,inc_sp    ,inc_xhl ,dec_xhl ,ld_xhl_byte,scf     ,
 jr_c    ,add_hl_sp ,ld_a_xbyte ,dec_sp    ,inc_a   ,dec_a   ,ld_a_byte  ,ccf     ,
 ld_b_b  ,ld_b_c    ,ld_b_d     ,ld_b_e    ,ld_b_h  ,ld_b_l  ,ld_b_xhl   ,ld_b_a  ,
 ld_c_b  ,ld_c_c    ,ld_c_d     ,ld_c_e    ,ld_c_h  ,ld_c_l  ,ld_c_xhl   ,ld_c_a  ,
 ld_d_b  ,ld_d_c    ,ld_d_d     ,ld_d_e    ,ld_d_h  ,ld_d_l  ,ld_d_xhl   ,ld_d_a  ,
 ld_e_b  ,ld_e_c    ,ld_e_d     ,ld_e_e    ,ld_e_h  ,ld_e_l  ,ld_e_xhl   ,ld_e_a  ,
 ld_h_b  ,ld_h_c    ,ld_h_d     ,ld_h_e    ,ld_h_h  ,ld_h_l  ,ld_h_xhl   ,ld_h_a  ,
 ld_l_b  ,ld_l_c    ,ld_l_d     ,ld_l_e    ,ld_l_h  ,ld_l_l  ,ld_l_xhl   ,ld_l_a  ,
 ld_xhl_b,ld_xhl_c  ,ld_xhl_d   ,ld_xhl_e  ,ld_xhl_h,ld_xhl_l,halt       ,ld_xhl_a,
 ld_a_b  ,ld_a_c    ,ld_a_d     ,ld_a_e    ,ld_a_h  ,ld_a_l  ,ld_a_xhl   ,ld_a_a  ,
 add_a_b ,add_a_c   ,add_a_d    ,add_a_e   ,add_a_h ,add_a_l ,add_a_xhl  ,add_a_a ,
 adc_a_b ,adc_a_c   ,adc_a_d    ,adc_a_e   ,adc_a_h ,adc_a_l ,adc_a_xhl  ,adc_a_a ,
 sub_b   ,sub_c     ,sub_d      ,sub_e     ,sub_h   ,sub_l   ,sub_xhl    ,sub_a   ,
 sbc_a_b ,sbc_a_c   ,sbc_a_d    ,sbc_a_e   ,sbc_a_h ,sbc_a_l ,sbc_a_xhl  ,sbc_a_a ,
 and_b   ,and_c     ,and_d      ,and_e     ,and_h   ,and_l   ,and_xhl    ,and_a   ,
 xor_b   ,xor_c     ,xor_d      ,xor_e     ,xor_h   ,xor_l   ,xor_xhl    ,xor_a   ,
 or_b    ,or_c      ,or_d       ,or_e      ,or_h    ,or_l    ,or_xhl     ,or_a    ,
 cp_b    ,cp_c      ,cp_d       ,cp_e      ,cp_h    ,cp_l    ,cp_xhl     ,cp_a    ,
 ret_nz  ,pop_bc    ,jp_nz      ,jp        ,call_nz ,push_bc ,add_a_byte ,rst_00  ,
 ret_z   ,ret       ,jp_z       ,cb        ,call_z  ,call    ,adc_a_byte ,rst_08  ,
 ret_nc  ,pop_de    ,jp_nc      ,out_byte_a,call_nc ,push_de ,sub_byte   ,rst_10  ,
 ret_c   ,exx       ,jp_c       ,in_a_byte ,call_c  ,dd      ,sbc_a_byte ,rst_18  ,
 ret_po  ,pop_hl    ,jp_po      ,ex_xsp_hl ,call_po ,push_hl ,and_byte   ,rst_20  ,
 ret_pe  ,jp_hl     ,jp_pe      ,ex_de_hl  ,call_pe ,ed      ,xor_byte   ,rst_28  ,
 ret_p   ,pop_af    ,jp_p       ,di        ,call_p  ,push_af ,or_byte    ,rst_30  ,
 ret_m   ,ld_sp_hl  ,jp_m       ,ei        ,call_m  ,fd      ,cp_byte    ,rst_38
};


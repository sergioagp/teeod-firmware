#ifndef __TEE_DEFINES_H__
#define __TEE_DEFINES_H__

#define REGSIZE 4

#define PARAM_SIZE  8

#define TEE_IRQn           0
#define TEE_INT_Msk       (1UL << ((TEE_IRQn) & 0x1FUL))

// Addrs for TEE
#define TEE_MSG_ADDR		        0x44A00000

// Relative ADDR for the teeod_msg_arg parameters
#define TEE_MSG_ARG_CMD_ADDR		    (TEE_MSG_ADDR + REGSIZE*0x00)
#define TEE_MSG_ARG_FUNC_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x01)
#define TEE_MSG_ARG_SESS_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x02)
#define TEE_MSG_ARG_CL_ID_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x03)
#define TEE_MSG_ARG_PAD_ADDR		    (TEE_MSG_ADDR + REGSIZE*0x04)
#define TEE_MSG_ARG_RET_ADDR		    (TEE_MSG_ADDR + REGSIZE*0x05)
#define TEE_MSG_ARG_RET_ORIGIN_ADDR     (TEE_MSG_ADDR + REGSIZE*0x06)	
#define TEE_MSG_ARG_NUM_PARAMS_ADDR     (TEE_MSG_ADDR + REGSIZE*0x07)
#define TEE_MSG_ARG_PARAM0_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x08)
#define TEE_MSG_ARG_PARAM1_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x10)
#define TEE_MSG_ARG_PARAM2_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x18)
#define TEE_MSG_ARG_PARAM3_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x20)
#define TEE_MSG_ARG_PARAM4_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x28)
#define TEE_MSG_ARG_PARAM5_ADDR	        (TEE_MSG_ADDR + REGSIZE*0x30)
#define TEE_MSG_ARG_RSRV0_ADDR          (TEE_MSG_ADDR + REGSIZE*0x38)
#define TEE_MSG_ARG_RSRV1_ADDR          (TEE_MSG_ADDR + REGSIZE*0x39)
#define TEE_MSG_ARG_RSRV2_ADDR          (TEE_MSG_ADDR + REGSIZE*0x3A)
#define TEE_MSG_ARG_RSRV3_ADDR          (TEE_MSG_ADDR + REGSIZE*0x3B)
#define TEE_MSG_ARG_RSRV4_ADDR          (TEE_MSG_ADDR + REGSIZE*0x3C)
#define TEE_MSG_ARG_RSRV5_ADDR          (TEE_MSG_ADDR + REGSIZE*0x3D)
#define TEE_MSG_ARG_INTCLR_ADDR    	    (TEE_MSG_ADDR + REGSIZE*0x3E)
#define TEE_MSG_ARG_DONE_ADDR    	    (TEE_MSG_ADDR + REGSIZE*0x3F)

#define ASM_TEE_MSG_ARG_INTCLR_ADDR    	0x44A000F8
#define ASM_TEE_MSG_ARG_DONE_ADDR    	0x44A000FC


// Relative ADDR for the teeod_msg_param parameters
#define TEE_MSG_PARAM_ATTR0         0x00
#define TEE_MSG_PARAM_ATTR1         0x01
#define TEE_MSG_PARAM_VAL0	        0x02
#define TEE_MSG_PARAM_VAL1	        0x03
#define TEE_MSG_PARAM_VAL2	        0x04
#define TEE_MSG_PARAM_VAL3	        0x05
#define TEE_MSG_PARAM_VAL4	        0x06
#define TEE_MSG_PARAM_VAL5	        0x07

// Relative ADDR for UUID 
#define TEE_MSG_ARG_UUID0           (TEE_MSG_ARG_PARAM0 + TEE_MSG_PARAM_VAL0)
#define TEE_MSG_ARG_UUID1           (TEE_MSG_ARG_PARAM0 + TEE_MSG_PARAM_VAL1)
#define TEE_MSG_ARG_UUID2           (TEE_MSG_ARG_PARAM0 + TEE_MSG_PARAM_VAL2)
#define TEE_MSG_ARG_UUID3           (TEE_MSG_ARG_PARAM0 + TEE_MSG_PARAM_VAL3)



// // values for TEEOD_MSG_ARG_CMD
#define TEE_MSG_CMD_OPEN_SESSION    0
#define TEE_MSG_CMD_INVOKE_COMMAND  1
#define TEE_MSG_CMD_CLOSE_SESSION	2

#endif  /*__TEE_DEFINES_H__*/
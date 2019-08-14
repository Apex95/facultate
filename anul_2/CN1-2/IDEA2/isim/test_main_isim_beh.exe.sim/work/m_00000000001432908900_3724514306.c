/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

/* This file is designed for use with ISim build 0x7708f090 */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "D:/IDEA2/keyshifter.v";
static int ng1[] = {0, 0};
static int ng2[] = {127, 0};
static int ng3[] = {1, 0};
static int ng4[] = {7, 0};
static int ng5[] = {128, 0};
static int ng6[] = {25, 0};
static int ng7[] = {103, 0};
static int ng8[] = {0, 0, 0, 0};
static int ng9[] = {832, 0};
static int ng10[] = {895, 0};



static void Always_28_0(char *t0)
{
    char t4[32];
    char t7[8];
    char t8[8];
    char t9[8];
    char t41[32];
    char t42[32];
    char t54[8];
    char t55[8];
    char t58[32];
    char t59[32];
    char t100[8];
    char t101[8];
    char t102[8];
    char t110[8];
    char *t1;
    char *t2;
    char *t3;
    char *t5;
    char *t6;
    char *t10;
    char *t11;
    char *t12;
    char *t13;
    char *t14;
    char *t15;
    unsigned int t16;
    int t17;
    char *t18;
    unsigned int t19;
    int t20;
    int t21;
    char *t22;
    unsigned int t23;
    int t24;
    int t25;
    unsigned int t26;
    int t27;
    unsigned int t28;
    unsigned int t29;
    int t30;
    int t31;
    char *t32;
    char *t33;
    char *t34;
    char *t35;
    char *t36;
    char *t37;
    char *t38;
    char *t39;
    char *t40;
    char *t43;
    char *t44;
    char *t45;
    char *t46;
    char *t47;
    char *t48;
    char *t49;
    char *t50;
    char *t51;
    char *t52;
    char *t53;
    char *t56;
    char *t57;
    unsigned int t60;
    unsigned int t61;
    char *t62;
    char *t63;
    char *t64;
    unsigned int t65;
    unsigned int t66;
    unsigned int t67;
    unsigned int t68;
    unsigned int t69;
    char *t70;
    unsigned int t71;
    char *t72;
    unsigned int t73;
    char *t74;
    unsigned int t75;
    unsigned int t76;
    unsigned int t77;
    unsigned int t78;
    unsigned int t79;
    unsigned int t80;
    unsigned int t81;
    unsigned int t82;
    char *t83;
    unsigned int t84;
    char *t85;
    char *t86;
    unsigned int t87;
    char *t88;
    unsigned int t89;
    unsigned int t90;
    unsigned int t91;
    unsigned int t92;
    unsigned int t93;
    unsigned int t94;
    unsigned int t95;
    unsigned int t96;
    unsigned int t97;
    unsigned int t98;
    char *t99;
    char *t103;
    char *t104;
    char *t105;
    char *t106;
    char *t107;
    char *t108;
    char *t109;
    char *t111;
    char *t112;
    unsigned int t113;
    char *t114;
    unsigned int t115;
    char *t116;
    unsigned int t117;
    unsigned int t118;
    unsigned int t119;
    unsigned int t120;
    int t121;
    int t122;

LAB0:    t1 = (t0 + 2688U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(28, ng0);
    t2 = (t0 + 3008);
    *((int *)t2) = 1;
    t3 = (t0 + 2720);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(29, ng0);

LAB5:    xsi_set_current_line(31, ng0);
    t5 = (t0 + 1048U);
    t6 = *((char **)t5);
    xsi_vlog_get_part_select_value(t4, 128, t6, 127, 0);
    t5 = (t0 + 1608);
    t10 = (t0 + 1608);
    t11 = (t10 + 72U);
    t12 = *((char **)t11);
    t13 = ((char*)((ng1)));
    t14 = ((char*)((ng2)));
    xsi_vlog_convert_partindices(t7, t8, t9, ((int*)(t12)), 2, t13, 32, 1, t14, 32, 1);
    t15 = (t7 + 4);
    t16 = *((unsigned int *)t15);
    t17 = (!(t16));
    t18 = (t8 + 4);
    t19 = *((unsigned int *)t18);
    t20 = (!(t19));
    t21 = (t17 && t20);
    t22 = (t9 + 4);
    t23 = *((unsigned int *)t22);
    t24 = (!(t23));
    t25 = (t21 && t24);
    if (t25 == 1)
        goto LAB6;

LAB7:    xsi_set_current_line(33, ng0);
    xsi_set_current_line(33, ng0);
    t2 = ((char*)((ng3)));
    t3 = (t0 + 1768);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 6);

LAB8:    t2 = (t0 + 1768);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng4)));
    memset(t7, 0, 8);
    t10 = (t5 + 4);
    if (*((unsigned int *)t10) != 0)
        goto LAB10;

LAB9:    t11 = (t6 + 4);
    if (*((unsigned int *)t11) != 0)
        goto LAB10;

LAB13:    if (*((unsigned int *)t5) < *((unsigned int *)t6))
        goto LAB11;

LAB12:    t13 = (t7 + 4);
    t16 = *((unsigned int *)t13);
    t19 = (~(t16));
    t23 = *((unsigned int *)t7);
    t26 = (t23 & t19);
    t28 = (t26 != 0);
    if (t28 > 0)
        goto LAB14;

LAB15:    xsi_set_current_line(38, ng0);
    t2 = ((char*)((ng8)));
    t3 = (t0 + 1608);
    t5 = (t0 + 1608);
    t6 = (t5 + 72U);
    t10 = *((char **)t6);
    t11 = ((char*)((ng9)));
    t12 = ((char*)((ng10)));
    xsi_vlog_convert_partindices(t7, t8, t9, ((int*)(t10)), 2, t11, 32, 1, t12, 32, 1);
    t13 = (t7 + 4);
    t16 = *((unsigned int *)t13);
    t17 = (!(t16));
    t14 = (t8 + 4);
    t19 = *((unsigned int *)t14);
    t20 = (!(t19));
    t21 = (t17 && t20);
    t15 = (t9 + 4);
    t23 = *((unsigned int *)t15);
    t24 = (!(t23));
    t25 = (t21 && t24);
    if (t25 == 1)
        goto LAB25;

LAB26:    goto LAB2;

LAB6:    t26 = *((unsigned int *)t9);
    t27 = (t26 + 0);
    t28 = *((unsigned int *)t7);
    t29 = *((unsigned int *)t8);
    t30 = (t28 - t29);
    t31 = (t30 + 1);
    xsi_vlogvar_assign_value(t5, t4, t27, *((unsigned int *)t8), t31);
    goto LAB7;

LAB10:    t12 = (t7 + 4);
    *((unsigned int *)t7) = 1;
    *((unsigned int *)t12) = 1;
    goto LAB12;

LAB11:    *((unsigned int *)t7) = 1;
    goto LAB12;

LAB14:    xsi_set_current_line(34, ng0);

LAB16:    xsi_set_current_line(35, ng0);
    t14 = (t0 + 1608);
    t15 = (t14 + 56U);
    t18 = *((char **)t15);
    t22 = (t0 + 1608);
    t32 = (t22 + 72U);
    t33 = *((char **)t32);
    t34 = ((char*)((ng5)));
    t35 = (t0 + 1768);
    t36 = (t35 + 56U);
    t37 = *((char **)t36);
    t38 = ((char*)((ng3)));
    memset(t8, 0, 8);
    xsi_vlog_unsigned_minus(t8, 32, t37, 6, t38, 32);
    memset(t9, 0, 8);
    xsi_vlog_unsigned_multiply(t9, 32, t34, 32, t8, 32);
    t39 = ((char*)((ng5)));
    xsi_vlog_get_indexed_partselect(t4, 128, t18, ((int*)(t33)), 2, t9, 32, 2, t39, 32, 1, 1);
    t40 = ((char*)((ng6)));
    xsi_vlog_unsigned_lshift(t41, 128, t4, 128, t40, 32);
    t43 = (t0 + 1608);
    t44 = (t43 + 56U);
    t45 = *((char **)t44);
    t46 = (t0 + 1608);
    t47 = (t46 + 72U);
    t48 = *((char **)t47);
    t49 = ((char*)((ng5)));
    t50 = (t0 + 1768);
    t51 = (t50 + 56U);
    t52 = *((char **)t51);
    t53 = ((char*)((ng3)));
    memset(t54, 0, 8);
    xsi_vlog_unsigned_minus(t54, 32, t52, 6, t53, 32);
    memset(t55, 0, 8);
    xsi_vlog_unsigned_multiply(t55, 32, t49, 32, t54, 32);
    t56 = ((char*)((ng5)));
    xsi_vlog_get_indexed_partselect(t42, 128, t45, ((int*)(t48)), 2, t55, 32, 2, t56, 32, 1, 1);
    t57 = ((char*)((ng7)));
    xsi_vlog_unsigned_rshift(t58, 128, t42, 128, t57, 32);
    t29 = 0;

LAB20:    t60 = (t29 < 4);
    if (t60 == 1)
        goto LAB21;

LAB22:    t99 = (t0 + 1608);
    t103 = (t0 + 1608);
    t104 = (t103 + 72U);
    t105 = *((char **)t104);
    t106 = ((char*)((ng5)));
    t107 = (t0 + 1768);
    t108 = (t107 + 56U);
    t109 = *((char **)t108);
    memset(t110, 0, 8);
    xsi_vlog_unsigned_multiply(t110, 32, t106, 32, t109, 6);
    t111 = ((char*)((ng5)));
    xsi_vlog_convert_indexed_partindices(t100, t101, t102, ((int*)(t105)), 2, t110, 32, 2, t111, 32, 1, 1);
    t112 = (t100 + 4);
    t113 = *((unsigned int *)t112);
    t21 = (!(t113));
    t114 = (t101 + 4);
    t115 = *((unsigned int *)t114);
    t24 = (!(t115));
    t25 = (t21 && t24);
    t116 = (t102 + 4);
    t117 = *((unsigned int *)t116);
    t27 = (!(t117));
    t30 = (t25 && t27);
    if (t30 == 1)
        goto LAB23;

LAB24:    xsi_set_current_line(33, ng0);
    t2 = (t0 + 1768);
    t3 = (t2 + 56U);
    t5 = *((char **)t3);
    t6 = ((char*)((ng3)));
    memset(t7, 0, 8);
    xsi_vlog_unsigned_add(t7, 32, t5, 6, t6, 32);
    t10 = (t0 + 1768);
    xsi_vlogvar_assign_value(t10, t7, 0, 0, 6);
    goto LAB8;

LAB17:    t80 = (t29 * 8);
    t81 = *((unsigned int *)t64);
    t82 = *((unsigned int *)t74);
    *((unsigned int *)t64) = (t81 | t82);
    t83 = (t41 + t80);
    t84 = (t80 + 4);
    t85 = (t41 + t84);
    t86 = (t58 + t80);
    t87 = (t80 + 4);
    t88 = (t58 + t87);
    t89 = *((unsigned int *)t85);
    t90 = (~(t89));
    t91 = *((unsigned int *)t83);
    t17 = (t91 & t90);
    t92 = *((unsigned int *)t88);
    t93 = (~(t92));
    t94 = *((unsigned int *)t86);
    t20 = (t94 & t93);
    t95 = (~(t17));
    t96 = (~(t20));
    t97 = *((unsigned int *)t74);
    *((unsigned int *)t74) = (t97 & t95);
    t98 = *((unsigned int *)t74);
    *((unsigned int *)t74) = (t98 & t96);

LAB19:    t29 = (t29 + 1);
    goto LAB20;

LAB18:    goto LAB19;

LAB21:    t61 = (t29 * 8);
    t62 = (t41 + t61);
    t63 = (t58 + t61);
    t64 = (t59 + t61);
    t65 = *((unsigned int *)t62);
    t66 = *((unsigned int *)t63);
    t67 = (t65 | t66);
    *((unsigned int *)t64) = t67;
    t68 = (t29 * 8);
    t69 = (t68 + 4);
    t70 = (t41 + t69);
    t71 = (t68 + 4);
    t72 = (t58 + t71);
    t73 = (t68 + 4);
    t74 = (t59 + t73);
    t75 = *((unsigned int *)t70);
    t76 = *((unsigned int *)t72);
    t77 = (t75 | t76);
    *((unsigned int *)t74) = t77;
    t78 = *((unsigned int *)t74);
    t79 = (t78 != 0);
    if (t79 == 1)
        goto LAB17;
    else
        goto LAB18;

LAB23:    t118 = *((unsigned int *)t102);
    t31 = (t118 + 0);
    t119 = *((unsigned int *)t100);
    t120 = *((unsigned int *)t101);
    t121 = (t119 - t120);
    t122 = (t121 + 1);
    xsi_vlogvar_assign_value(t99, t59, t31, *((unsigned int *)t101), t122);
    goto LAB24;

LAB25:    t26 = *((unsigned int *)t9);
    t27 = (t26 + 0);
    t28 = *((unsigned int *)t7);
    t29 = *((unsigned int *)t8);
    t30 = (t28 - t29);
    t31 = (t30 + 1);
    xsi_vlogvar_assign_value(t3, t2, t27, *((unsigned int *)t8), t31);
    goto LAB26;

}


extern void work_m_00000000001432908900_3724514306_init()
{
	static char *pe[] = {(void *)Always_28_0};
	xsi_register_didat("work_m_00000000001432908900_3724514306", "isim/test_main_isim_beh.exe.sim/work/m_00000000001432908900_3724514306.didat");
	xsi_register_executes(pe);
}

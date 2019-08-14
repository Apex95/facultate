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

/* This file is designed for use with ISim build 0xfbc00daa */

#define XSI_HIDE_SYMBOL_SPEC true
#include "xsi.h"
#include <memory.h>
#ifdef __GNUC__
#include <stdlib.h>
#else
#include <malloc.h>
#define alloca _alloca
#endif
static const char *ng0 = "/home/alexm/Desktop/pipeMult/pipeDecomp.v";
static int ng1[] = {0, 0};
static int ng2[] = {1, 0};



static void Always_32_0(char *t0)
{
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    unsigned int t6;
    unsigned int t7;
    unsigned int t8;
    unsigned int t9;
    unsigned int t10;
    char *t11;
    char *t12;

LAB0:    t1 = (t0 + 4600U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(32, ng0);
    t2 = (t0 + 5168);
    *((int *)t2) = 1;
    t3 = (t0 + 4632);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(32, ng0);

LAB5:    xsi_set_current_line(33, ng0);
    t4 = (t0 + 1208U);
    t5 = *((char **)t4);
    t4 = (t5 + 4);
    t6 = *((unsigned int *)t4);
    t7 = (~(t6));
    t8 = *((unsigned int *)t5);
    t9 = (t8 & t7);
    t10 = (t9 != 0);
    if (t10 > 0)
        goto LAB6;

LAB7:    xsi_set_current_line(42, ng0);

LAB10:    xsi_set_current_line(43, ng0);
    t2 = (t0 + 2888);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 1928);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 1);
    xsi_set_current_line(44, ng0);
    t2 = (t0 + 3048);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 2088);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 1);
    xsi_set_current_line(45, ng0);
    t2 = (t0 + 3208);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 2248);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(46, ng0);
    t2 = (t0 + 3368);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 2408);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 8);
    xsi_set_current_line(47, ng0);
    t2 = (t0 + 3528);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 2568);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 23);
    xsi_set_current_line(48, ng0);
    t2 = (t0 + 3688);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 2728);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 23);

LAB8:    goto LAB2;

LAB6:    xsi_set_current_line(33, ng0);

LAB9:    xsi_set_current_line(34, ng0);
    t11 = ((char*)((ng1)));
    t12 = (t0 + 1928);
    xsi_vlogvar_assign_value(t12, t11, 0, 0, 1);
    xsi_set_current_line(35, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 2088);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(36, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 2248);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 8);
    xsi_set_current_line(37, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 2408);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 8);
    xsi_set_current_line(38, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 2568);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 23);
    xsi_set_current_line(39, ng0);
    t2 = ((char*)((ng1)));
    t3 = (t0 + 2728);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 23);
    goto LAB8;

}

static void Always_52_1(char *t0)
{
    char t6[8];
    char t11[8];
    char t27[8];
    char t41[8];
    char t46[8];
    char t62[8];
    char t70[8];
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    char *t12;
    char *t13;
    unsigned int t14;
    unsigned int t15;
    unsigned int t16;
    unsigned int t17;
    unsigned int t18;
    unsigned int t19;
    unsigned int t20;
    unsigned int t21;
    unsigned int t22;
    unsigned int t23;
    unsigned int t24;
    unsigned int t25;
    char *t26;
    char *t28;
    unsigned int t29;
    unsigned int t30;
    unsigned int t31;
    unsigned int t32;
    unsigned int t33;
    char *t34;
    char *t35;
    unsigned int t36;
    unsigned int t37;
    unsigned int t38;
    char *t39;
    char *t40;
    char *t42;
    char *t43;
    char *t44;
    char *t45;
    char *t47;
    char *t48;
    unsigned int t49;
    unsigned int t50;
    unsigned int t51;
    unsigned int t52;
    unsigned int t53;
    unsigned int t54;
    unsigned int t55;
    unsigned int t56;
    unsigned int t57;
    unsigned int t58;
    unsigned int t59;
    unsigned int t60;
    char *t61;
    char *t63;
    unsigned int t64;
    unsigned int t65;
    unsigned int t66;
    unsigned int t67;
    unsigned int t68;
    char *t69;
    unsigned int t71;
    unsigned int t72;
    unsigned int t73;
    char *t74;
    char *t75;
    char *t76;
    unsigned int t77;
    unsigned int t78;
    unsigned int t79;
    unsigned int t80;
    unsigned int t81;
    unsigned int t82;
    unsigned int t83;
    char *t84;
    char *t85;
    unsigned int t86;
    unsigned int t87;
    unsigned int t88;
    unsigned int t89;
    unsigned int t90;
    unsigned int t91;
    unsigned int t92;
    unsigned int t93;
    int t94;
    int t95;
    unsigned int t96;
    unsigned int t97;
    unsigned int t98;
    unsigned int t99;
    unsigned int t100;
    unsigned int t101;
    char *t102;
    unsigned int t103;
    unsigned int t104;
    unsigned int t105;
    unsigned int t106;
    unsigned int t107;
    char *t108;
    char *t109;

LAB0:    t1 = (t0 + 4848U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(52, ng0);
    t2 = (t0 + 5184);
    *((int *)t2) = 1;
    t3 = (t0 + 4880);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(52, ng0);

LAB5:    xsi_set_current_line(53, ng0);
    t4 = (t0 + 1368U);
    t5 = *((char **)t4);
    t4 = (t0 + 1328U);
    t7 = (t4 + 72U);
    t8 = *((char **)t7);
    t9 = ((char*)((ng1)));
    xsi_vlog_generic_get_index_select_value(t6, 32, t5, t8, 2, t9, 32, 1);
    t10 = ((char*)((ng2)));
    memset(t11, 0, 8);
    t12 = (t6 + 4);
    t13 = (t10 + 4);
    t14 = *((unsigned int *)t6);
    t15 = *((unsigned int *)t10);
    t16 = (t14 ^ t15);
    t17 = *((unsigned int *)t12);
    t18 = *((unsigned int *)t13);
    t19 = (t17 ^ t18);
    t20 = (t16 | t19);
    t21 = *((unsigned int *)t12);
    t22 = *((unsigned int *)t13);
    t23 = (t21 | t22);
    t24 = (~(t23));
    t25 = (t20 & t24);
    if (t25 != 0)
        goto LAB9;

LAB6:    if (t23 != 0)
        goto LAB8;

LAB7:    *((unsigned int *)t11) = 1;

LAB9:    memset(t27, 0, 8);
    t28 = (t11 + 4);
    t29 = *((unsigned int *)t28);
    t30 = (~(t29));
    t31 = *((unsigned int *)t11);
    t32 = (t31 & t30);
    t33 = (t32 & 1U);
    if (t33 != 0)
        goto LAB10;

LAB11:    if (*((unsigned int *)t28) != 0)
        goto LAB12;

LAB13:    t35 = (t27 + 4);
    t36 = *((unsigned int *)t27);
    t37 = *((unsigned int *)t35);
    t38 = (t36 || t37);
    if (t38 > 0)
        goto LAB14;

LAB15:    memcpy(t70, t27, 8);

LAB16:    t102 = (t70 + 4);
    t103 = *((unsigned int *)t102);
    t104 = (~(t103));
    t105 = *((unsigned int *)t70);
    t106 = (t105 & t104);
    t107 = (t106 != 0);
    if (t107 > 0)
        goto LAB28;

LAB29:    xsi_set_current_line(62, ng0);

LAB32:    xsi_set_current_line(63, ng0);
    t2 = (t0 + 1368U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 31);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t17 = *((unsigned int *)t4);
    t18 = (t17 >> 31);
    t19 = (t18 & 1);
    *((unsigned int *)t2) = t19;
    t5 = (t0 + 2888);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 1);
    xsi_set_current_line(64, ng0);
    t2 = (t0 + 1368U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 23);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 23);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 255U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 255U);
    t5 = (t0 + 3208);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 8);
    xsi_set_current_line(65, ng0);
    t2 = (t0 + 1368U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 0);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 0);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 8388607U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 8388607U);
    t5 = (t0 + 3528);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 23);
    xsi_set_current_line(66, ng0);
    t2 = (t0 + 1528U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 31);
    t16 = (t15 & 1);
    *((unsigned int *)t6) = t16;
    t17 = *((unsigned int *)t4);
    t18 = (t17 >> 31);
    t19 = (t18 & 1);
    *((unsigned int *)t2) = t19;
    t5 = (t0 + 3048);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 1);
    xsi_set_current_line(67, ng0);
    t2 = (t0 + 1528U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 23);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 23);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 255U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 255U);
    t5 = (t0 + 3368);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 8);
    xsi_set_current_line(68, ng0);
    t2 = (t0 + 1528U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 0);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 0);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 8388607U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 8388607U);
    t5 = (t0 + 3688);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 23);

LAB30:    goto LAB2;

LAB8:    t26 = (t11 + 4);
    *((unsigned int *)t11) = 1;
    *((unsigned int *)t26) = 1;
    goto LAB9;

LAB10:    *((unsigned int *)t27) = 1;
    goto LAB13;

LAB12:    t34 = (t27 + 4);
    *((unsigned int *)t27) = 1;
    *((unsigned int *)t34) = 1;
    goto LAB13;

LAB14:    t39 = (t0 + 1528U);
    t40 = *((char **)t39);
    t39 = (t0 + 1488U);
    t42 = (t39 + 72U);
    t43 = *((char **)t42);
    t44 = ((char*)((ng1)));
    xsi_vlog_generic_get_index_select_value(t41, 32, t40, t43, 2, t44, 32, 1);
    t45 = ((char*)((ng1)));
    memset(t46, 0, 8);
    t47 = (t41 + 4);
    t48 = (t45 + 4);
    t49 = *((unsigned int *)t41);
    t50 = *((unsigned int *)t45);
    t51 = (t49 ^ t50);
    t52 = *((unsigned int *)t47);
    t53 = *((unsigned int *)t48);
    t54 = (t52 ^ t53);
    t55 = (t51 | t54);
    t56 = *((unsigned int *)t47);
    t57 = *((unsigned int *)t48);
    t58 = (t56 | t57);
    t59 = (~(t58));
    t60 = (t55 & t59);
    if (t60 != 0)
        goto LAB20;

LAB17:    if (t58 != 0)
        goto LAB19;

LAB18:    *((unsigned int *)t46) = 1;

LAB20:    memset(t62, 0, 8);
    t63 = (t46 + 4);
    t64 = *((unsigned int *)t63);
    t65 = (~(t64));
    t66 = *((unsigned int *)t46);
    t67 = (t66 & t65);
    t68 = (t67 & 1U);
    if (t68 != 0)
        goto LAB21;

LAB22:    if (*((unsigned int *)t63) != 0)
        goto LAB23;

LAB24:    t71 = *((unsigned int *)t27);
    t72 = *((unsigned int *)t62);
    t73 = (t71 & t72);
    *((unsigned int *)t70) = t73;
    t74 = (t27 + 4);
    t75 = (t62 + 4);
    t76 = (t70 + 4);
    t77 = *((unsigned int *)t74);
    t78 = *((unsigned int *)t75);
    t79 = (t77 | t78);
    *((unsigned int *)t76) = t79;
    t80 = *((unsigned int *)t76);
    t81 = (t80 != 0);
    if (t81 == 1)
        goto LAB25;

LAB26:
LAB27:    goto LAB16;

LAB19:    t61 = (t46 + 4);
    *((unsigned int *)t46) = 1;
    *((unsigned int *)t61) = 1;
    goto LAB20;

LAB21:    *((unsigned int *)t62) = 1;
    goto LAB24;

LAB23:    t69 = (t62 + 4);
    *((unsigned int *)t62) = 1;
    *((unsigned int *)t69) = 1;
    goto LAB24;

LAB25:    t82 = *((unsigned int *)t70);
    t83 = *((unsigned int *)t76);
    *((unsigned int *)t70) = (t82 | t83);
    t84 = (t27 + 4);
    t85 = (t62 + 4);
    t86 = *((unsigned int *)t27);
    t87 = (~(t86));
    t88 = *((unsigned int *)t84);
    t89 = (~(t88));
    t90 = *((unsigned int *)t62);
    t91 = (~(t90));
    t92 = *((unsigned int *)t85);
    t93 = (~(t92));
    t94 = (t87 & t89);
    t95 = (t91 & t93);
    t96 = (~(t94));
    t97 = (~(t95));
    t98 = *((unsigned int *)t76);
    *((unsigned int *)t76) = (t98 & t96);
    t99 = *((unsigned int *)t76);
    *((unsigned int *)t76) = (t99 & t97);
    t100 = *((unsigned int *)t70);
    *((unsigned int *)t70) = (t100 & t96);
    t101 = *((unsigned int *)t70);
    *((unsigned int *)t70) = (t101 & t97);
    goto LAB27;

LAB28:    xsi_set_current_line(53, ng0);

LAB31:    xsi_set_current_line(54, ng0);
    t108 = ((char*)((ng1)));
    t109 = (t0 + 2888);
    xsi_vlogvar_assign_value(t109, t108, 0, 0, 1);
    xsi_set_current_line(55, ng0);
    t2 = (t0 + 1528U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 23);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 23);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 255U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 255U);
    t5 = (t0 + 3208);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 8);
    xsi_set_current_line(56, ng0);
    t2 = (t0 + 1528U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 0);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 0);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 8388607U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 8388607U);
    t5 = (t0 + 3528);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 23);
    xsi_set_current_line(57, ng0);
    t2 = ((char*)((ng2)));
    t3 = (t0 + 3048);
    xsi_vlogvar_assign_value(t3, t2, 0, 0, 1);
    xsi_set_current_line(58, ng0);
    t2 = (t0 + 1368U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 23);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 23);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 255U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 255U);
    t5 = (t0 + 3368);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 8);
    xsi_set_current_line(59, ng0);
    t2 = (t0 + 1368U);
    t3 = *((char **)t2);
    memset(t6, 0, 8);
    t2 = (t6 + 4);
    t4 = (t3 + 4);
    t14 = *((unsigned int *)t3);
    t15 = (t14 >> 0);
    *((unsigned int *)t6) = t15;
    t16 = *((unsigned int *)t4);
    t17 = (t16 >> 0);
    *((unsigned int *)t2) = t17;
    t18 = *((unsigned int *)t6);
    *((unsigned int *)t6) = (t18 & 8388607U);
    t19 = *((unsigned int *)t2);
    *((unsigned int *)t2) = (t19 & 8388607U);
    t5 = (t0 + 3688);
    xsi_vlogvar_assign_value(t5, t6, 0, 0, 23);
    goto LAB30;

}


extern void work_m_06408031384974879564_1260441629_init()
{
	static char *pe[] = {(void *)Always_32_0,(void *)Always_52_1};
	xsi_register_didat("work_m_06408031384974879564_1260441629", "isim/testPipe_isim_beh.exe.sim/work/m_06408031384974879564_1260441629.didat");
	xsi_register_executes(pe);
}

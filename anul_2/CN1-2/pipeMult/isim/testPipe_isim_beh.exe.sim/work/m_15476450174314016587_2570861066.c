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
static const char *ng0 = "/home/alexm/Desktop/pipeMult/pipeFin.v";
static int ng1[] = {0, 0};
static int ng2[] = {1, 0};
static int ng3[] = {8, 0};
static int ng4[] = {9, 0};
static int ng5[] = {31, 0};
static const char *ng6 = "res = %b";



static void Always_29_0(char *t0)
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

LAB0:    t1 = (t0 + 3160U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(29, ng0);
    t2 = (t0 + 3728);
    *((int *)t2) = 1;
    t3 = (t0 + 3192);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(29, ng0);

LAB5:    xsi_set_current_line(30, ng0);
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

LAB7:    xsi_set_current_line(33, ng0);
    t2 = (t0 + 2248);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    t5 = (t0 + 2088);
    xsi_vlogvar_assign_value(t5, t4, 0, 0, 32);

LAB8:    goto LAB2;

LAB6:    xsi_set_current_line(31, ng0);
    t11 = ((char*)((ng1)));
    t12 = (t0 + 2088);
    xsi_vlogvar_assign_value(t12, t11, 0, 0, 32);
    goto LAB8;

}

static void Always_36_1(char *t0)
{
    char t6[8];
    char t14[8];
    char t15[8];
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    char *t7;
    char *t8;
    char *t9;
    char *t10;
    char *t11;
    unsigned int t12;
    int t13;
    unsigned int t16;
    int t17;
    int t18;
    char *t19;
    unsigned int t20;
    int t21;
    int t22;
    unsigned int t23;
    int t24;
    unsigned int t25;
    unsigned int t26;
    int t27;
    int t28;

LAB0:    t1 = (t0 + 3408U);
    t2 = *((char **)t1);
    if (t2 == 0)
        goto LAB2;

LAB3:    goto *t2;

LAB2:    xsi_set_current_line(36, ng0);
    t2 = (t0 + 3744);
    *((int *)t2) = 1;
    t3 = (t0 + 3440);
    *((char **)t3) = t2;
    *((char **)t1) = &&LAB4;

LAB1:    return;
LAB4:    xsi_set_current_line(36, ng0);

LAB5:    xsi_set_current_line(37, ng0);
    t4 = (t0 + 1368U);
    t5 = *((char **)t4);
    t4 = (t0 + 2248);
    t7 = (t0 + 2248);
    t8 = (t7 + 72U);
    t9 = *((char **)t8);
    t10 = ((char*)((ng1)));
    xsi_vlog_generic_convert_bit_index(t6, t9, 2, t10, 32, 1);
    t11 = (t6 + 4);
    t12 = *((unsigned int *)t11);
    t13 = (!(t12));
    if (t13 == 1)
        goto LAB6;

LAB7:    xsi_set_current_line(38, ng0);
    t2 = (t0 + 1528U);
    t3 = *((char **)t2);
    t2 = (t0 + 2248);
    t4 = (t0 + 2248);
    t5 = (t4 + 72U);
    t7 = *((char **)t5);
    t8 = ((char*)((ng2)));
    t9 = ((char*)((ng3)));
    xsi_vlog_convert_partindices(t6, t14, t15, ((int*)(t7)), 2, t8, 32, 1, t9, 32, 1);
    t10 = (t6 + 4);
    t12 = *((unsigned int *)t10);
    t13 = (!(t12));
    t11 = (t14 + 4);
    t16 = *((unsigned int *)t11);
    t17 = (!(t16));
    t18 = (t13 && t17);
    t19 = (t15 + 4);
    t20 = *((unsigned int *)t19);
    t21 = (!(t20));
    t22 = (t18 && t21);
    if (t22 == 1)
        goto LAB8;

LAB9:    xsi_set_current_line(39, ng0);
    t2 = (t0 + 1688U);
    t3 = *((char **)t2);
    t2 = (t0 + 2248);
    t4 = (t0 + 2248);
    t5 = (t4 + 72U);
    t7 = *((char **)t5);
    t8 = ((char*)((ng4)));
    t9 = ((char*)((ng5)));
    xsi_vlog_convert_partindices(t6, t14, t15, ((int*)(t7)), 2, t8, 32, 1, t9, 32, 1);
    t10 = (t6 + 4);
    t12 = *((unsigned int *)t10);
    t13 = (!(t12));
    t11 = (t14 + 4);
    t16 = *((unsigned int *)t11);
    t17 = (!(t16));
    t18 = (t13 && t17);
    t19 = (t15 + 4);
    t20 = *((unsigned int *)t19);
    t21 = (!(t20));
    t22 = (t18 && t21);
    if (t22 == 1)
        goto LAB10;

LAB11:    xsi_set_current_line(41, ng0);
    t2 = (t0 + 2248);
    t3 = (t2 + 56U);
    t4 = *((char **)t3);
    xsi_vlogfile_write(1, 0, 0, ng6, 2, t0, (char)118, t4, 32);
    goto LAB2;

LAB6:    xsi_vlogvar_assign_value(t4, t5, 0, *((unsigned int *)t6), 1);
    goto LAB7;

LAB8:    t23 = *((unsigned int *)t15);
    t24 = (t23 + 0);
    t25 = *((unsigned int *)t6);
    t26 = *((unsigned int *)t14);
    t27 = (t25 - t26);
    t28 = (t27 + 1);
    xsi_vlogvar_assign_value(t2, t3, t24, *((unsigned int *)t14), t28);
    goto LAB9;

LAB10:    t23 = *((unsigned int *)t15);
    t24 = (t23 + 0);
    t25 = *((unsigned int *)t6);
    t26 = *((unsigned int *)t14);
    t27 = (t25 - t26);
    t28 = (t27 + 1);
    xsi_vlogvar_assign_value(t2, t3, t24, *((unsigned int *)t14), t28);
    goto LAB11;

}


extern void work_m_15476450174314016587_2570861066_init()
{
	static char *pe[] = {(void *)Always_29_0,(void *)Always_36_1};
	xsi_register_didat("work_m_15476450174314016587_2570861066", "isim/testPipe_isim_beh.exe.sim/work/m_15476450174314016587_2570861066.didat");
	xsi_register_executes(pe);
}

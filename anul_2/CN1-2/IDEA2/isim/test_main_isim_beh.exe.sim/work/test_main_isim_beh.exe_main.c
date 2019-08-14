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

#include "xsi.h"

struct XSI_INFO xsi_info;



int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    work_m_00000000001432908900_3724514306_init();
    work_m_00000000001242850379_1137053943_init();
    work_m_00000000003296414130_3408695897_init();
    work_m_00000000001060505493_0286164271_init();
    work_m_00000000001041382181_4042999134_init();
    work_m_00000000004134447467_2073120511_init();


    xsi_register_tops("work_m_00000000001041382181_4042999134");
    xsi_register_tops("work_m_00000000004134447467_2073120511");


    return xsi_run_simulation(argc, argv);

}

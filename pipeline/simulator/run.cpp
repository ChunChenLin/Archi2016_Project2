#include "run.h"

void run_pipeline() {
	if(Register::cycle == 59) {
            printf("in main before WB ID_EX.$rt_out = %d\n", ID_EX.$rt_out);
        }
	WB();
	if(Register::cycle == 59) {
            printf("in main before DM ID_EX.$rt_out = %d\n", ID_EX.$rt_out);
        }
    DM();
    if(Register::cycle == 59) {
            printf("in main before EX ID_EX.$rt_out = %d\n", ID_EX.$rt_out);
        }
    EX();
    if(Register::cycle == 59) {
            printf("in main before ID ID_EX.$rt_out = %d\n", ID_EX.$rt_out);
        }
    ID();
    if(Register::cycle == 59) {
            printf("in main before IF ID_EX.$rt_out = %d\n", ID_EX.$rt_out);
        }
    IF();
    if(Register::cycle == 59) {
            printf("in main after IF ID_EX.$rt_out = %d\n", ID_EX.$rt_out);
        }
}
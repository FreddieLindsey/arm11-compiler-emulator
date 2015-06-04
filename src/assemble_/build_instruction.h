#ifndef INSTRUCTIONS_H 
#define INSTRUCTIONS_H 

/* DATA TRANSFER */
decoded_instruction_t *build_add(char **args);
decoded_instruction_t *build_sub(char **args);
decoded_instruction_t *build_rsb(char **args);
decoded_instruction_t *build_and(char **args);
decoded_instruction_t *build_eor(char **args);
decoded_instruction_t *build_orr(char **args);
decoded_instruction_t *build_mov(char **args);
decoded_instruction_t *build_tst(char **args);
decoded_instruction_t *build_teq(char **args);
decoded_instruction_t *build_cmp(char **args);

/* MULTIPLY */
decoded_instruction_t *build_mul(char **args);
decoded_instruction_t *build_mla(char **args);

/* SINGLE DATA TRANSFER */
decoded_instruction_t *build_ldr(char **args, output_data_t *out, int pos);
decoded_instruction_t *build_str(char **args, output_data_t *out, int pos);

/* BRANCH */
decoded_instruction_t *build_beq(int offset);
decoded_instruction_t *build_bne(int offset);
decoded_instruction_t *build_bge(int offset);
decoded_instruction_t *build_blt(int offset);
decoded_instruction_t *build_bgt(int offset);
decoded_instruction_t *build_ble(int offset);
decoded_instruction_t *build_b(int offset);

/* SPECIAL */
decoded_instruction_t *build_andeq(void);
decoded_instruction_t *build_lsl(char **args);

#endif

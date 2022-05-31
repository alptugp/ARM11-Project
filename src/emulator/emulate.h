typedef uint32_t word;

struct RegisterFile {
    word general_purpose[GENERAL_PURPOSE_REGISTERS];
    word lr;
    word sp;
    word program_counter;
    word cpsr;
};
#ifndef SEQ2SEQ_H
#define SEQ2SEQ_H

// Seq2Seq Configuration
#define NUM_LAYERS 1
#define HIDDEN_SIZE 128
#define INPUT_SIZE 70
#define OUTPUT_SIZE 3
#define WEIGHTS_SIZE 384



// Encoder
void encoder(const float input[1][1], float output_first_cell[1][HIDDEN_SIZE] , float output_second_cell[1][HIDDEN_SIZE]);

// Decoder
void decoder(const float encoder_state_1[1][HIDDEN_SIZE],const float encoder_state_2[1][HIDDEN_SIZE], float output[INPUT_SIZE][OUTPUT_SIZE]);

// Seq2Seq Model
void seq2seqmodel(const float encoder_input[INPUT_SIZE][1], float decoder_output[INPUT_SIZE][OUTPUT_SIZE]);


#endif // SEQ2SEQ_H
